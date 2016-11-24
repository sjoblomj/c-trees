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

#include "vnrfile.h"

#include <libintl.h>
#include <glib/gi18n.h>
#define _(String) gettext (String)

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <gdk/gdkpixbuf.h>

G_DEFINE_TYPE (VnrFile, vnr_file, G_TYPE_OBJECT);


static void
vnr_file_class_init (VnrFileClass *klass)
{
}

static void
vnr_file_init(VnrFile *file)
{
    file->display_name = NULL;
}

VnrFile *
vnr_file_new()
{
    return VNR_FILE (g_object_new (VNR_TYPE_FILE, NULL));
}

static void
vnr_file_set_display_name(VnrFile *vnr_file, char *display_name)
{
    vnr_file->display_name = display_name;
    vnr_file->display_name_collate = g_utf8_collate_key_for_filename(display_name, -1);
}


static void
vnr_file_set_file_info(VnrFile *vnrfile, char *path, char *display_name, gboolean is_directory)
{
    vnrfile->path = g_strdup(path);
    vnrfile->is_directory = is_directory;
    vnr_file_set_display_name(vnrfile, display_name);
}

VnrFile*
vnr_file_create_new(gchar *path,
                    char *display_name,
                    gboolean is_directory)
{
    VnrFile *vnrfile;
    vnrfile = vnr_file_new();
    vnr_file_set_file_info(vnrfile, path, display_name, is_directory);
    return vnrfile;
}

void
vnr_file_destroy_data(VnrFile *vnrfile)
{
    g_free(vnrfile->path);
    g_free(vnrfile->display_name);
    g_object_unref(vnrfile);
}