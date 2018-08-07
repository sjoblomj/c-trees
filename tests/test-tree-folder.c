/*
 * Copyright © 2009-2014 Siyan Panayotov <siyan.panayotov@gmail.com>
 * Copyright © 2016-2018 Johan Sjöblom <sjoblomj88@gmail.com>
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

#include "test-tree-folder.h"
#include "utils.h"


static void test_singleFolder_DontIncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME RESET " (3 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    GNode *tree = get_tree(SINGLE_FILE, FALSE, FALSE);
    assert_numbers_equals("#Leaves Single folder ─ Include hidden files: F ─ Recursive: F", 3, get_total_number_of_leaves(tree));

    assert_equals("Single folder ─ Include hidden files: F ─ Recursive: F", expected, print_and_free_tree(tree));

    after();
}

static void test_singleFolder_DontIncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIRNAME RESET " (5 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one" RESET " (1 children)\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two" RESET " (7 children)\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four" RESET " (1 children)\n\
  │ └──" KWHT "subsub" RESET " (0 children)\n\
  ├─┬" KWHT "sub_dir_one" RESET " (3 children)\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three" RESET " (0 children)\n\
  └─┬" KWHT "sub_dir_two" RESET " (4 children)\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";
    GNode *tree = get_tree(SINGLE_FILE, FALSE, TRUE);
    assert_numbers_equals("#Leaves Single folder ─ Include hidden files: F ─ Recursive: T", 14, get_total_number_of_leaves(tree));

    assert_equals("Single folder ─ Include hidden files: F ─ Recursive: T", expected, print_and_free_tree(tree));

    after();
}

static void test_singleFolder_IncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME RESET " (5 children)\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";
    GNode *tree = get_tree(SINGLE_FILE, TRUE, FALSE);
    assert_numbers_equals("#Leaves Single folder ─ Include hidden files: T ─ Recursive: F", 5, get_total_number_of_leaves(tree));

    assert_equals("Single folder ─ Include hidden files: T ─ Recursive: F", expected, print_and_free_tree(tree));

    after();
}

static void test_singleFolder_IncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIRNAME RESET " (7 children)\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one" RESET " (3 children)\n\
│ ├─ .three.png\n\
│ ├─ two.jpg\n\
│ └─┬" KWHT ".secrets" RESET " (1 children)\n\
│   └─ img.jpg\n\
└─┬" KWHT "dir_two" RESET " (7 children)\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four" RESET " (1 children)\n\
  │ └──" KWHT "subsub" RESET " (0 children)\n\
  ├─┬" KWHT "sub_dir_one" RESET " (3 children)\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three" RESET " (0 children)\n\
  └─┬" KWHT "sub_dir_two" RESET " (4 children)\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";
    GNode *tree = get_tree(SINGLE_FILE, TRUE, TRUE);
    assert_numbers_equals("#Leaves Single folder ─ Include hidden files: T ─ Recursive: T", 18, get_total_number_of_leaves(tree));

    assert_equals("Single folder ─ Include hidden files: T ─ Recursive: T", expected, print_and_free_tree(tree));

    after();
}



void test_tree_folder() {
    test_singleFolder_DontIncludeHidden_NotRecursive();
    test_singleFolder_DontIncludeHidden_Recursive();
    test_singleFolder_IncludeHidden_NotRecursive();
    test_singleFolder_IncludeHidden_Recursive();
}
