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

#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../src/tree.h"
#include "../src/vnrfile.h"
#include "tree-utils.h"
#include "monitor-tests.h"
#include "tree-tests.h"

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\033[1m\033[37m"
#define RESET "\x1B[0m"
#define OUTPUTSIZE 2048
#define TESTDIRNAME "c-trees-tests"
#define TESTDIR "/tmp/" TESTDIRNAME

char* output;

typedef enum {SINGLE_FILE, SINGLE_FOLDER, VALID_LIST, SEMI_INVALID_LIST, COMPLETELY_INVALID_LIST} inputtype;


GNode* single_file(gboolean include_hidden, gboolean recursive);
GNode* single_folder(gboolean include_hidden, gboolean recursive);
GNode* uri_list(gboolean include_hidden, gboolean recursive);
GNode* uri_list_with_some_invalid_entries(gboolean include_hidden, gboolean recursive);
GNode* uri_list_with_only_invalid_entries(gboolean include_hidden, gboolean recursive);
GNode* get_tree(inputtype type, gboolean include_hidden, gboolean recursive);
char* print_and_free_tree(GNode *tree);
char* get_printed_tree(inputtype type, gboolean include_hidden, gboolean recursive);
GNode* uri_list_with_no_entries(gboolean include_hidden, gboolean recursive);

void create_dir(const char *path);
void create_file(const char *path);
void create_file_structure();
int remove_directory(const char *path);
void remove_file_structure();

void before();
void after();
void reset_output();
void assert_equals(char* description, char* expected, char* actual);
void assert_number_of_leaves_equals(char* description, int expected, int actual);
void assert_trees_equal(char* description, GNode* expected, GNode* actual);
void assert_tree_is_null(char* description, GNode* tree);
void assert_error_is_null(GError* error);
void assert_error_is_not_null(GError* error);
void wait_until_tree_is_as_expected(GNode* tree, char* expected);

#endif /* __TEST_UTILS_H__ */
