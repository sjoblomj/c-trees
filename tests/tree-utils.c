/*
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../src/tree.h"
#include "../src/vnrfile.h"
#include "tree-utils.h"

char* output;
int output_offset = 0;

static void print_tree(GNode *tree, char* tree_structure_base);


char* create_string(char* base_str, char* append_str) {
    char* new_str = malloc(strlen(base_str) + strlen(append_str) + 1);
    strcpy(new_str, base_str);
    strcat(new_str, append_str);
    return new_str;
}



static char* get_file_name(GNode* node) {
    if(node->data != NULL) {
        VnrFile* vnrfile = node->data;
        return vnrfile->display_name;
    }
    return "<ROOT>";
}

static gboolean is_leaf(GNode *node) {
    VnrFile* vnrfile = node->data;
    return vnrfile != NULL && !vnrfile->is_directory; // A leaf in the tree can represent an empty directory. Otherwise we could do G_NODE_IS_LEAF(node)
}

static gboolean has_children(GNode *tree) {
    return g_node_n_children(tree) > 0;
}

static gboolean has_more_siblings(GNode *tree, Direction direction) {
    return tree != (direction == RIGHT ? g_node_last_sibling(tree) : g_node_first_sibling(tree));
}


static void print_node(GNode *node, gpointer data) {
    if(is_leaf(node)) {
        char* append_str = has_more_siblings(node, RIGHT) ? "├─ " : "└─ ";
        char* tree_structure = create_string((char*) data, append_str);

        output_offset += snprintf(output + output_offset, OUTPUTSIZE - output_offset, "%s%s\n", tree_structure, get_file_name(node));

        free(tree_structure);

    } else {
        char* tree_structure = create_string((char*) data, "");
        print_tree(node, tree_structure);
        free(tree_structure);
    }
}


static void print_tree(GNode *tree, char* tree_structure_base) {

    char* tree_structure_end = "";
    if (!G_NODE_IS_ROOT(tree)) {
        if (has_more_siblings(tree, RIGHT)) {
            tree_structure_end = has_children(tree) ? "├─┬" : "├──";
        } else {
            tree_structure_end = has_children(tree) ? "└─┬" : "└──";
        }
    }

    output_offset += snprintf(output + output_offset, OUTPUTSIZE - output_offset, "%s%s" KWHT "%s  (%i children)" RESET "\n",
                              tree_structure_base, tree_structure_end, get_file_name(tree), g_node_n_children(tree));

    char* append_str = (G_NODE_IS_ROOT(tree) ? "" : (has_more_siblings(tree, RIGHT) ? "│ " : "  "));
    char* tree_structure = create_string(tree_structure_base, append_str);

    g_node_children_foreach(tree,
                            G_TRAVERSE_ALL,
                            print_node,
                            tree_structure);

    free(tree_structure);
}

char* pretty_print_tree(GNode *tree, char* out) {
	output = out;
	output_offset = 0;
	print_tree(tree, "");
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

    } else if(has_more_siblings(tree, direction)) {
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

    } else if(is_leaf(tree) && has_more_siblings(tree, direction)) {
        next = get_prev_or_next(tree, direction);

    } else if(is_leaf(tree) && !has_more_siblings(tree, direction)) {
        next = tree->parent;
        course = RETREAT;

    } else if(!is_leaf(tree) && has_children(tree)) {
        next = get_first_or_last(tree, direction);

    } else if(!is_leaf(tree) && has_more_siblings(tree, direction)) {
        next = get_prev_or_next(tree, direction);

    } else if(!is_leaf(tree) && !has_children(tree) && !has_more_siblings(tree, direction)) {
        next = tree->parent;
        course = RETREAT;
    }

    return recursively_find_prev_or_next(next, tree, direction, course);
}

GNode* get_next_in_tree(GNode *tree) {
    return get_prev_or_next_in_tree(tree, RIGHT);
}

GNode* get_prev_in_tree(GNode *tree) {
    return get_prev_or_next_in_tree(tree, LEFT);
}
