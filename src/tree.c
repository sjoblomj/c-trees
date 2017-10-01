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
#include <stdlib.h>
#define _(String) gettext (String)

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <gdk/gdkpixbuf.h>

#include "tree.h"
#include "vnrfile.h"

typedef enum {RIGHT, LEFT} Direction;
typedef enum {CONTINUE, RETREAT} Course;


struct Preference_Settings {
    gboolean include_hidden;
    gboolean include_dirs;
    gboolean set_file_monitor_for_file;
};




static GNode*
vnr_file_dir_content_to_list(VnrFile  *vnrfile,
                             struct Preference_Settings* preference_settings,
                             GError   **error);

static gboolean
vnr_file_get_file_info(char *filepath,
                       VnrFile **vnrfile,
                       gboolean include_hidden,
                       GError **error);

static void
vnr_file_set_file_monitor(GNode* tree, struct Preference_Settings* preference_settings);


GList * supported_mime_types;


GNode* init() {
    return g_node_new(NULL);
}

gint compare_quarks (gconstpointer a, gconstpointer b) {
    GQuark quark = g_quark_from_string ((const gchar *) a);
    return quark - GPOINTER_TO_INT (b);
}


static gint vnr_file_list_compare(gconstpointer a, gconstpointer b) {
    return g_strcmp0(VNR_FILE(a)->display_name_collate,
                     VNR_FILE(b)->display_name_collate);
}



/* Modified version of eog's eog_image_get_supported_mime_types */
static GList * vnr_file_get_supported_mime_types(void) {
    GSList *format_list, *it;
    gchar **mime_types;
    int i;

    if(!supported_mime_types) {
        format_list = gdk_pixbuf_get_formats();

        for(it = format_list; it != NULL; it = it->next) {
            mime_types = gdk_pixbuf_format_get_mime_types ((GdkPixbufFormat *) it->data);

            for(i = 0; mime_types[i] != NULL; i++) {
                supported_mime_types = g_list_prepend(supported_mime_types, g_strdup(mime_types[i]));
            }

            g_strfreev(mime_types);
        }

        supported_mime_types = g_list_prepend(supported_mime_types, "image/vnd.microsoft.icon");
        supported_mime_types = g_list_sort(supported_mime_types, (GCompareFunc) compare_quarks);

        g_slist_free(format_list);
    }

    return supported_mime_types;
}

static gboolean vnr_file_is_supported_mime_type(const char *mime_type) {
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


static struct Preference_Settings* create_preference_settings(gboolean include_hidden, gboolean  include_dirs, gboolean set_file_monitor_for_file) {
    struct Preference_Settings* preference_settings = malloc(sizeof(*preference_settings));
    preference_settings->include_hidden = include_hidden;
    preference_settings->include_dirs = include_dirs;
    preference_settings->set_file_monitor_for_file = set_file_monitor_for_file;
    return preference_settings;
}




static void
vnr_file_directory_updated(GFileMonitor       *monitor,
                           GFile              *file,
                           GFile              *other_file,
                           GFileMonitorEvent   type,
                           gpointer            data)
{
    char *file_path;
    VnrFile* vnrfile_new = NULL;

    VnrFile* vnrfile = data;
    GNode* tree = (vnrfile->context)->tree;
    gboolean include_hidden = (vnrfile->context)->include_hidden;
    gboolean include_dirs = (vnrfile->context)->include_dirs;
    gboolean set_file_monitor_for_file = (vnrfile->context)->set_file_monitor_for_file;

    switch (type) {
        case G_FILE_MONITOR_EVENT_DELETED:

            file_path = g_file_get_path(file);
            GNode* child = get_child_in_directory(tree, file_path);

            if(child != NULL) {

                g_node_unlink(child);
                free_tree(child);
            }

            g_free(file_path);
            break;

        case G_FILE_MONITOR_EVENT_CREATED:
            file_path = g_file_get_path(file);
            vnr_file_get_file_info(file_path,
                                   &vnrfile_new,
                                   include_hidden,
                                   NULL);

            GNode *newnode = g_node_new(vnrfile_new);

            if(vnr_file_is_directory(vnrfile_new)) {
                if(include_dirs) {
                    // Newly created directory. It might already have been populated.

                    struct Preference_Settings* preference_settings = create_preference_settings(include_hidden, include_dirs, set_file_monitor_for_file);

                    GNode* subtree = vnr_file_dir_content_to_list(vnrfile_new, preference_settings, NULL);
                    add_node_in_tree(tree, subtree);
                    vnr_file_set_file_monitor(newnode, preference_settings);

                    free(preference_settings);
                }

            } else {
                add_node_in_tree(tree, newnode);
            }

            g_free(file_path);
            break;

        default:
            break;
    }
}



static void
vnr_file_set_file_monitor(GNode* tree, struct Preference_Settings* preference_settings)
{
    VnrFile* vnrfile = tree->data;
    GFile *file = g_file_new_for_path(vnrfile->path);
    // It's not a problem if directory monitoring isn't supported,
    // so set error to NULL.
    vnrfile->monitor = g_file_monitor(file,
                                      G_FILE_MONITOR_NONE,
                                      NULL,
                                      NULL);
    g_object_unref(file);

    if (vnrfile->monitor) {
        // This will be freed when the VnrFile is destroyed.
        struct Context* context = malloc(sizeof(*context));
        context->tree = tree;
        context->include_hidden = preference_settings->include_hidden;
        context->include_dirs = preference_settings->include_dirs;
        context->set_file_monitor_for_file = preference_settings->set_file_monitor_for_file;
        vnrfile->context = context;

        g_signal_connect(vnrfile->monitor,
                         "changed",
                         G_CALLBACK(vnr_file_directory_updated),
                         vnrfile);
    }
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

    if(file_info_success && (include_hidden || !g_file_info_get_is_hidden(fileinfo))) {
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


static void
vnr_file_add_file_to_lists_if_possible(gchar *filepath,
                                       GList **dir_list,
                                       GList **file_list,
                                       struct Preference_Settings* preference_settings,
                                       GError **error)
{
    gboolean file_info_ok;
    VnrFile *vnrfile;

    file_info_ok = vnr_file_get_file_info(filepath,
                                          &vnrfile,
                                          preference_settings->include_hidden,
                                          error);

    if(file_info_ok && vnr_file_is_directory(vnrfile) && preference_settings->include_dirs) {
        *dir_list  = g_list_prepend(*dir_list,  vnrfile);
    } else if(file_info_ok && vnr_file_is_image_file(vnrfile)) {
        *file_list = g_list_prepend(*file_list, vnrfile);
    } else if(vnrfile != NULL) {
        vnr_file_destroy_data(vnrfile);
    }
}

static void
vnr_append_file_and_dir_lists_to_tree(GNode    **tree,
                                      GList    **dir_list,
                                      GList    **file_list,
                                      struct Preference_Settings* preference_settings,
                                      GError   **error)
{
    GNode *subtree = NULL;

    *dir_list  = g_list_sort( *dir_list, vnr_file_list_compare);
    *file_list = g_list_sort(*file_list, vnr_file_list_compare);

    while(*file_list != NULL) {
        subtree = g_node_new((*file_list)->data);
        add_node_in_tree(*tree, subtree);

        if(preference_settings->set_file_monitor_for_file) {
            vnr_file_set_file_monitor(subtree, preference_settings);
        }

        *file_list = g_list_next(*file_list);
    }

    struct Preference_Settings* dir_preference_settings = create_preference_settings(preference_settings->include_hidden,
                                                                                        preference_settings->include_dirs,
                                                                                        FALSE);
    while(*dir_list != NULL) {

        subtree = vnr_file_dir_content_to_list((*dir_list)->data,
                                               dir_preference_settings,
                                               error);
        vnr_file_set_file_monitor(subtree, preference_settings);

        add_node_in_tree(*tree, subtree);
        *dir_list = g_list_next(*dir_list);
    }

    free(dir_preference_settings);
    g_free(*dir_list);
    g_free(*file_list);
}


static GNode*
vnr_file_dir_content_to_list(VnrFile  *vnrfile,
                             struct Preference_Settings* preference_settings,
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


    while(file_info != NULL) {
        char* child_path = g_strjoin(G_DIR_SEPARATOR_S, folder_path,
                                       (char*)g_file_info_get_name (file_info), NULL);

        vnr_file_add_file_to_lists_if_possible(child_path,
                                               &dir_list,
                                               &file_list,
                                               preference_settings,
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
                                          preference_settings,
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

    struct Preference_Settings* preference_settings = create_preference_settings(include_hidden, include_dirs, FALSE);

    file_info_ok = vnr_file_get_file_info(p_path,
                                          &vnrfile,
                                          include_hidden,
                                          error);

    if(file_info_ok && vnrfile != NULL && vnrfile->is_directory) {
        tree = vnr_file_dir_content_to_list(vnrfile,
                                            preference_settings,
                                            error);
        vnr_file_set_file_monitor(tree, preference_settings);


    } else if(file_info_ok && vnrfile != NULL) {
        vnr_file_destroy_data(vnrfile);
        char* parent_path = vnr_get_parent_file_path(p_path);

        file_info_ok = vnr_file_get_file_info(parent_path,
                                              &vnrfile,
                                              include_hidden,
                                              error);

        if(file_info_ok && vnrfile != NULL) {
            tree = vnr_file_dir_content_to_list(vnrfile,
                                                preference_settings,
                                                error);
        }
    }

    free(preference_settings);
    return tree;
}

GNode*
vnr_file_load_uri_list(GSList *uri_list, gboolean include_hidden, gboolean include_dirs, GError **error)
{
    GNode *tree      = g_node_new(NULL);
    GList *dir_list  = NULL;
    GList *file_list = NULL;


    struct Preference_Settings* dir_preference_settings = create_preference_settings(include_hidden, TRUE, TRUE);

    while(uri_list != NULL) {

        vnr_file_add_file_to_lists_if_possible(uri_list->data,
                                               &dir_list,
                                               &file_list,
                                               dir_preference_settings,
                                               error);
        g_clear_error(error);
        uri_list = g_slist_next(uri_list);
    }

    struct Preference_Settings* preference_settings = create_preference_settings(include_hidden, include_dirs, TRUE);

    vnr_append_file_and_dir_lists_to_tree(&tree,
                                          &dir_list,
                                          &file_list,
                                          preference_settings,
                                          error);
    free(dir_preference_settings);
    free(preference_settings);
    return tree;
}






static gboolean has_more_siblings_in_direction(GNode *tree, Direction direction) {
    return tree != (direction == RIGHT ? g_node_last_sibling(tree) : g_node_first_sibling(tree));
}

static GNode* get_prev_or_next(GNode* tree, Direction direction) {
    return direction == RIGHT ? g_node_next_sibling(tree) : g_node_prev_sibling(tree);
}
static GNode* get_first_or_last(GNode* tree, Direction direction) {
    return direction == RIGHT ? g_node_first_child(tree) : g_node_last_child(tree);
}

static GNode* recursively_find_prev_or_next(GNode *tree, GNode *original_node, Direction direction, Course course) {

    if(tree == NULL || tree == original_node) {
        return original_node;
    }
    if(is_leaf(tree)) {
        return tree;
    }

    // It is a directory.

    GNode *node;
    Course new_course = CONTINUE;
    if(G_NODE_IS_ROOT(tree)) {
        node = get_first_or_last(tree, direction);

    } else if(has_children(tree) && course != RETREAT) {
        node = get_first_or_last(tree, direction);

    } else if(has_more_siblings_in_direction(tree, direction)) {
        node = get_prev_or_next(tree, direction);

    } else {
        node = tree->parent;
        new_course = RETREAT;
    }
    return recursively_find_prev_or_next(node, original_node, direction, new_course);
}

static GNode* get_prev_or_next_in_tree(GNode *tree, Direction direction) {

    if(tree == NULL) {
        return NULL;
    }
    GNode *next = tree;
    Course course = CONTINUE;

    if(G_NODE_IS_ROOT(tree)) {
        next = get_first_or_last(tree, direction);

    } else if(is_leaf(tree) && has_more_siblings_in_direction(tree, direction)) {
        next = get_prev_or_next(tree, direction);

    } else if(is_leaf(tree) && !has_more_siblings_in_direction(tree, direction)) {
        next = tree->parent;
        course = RETREAT;

    } else if(!is_leaf(tree) && has_children(tree)) {
        next = get_first_or_last(tree, direction);

    } else if(!is_leaf(tree) && has_more_siblings_in_direction(tree, direction)) {
        next = get_prev_or_next(tree, direction);

    } else if(!is_leaf(tree) && !has_children(tree) && !has_more_siblings_in_direction(tree, direction)) {
        next = tree->parent;
        course = RETREAT;
    }

    return recursively_find_prev_or_next(next, tree, direction, course);
}

gboolean found_position_of_node(GNode *currnode, GNode *newnode) {
    if(currnode == NULL) {
        return TRUE;
    }

    gboolean currnode_is_dir = !is_leaf(currnode);
    gboolean newnode_is_file = is_leaf(newnode);
    gboolean both_nodes_are_of_same_type = is_leaf(currnode) == is_leaf(newnode);
    return (currnode_is_dir && newnode_is_file) ||
            (both_nodes_are_of_same_type && g_strcmp0(((VnrFile*) currnode->data)->display_name, ((VnrFile*) newnode->data)->display_name) > 0);
}

void add_node_in_tree(GNode *tree, GNode *node) {
    GNode *child = get_first_or_last(tree, RIGHT);

    int i = 0;
    while(!found_position_of_node(child, node)) {
        i++;
        if(!has_more_siblings(child)) {
            break;
        }
        child = g_node_next_sibling(child);
    }
    g_node_insert(tree, i, node);
}




GNode* get_child_in_directory(GNode *tree, char* path) {
    if(tree == NULL) {
        return NULL;
    }
    GNode* child = get_first_or_last(tree, RIGHT);

    if(child == NULL && has_children(tree)) {
        // This should be the top node in a uriList.
        child = get_next_in_tree(tree);
    } else if(child == NULL && tree->parent != NULL) {
        child = get_first_or_last(tree->parent, RIGHT);
    }

    VnrFile* vnrfile = child != NULL ? child->data : NULL;

    while(child != NULL && g_strcmp0(vnrfile != NULL ? vnrfile->path : "", path) != 0) {
        child = get_prev_or_next(child, RIGHT);
        vnrfile = child != NULL ? child->data : NULL;
    }
    return child;
}

gboolean is_leaf(GNode *node) {
    VnrFile* vnrfile = node->data;
    return vnrfile != NULL && !vnrfile->is_directory; // A leaf in the tree can represent an empty directory. Otherwise we could do G_NODE_IS_LEAF(node)
}

gboolean has_children(GNode *tree) {
    return g_node_n_children(tree) > 0;
}

gboolean has_more_siblings(GNode *tree) {
    return has_more_siblings_in_direction(tree, RIGHT);
}

GNode* get_next_in_tree(GNode *tree) {
    return get_prev_or_next_in_tree(tree, RIGHT);
}

GNode* get_prev_in_tree(GNode *tree) {
    return get_prev_or_next_in_tree(tree, LEFT);
}

static gboolean destroy_node(GNode *node, gpointer data) {
    vnr_file_destroy_data(node->data);
    return FALSE;
}

void free_tree(GNode *tree) {
    g_node_traverse(tree, G_POST_ORDER, G_TRAVERSE_ALL, -1, destroy_node, NULL);
    g_node_destroy(tree);
}
