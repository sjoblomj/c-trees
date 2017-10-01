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

#ifndef tree_H
#define tree_H

#include <glib.h>


GNode*   vnr_file_load_uri_list  (GSList *uri_list,
                                  gboolean include_hidden,
                                  gboolean recursive,
                                  GError **error);

GNode*   vnr_file_load_single_uri(char *p_uri,
                                  gboolean include_hidden,
                                  gboolean recursive,
                                  GError **error);

void add_node_in_tree(GNode *tree, GNode *node);
GNode* get_next_in_tree(GNode *tree);
GNode* get_prev_in_tree(GNode *tree);
GNode* get_child_in_directory(GNode *tree, char* path);

gboolean is_leaf(GNode *node);
gboolean has_children(GNode *tree);
gboolean has_more_siblings(GNode *tree);

void free_tree(GNode *tree);

#endif // tree_H
