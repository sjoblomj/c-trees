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

#ifndef __TREE_UTILS_H__
#define __TREE_UTILS_H__

#define KWHT  "\033[1m\033[37m"
#define RESET "\x1B[0m"
#define OUTPUTSIZE 2048

typedef enum {RIGHT, LEFT} Direction;
typedef enum {CONTINUE, RETREAT} Course;


GNode* get_next_in_tree(GNode *tree);
GNode* get_prev_in_tree(GNode *tree);

char* create_string(char* base_str, char* append_str);
char* pretty_print_tree(GNode *tree, char* out);

#endif /* __TREE_UTILS_H__ */

