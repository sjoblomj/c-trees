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

#include "test-tree-getchildindir.h"
#include "utils.h"


static void assert_child_is_equal(char* description, GNode* tree, char* expected) {
    VnrFile *vnrfile = tree != NULL ? tree->data : NULL;
    assert_equals(description, expected, vnrfile != NULL ? vnrfile->path : "NULL");
}


static void test_getChildInDirectory_FindFromRoot_FileExists() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// test-dir (7 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// ├─ epa.png
// ├─┬dir_one (3 children)
// │ ├─ .three.png
// │ ├─ two.jpg
// │ └─┬.secrets (1 children)
// │   └─ img.jpg
// └─┬dir_two (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four (1 children)
//   │ └──subsub (0 children)
//   ├─┬sub_dir_one (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three (0 children)
//   └─┬sub_dir_two (3 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    GNode *child0 = get_child_in_directory(tree, TESTDIR "/bepa.png");
    GNode *child1 = get_child_in_directory(tree, TESTDIR "/dir_two/bepa.png");

    assert_child_is_equal("Get child in directory ─ From root ─ File exists", child0, TESTDIR "/bepa.png");
    assert_child_is_equal("Get child in directory ─ From root ─ File exists", child1, TESTDIR "/dir_two/bepa.png");

    free_whole_tree(tree);
    after();
}

static void test_getChildInDirectory_FindFromFileAndDir_FileExists() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// test-dir (7 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// ├─ epa.png
// ├─┬dir_one (3 children)
// │ ├─ .three.png
// │ ├─ two.jpg
// │ └─┬.secrets (1 children)
// │   └─ img.jpg
// └─┬dir_two (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four (1 children)
//   │ └──subsub (0 children)
//   ├─┬sub_dir_one (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three (0 children)
//   └─┬sub_dir_two (3 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    GNode *child0 = get_child_in_directory(tree, TESTDIR "/bepa.png");
    GNode *child1 = g_node_last_child(get_root_node(tree));
    assert_child_is_equal("Get child in directory ─ Find bepa.png", child0, TESTDIR "/bepa.png");
    assert_child_is_equal("Get child in directory ─ Last child is dir_two", child1, TESTDIR "/dir_two");

    GNode *child2 = get_child_in_directory(child0, TESTDIR "/dir_two/bepa.png");
    GNode *child3 = get_child_in_directory(child1, TESTDIR "/dir_two/bepa.png");

    assert_child_is_equal("Get child in directory ─ From file ─ File exists", child2, TESTDIR "/dir_two/bepa.png");
    assert_child_is_equal("Get child in directory ─ From subdir ─ File exists", child3, TESTDIR "/dir_two/bepa.png");

    free_whole_tree(tree);
    after();
}

static void test_getChildInDirectory_FindAbove_FileExists() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// test-dir (7 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// ├─ epa.png
// ├─┬dir_one (3 children)
// │ ├─ .three.png
// │ ├─ two.jpg
// │ └─┬.secrets (1 children)
// │   └─ img.jpg
// └─┬dir_two (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four (1 children)
//   │ └──subsub (0 children)
//   ├─┬sub_dir_one (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three (0 children)
//   └─┬sub_dir_two (3 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    GNode *child0 = g_node_last_child(get_root_node(tree));
    assert_child_is_equal("Get child in directory ─ Find above ─ Last child is dir_two", child0, TESTDIR "/dir_two");

    GNode *child1 = get_child_in_directory(child0, TESTDIR "/epa.png");

    assert_child_is_equal("Get child in directory ─ Find above ─ File exists", child1, TESTDIR "/epa.png");

    free_whole_tree(tree);
    after();
}

static void test_getChildInDirectory_FileDoesNotExist() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// test-dir (7 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// ├─ epa.png
// ├─┬dir_one (3 children)
// │ ├─ .three.png
// │ ├─ two.jpg
// │ └─┬.secrets (1 children)
// │   └─ img.jpg
// └─┬dir_two (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four (1 children)
//   │ └──subsub (0 children)
//   ├─┬sub_dir_one (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three (0 children)
//   └─┬sub_dir_two (3 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    GNode *child = g_node_last_child(get_root_node(tree));
    assert_child_is_equal("Get child in directory ─ File does not exist ─ Last child is dir_two", child, TESTDIR "/dir_two");

    child = get_child_in_directory(child, TESTDIR "/dir_two/depa.png");

    assert_tree_is_null("Get child in directory ─ File does not exist", child);

    free_whole_tree(tree);
    after();
}

static void test_getChildInDirectory_SearchFromSubDirectoryIsEmpty() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// test-dir (7 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// ├─ epa.png
// ├─┬dir_one (3 children)
// │ ├─ .three.png
// │ ├─ two.jpg
// │ └─┬.secrets (1 children)
// │   └─ img.jpg
// └─┬dir_two (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four (1 children)
//   │ └──subsub (0 children)
//   ├─┬sub_dir_one (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three (0 children)
//   └─┬sub_dir_two (3 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    GNode *child = get_root_node(tree);
    child = g_node_last_child(child);
    child = g_node_first_child(child);
    child = g_node_next_sibling(child);
    child = g_node_next_sibling(child);
    child = g_node_next_sibling(child);
    child = g_node_first_child(child);
    assert_child_is_equal("Get child in directory ─ From subdir ─ Last child is subsub", child, TESTDIR "/dir_two/sub_dir_four/subsub");

    child = get_child_in_directory(child, TESTDIR "/.apa.png");

    assert_child_is_equal("Get child in directory ─ From subdir ─ File exists", child, TESTDIR "/.apa.png");

    free_whole_tree(tree);
    after();
}

static void test_getChildInDirectory_DirectoryIsEmpty() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// test-dir (7 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// ├─ epa.png
// ├─┬dir_one (3 children)
// │ ├─ .three.png
// │ ├─ two.jpg
// │ └─┬.secrets (1 children)
// │   └─ img.jpg
// └─┬dir_two (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four (1 children)
//   │ └──subsub (0 children)
//   ├─┬sub_dir_one (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three (0 children)
//   └─┬sub_dir_two (3 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    GNode *child = get_root_node(tree);
    child = g_node_last_child(child);
    child = g_node_first_child(child);
    child = g_node_next_sibling(child);
    child = g_node_next_sibling(child);
    child = g_node_next_sibling(child);
    child = g_node_first_child(child);
    assert_child_is_equal("Get child in directory ─ Last child is subsub", child, TESTDIR "/dir_two/sub_dir_four/subsub");

    child = get_child_in_directory(child, TESTDIR "/dir_two/sub_dir_four/subsub/apa.jpg");

    assert_tree_is_null("Get child in directory ─ Directory is empty", child);

    free_whole_tree(tree);
    after();
}

static void test_getChildInDirectory_FindRoot() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// test-dir (7 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// ├─ epa.png
// ├─┬dir_one (3 children)
// │ ├─ .three.png
// │ ├─ two.jpg
// │ └─┬.secrets (1 children)
// │   └─ img.jpg
// └─┬dir_two (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four (1 children)
//   │ └──subsub (0 children)
//   ├─┬sub_dir_one (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three (0 children)
//   └─┬sub_dir_two (3 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    GNode *child = get_child_in_directory(tree, TESTDIR);

    assert_child_is_equal("Get child in directory ─ Find root", child, TESTDIR);

    free_whole_tree(tree);
    after();
}

static void test_getChildInDirectory_DirectoryIsNull() {
    before();

    GNode *child = get_child_in_directory(NULL, TESTDIR "/dir_two/sub_dir_four/subsub/apa.jpg");

    assert_tree_is_null("Get child in directory ─ Directory is empty", child);

    after();
}



void test_tree_getchildindir() {
    test_getChildInDirectory_FindFromRoot_FileExists();
    test_getChildInDirectory_FindFromFileAndDir_FileExists();
    test_getChildInDirectory_FindAbove_FileExists();
    test_getChildInDirectory_FileDoesNotExist();
    test_getChildInDirectory_SearchFromSubDirectoryIsEmpty();
    test_getChildInDirectory_DirectoryIsEmpty();
    test_getChildInDirectory_FindRoot();
    test_getChildInDirectory_DirectoryIsNull();
}
