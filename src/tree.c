/*
 * Copyright © 2009-2014 Siyan Panayotov <siyan.panayotov@gmail.com>
 * Copyright © 2016-2017 Johan Sjöblom <sjoblomj88@gmail.com>
 *
 * This file is part of c-trees.
 *
 * c-trees is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * c-trees is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with c-trees.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libintl.h>
#include <glib/gi18n.h>
#define _(String) gettext (String)

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <gdk/gdkpixbuf.h>

#include "tree.h"
#include "vnrfile.h"

static GNode*
vnr_file_dir_content_to_list(VnrFile  *vnrfile,
                             gboolean   include_hidden,
                             gboolean   include_dirs,
                             gboolean   set_file_monitor_for_file,
                             GError   **error);

GList * supported_mime_types;


GNode* init() {
    return g_node_new(NULL);
}

gint
compare_quarks (gconstpointer a, gconstpointer b)
{
    GQuark quark;

    quark = g_quark_from_string ((const gchar *) a);

    return quark - GPOINTER_TO_INT (b);
}


static gint
vnr_file_list_compare(gconstpointer a, gconstpointer b) {
    return g_strcmp0(VNR_FILE(a)->display_name_collate,
                     VNR_FILE(b)->display_name_collate);
}



/* Modified version of eog's eog_image_get_supported_mime_types */
static GList *
vnr_file_get_supported_mime_types(void)
{
    GSList *format_list, *it;
    gchar **mime_types;
    int i;

    if (!supported_mime_types) {
        format_list = gdk_pixbuf_get_formats ();

        for (it = format_list; it != NULL; it = it->next) {
            mime_types =
                gdk_pixbuf_format_get_mime_types ((GdkPixbufFormat *) it->data);

            for (i = 0; mime_types[i] != NULL; i++) {
                supported_mime_types =
                    g_list_prepend (supported_mime_types,
                            g_strdup (mime_types[i]));
            }

            g_strfreev (mime_types);
        }

        supported_mime_types = g_list_prepend(supported_mime_types,
                                              "image/vnd.microsoft.icon");

        supported_mime_types = g_list_sort (supported_mime_types,
                            (GCompareFunc) compare_quarks);

        g_slist_free (format_list);
    }

    return supported_mime_types;
}

static gboolean
vnr_file_is_supported_mime_type(const char *mime_type)
{
    GList *result;
    GQuark quark;

    if (mime_type == NULL) {
        return FALSE;
    }

    supported_mime_types = vnr_file_get_supported_mime_types ();

    quark = g_quark_from_string (mime_type);

    result = g_list_find_custom (supported_mime_types,
                     GINT_TO_POINTER (quark),
                     (GCompareFunc) compare_quarks);

    return (result != NULL);
}



static gboolean
vnr_file_get_file_info(char *filepath,
                       VnrFile **vnrfile,
                       gboolean include_hidden,
                       GError **error)
{
    GFile *file;
    GFileInfo *fileinfo;
    const char *mimetype;
    char* display_name;
    gboolean file_info_success;
    gboolean is_directory = FALSE;
    gboolean supported_mime_type = FALSE;

    *vnrfile = NULL;
    file = g_file_new_for_path(filepath);
    fileinfo = g_file_query_info(file,
                                 G_FILE_ATTRIBUTE_STANDARD_TYPE","
                                 G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME","
                                 G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE","
                                 G_FILE_ATTRIBUTE_STANDARD_IS_HIDDEN,
                                 0, NULL, error);
    file_info_success = fileinfo != NULL;

    if(file_info_success && (include_hidden || !g_file_info_get_is_hidden(fileinfo)))
    {
        is_directory = g_file_info_get_file_type(fileinfo) == G_FILE_TYPE_DIRECTORY;
        display_name = g_strdup(g_file_info_get_display_name(fileinfo));

        if(!is_directory) {
            mimetype = g_file_info_get_content_type(fileinfo);
            supported_mime_type = vnr_file_is_supported_mime_type(mimetype);
        }
        g_object_unref(fileinfo);

        if(is_directory || supported_mime_type) {
            *vnrfile = vnr_file_create_new(filepath, display_name, is_directory);
        }
    }
    g_object_unref(file);
    return file_info_success;
}

static gboolean
vnr_file_is_directory(VnrFile* vnrfile) {
    return vnrfile != NULL && vnrfile->is_directory;
}

static gboolean
vnr_file_is_image_file(VnrFile* vnrfile) {
    return vnrfile != NULL && !vnrfile->is_directory;
}

static void
vnr_file_add_file_to_lists_if_possible(gchar *filepath,
                                       GList **dir_list,
                                       GList **file_list,
                                       gboolean show_hidden,
                                       gboolean include_dirs,
                                       gboolean set_file_monitor_for_file,
                                       GError **error)
{
    gboolean file_info_ok;
    VnrFile *vnrfile;

    file_info_ok = vnr_file_get_file_info(filepath,
                                          &vnrfile,
                                          show_hidden,
                                          error);

    if(file_info_ok && vnr_file_is_directory(vnrfile) && include_dirs)
    {
        *dir_list = g_list_prepend(*dir_list, vnrfile);
    }
    else if(file_info_ok && vnr_file_is_image_file(vnrfile))
    {
        *file_list = g_list_prepend(*file_list, vnrfile);
    }
}

static void
vnr_append_file_and_dir_lists_to_tree(GNode    **tree,
                                      GList    **dir_list,
                                      GList    **file_list,
                                      gboolean   include_hidden,
                                      gboolean   include_dirs,
                                      gboolean   set_file_monitor_for_file,
                                      GError   **error)
{
    GNode *subtree = NULL;
    VnrFile* vnrfile;

    *dir_list  = g_list_sort( *dir_list, vnr_file_list_compare);
    *file_list = g_list_sort(*file_list, vnr_file_list_compare);

    while(*file_list != NULL) {
        GNode *tree_node = g_node_new((*file_list)->data);
        g_node_append(*tree, tree_node);

        *file_list = g_list_next(*file_list);
    }
    while(*dir_list != NULL) {
        subtree = vnr_file_dir_content_to_list((*dir_list)->data,
                                               include_hidden,
                                               include_dirs,
                                               set_file_monitor_for_file,
                                               error);

        g_node_append(*tree, subtree);
        *dir_list = g_list_next(*dir_list);
    }

    g_free(*dir_list);
    g_free(*file_list);
}


static GNode*
vnr_file_dir_content_to_list(VnrFile  *vnrfile,
                             gboolean   include_hidden,
                             gboolean   include_dirs,
                             gboolean   set_file_monitor_for_file,
                             GError   **error)
{
    GNode *tree       = g_node_new(vnrfile);
    GList *dir_list   = NULL;
    GList *file_list  = NULL;

    GFile *file;
    GFileEnumerator *f_enum;
    GFileInfo *file_info;

    char* folder_path = vnrfile->path;

    file   = g_file_new_for_path(folder_path);
    f_enum = g_file_enumerate_children(file, G_FILE_ATTRIBUTE_STANDARD_NAME","
                                       G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME","
                                       G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE","
                                       G_FILE_ATTRIBUTE_STANDARD_IS_HIDDEN,
                                       G_FILE_QUERY_INFO_NONE,
                                       NULL, NULL);
    file_info = g_file_enumerator_next_file(f_enum, NULL, NULL);


    while(file_info != NULL)
    {
        char* child_path = g_strjoin(G_DIR_SEPARATOR_S, folder_path,
                                       (char*)g_file_info_get_name (file_info), NULL);

        vnr_file_add_file_to_lists_if_possible(child_path,
                                               &dir_list,
                                               &file_list,
                                               include_hidden,
                                               include_dirs,
                                               set_file_monitor_for_file,
                                               error);

        g_object_unref(file_info);
        file_info = g_file_enumerator_next_file(f_enum, NULL, NULL);
    }

    g_object_unref(file);
    g_file_enumerator_close(f_enum, NULL, NULL);
    g_object_unref(f_enum);

    vnr_append_file_and_dir_lists_to_tree(&tree,
                                          &dir_list,
                                          &file_list,
                                          include_hidden,
                                          include_dirs,
                                          set_file_monitor_for_file,
                                          error);
    return tree;
}

static char*
vnr_get_parent_file_path(char *path)
{
    char* parent_path = NULL;

    GFile *file = g_file_new_for_path(path);
    GFile *parent = g_file_get_parent(file);

    parent_path = g_file_get_path(parent);

    g_object_unref(parent);
    g_object_unref(file);

    return parent_path;
}

GNode*
vnr_file_load_single_uri(char *p_path, gboolean include_hidden, gboolean include_dirs, GError **error)
{
    GNode *tree = NULL;
    VnrFile* vnrfile;
    gboolean file_info_ok;

    file_info_ok = vnr_file_get_file_info(p_path,
                                          &vnrfile,
                                          include_hidden,
                                          error);

    if(file_info_ok && vnrfile != NULL && vnrfile->is_directory) {
        tree = vnr_file_dir_content_to_list(vnrfile,
                                            include_hidden,
                                            include_dirs,
                                            FALSE,
                                            error);

    } else if(file_info_ok && vnrfile != NULL) {
        vnr_file_destroy_data(vnrfile);
        char* parent_path = vnr_get_parent_file_path(p_path);

        file_info_ok = vnr_file_get_file_info(parent_path,
                                              &vnrfile,
                                              include_hidden,
                                              error);

        if(file_info_ok && vnrfile != NULL) {
            tree = vnr_file_dir_content_to_list(vnrfile,
                                                include_hidden,
                                                include_dirs,
                                                FALSE,
                                                error);
        }
    }

    return tree;
}

GNode*
vnr_file_load_uri_list (GSList *uri_list, gboolean include_hidden, gboolean include_dirs, GError **error)
{
    GNode *tree      = g_node_new(NULL);
    GList *dir_list  = NULL;
    GList *file_list = NULL;

    while(uri_list != NULL) {
        vnr_file_add_file_to_lists_if_possible(uri_list->data,
                                               &dir_list,
                                               &file_list,
                                               include_hidden,
                                               TRUE,
                                               TRUE,
                                               error);
        g_clear_error(error);
        uri_list = g_slist_next(uri_list);
    }

    vnr_append_file_and_dir_lists_to_tree(&tree,
                                          &dir_list,
                                          &file_list,
                                          include_hidden,
                                          include_dirs,
                                          FALSE,
                                          error);
    return tree;
}
