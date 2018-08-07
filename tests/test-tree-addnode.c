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

#include "test-tree-addnode.h"
#include "utils.h"


static void test_addNodeInTree_NullIn() {
    before();

    GNode *tree = NULL;
    add_node_in_tree(tree, NULL);
    assert_tree_is_null("Add node in tree ─ Tree is null", tree);

// THIS IS THE STRUCTURE:
//
// test-dir (3 children)
//├─ bepa.png
//├─ cepa.jpg
//└─ epa.png

    GNode *expected = get_tree(SINGLE_FILE, FALSE, FALSE);

    add_node_in_tree(tree, expected);
    assert_tree_is_null("Add node in tree ─ Tree is null", tree);

    tree = get_tree(SINGLE_FILE, FALSE, FALSE);
    add_node_in_tree(tree, NULL);
    assert_trees_equal("Add node in tree ─ Node is null", tree, expected);


    GNode *node_without_data = g_node_new(NULL);
    add_node_in_tree(tree, node_without_data);
    assert_trees_equal("Add node in tree ─ Node is null", tree, expected);


    free_whole_tree(tree);
    free_whole_tree(expected);
    free_whole_tree(node_without_data);
    after();
}

static void test_addNodeInTree_TreesIn() {
    before();

    GNode *tree = get_tree(SINGLE_FILE, FALSE, FALSE);
    GNode *node = get_tree(SINGLE_FILE, FALSE, FALSE);
    tree = get_root_node(tree);
    node = get_root_node(node);

    add_node_in_tree(tree, node);
    char* expected = KWHT TESTDIRNAME RESET " (4 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
└─┬" KWHT TESTDIRNAME RESET " (3 children)\n\
  ├─ bepa.png\n\
  ├─ cepa.jpg\n\
  └─ epa.png\n\
";

    assert_equals("Add node in tree ─ Tree in tree: F ─ Recursive: F", expected, print_and_free_tree(tree));

    after();
}

static void test_addNodeInTree_DouplicateNode() {
    before();

    VnrFile *vnrfile = vnr_file_create_new(TESTDIR "/cepa.jpg", "cepa.jpg", FALSE);
    GNode *node = g_node_new(vnrfile);

    GNode *tree = get_tree(SINGLE_FILE, FALSE, FALSE);
    tree = get_root_node(tree);

    add_node_in_tree(tree, node);
    char* expected = KWHT TESTDIRNAME RESET " (3 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("Add node in tree ─ Douplicate node: F ─ Recursive: F", expected, print_and_free_tree(tree));

    free_whole_tree(node);
    after();
}

static void test_addNodeInTree_TreeIsLeaf() {
    before();

    VnrFile *vnrfile = vnr_file_create_new(TESTDIR "/cepa.jpg", "cepa.jpg", FALSE);
    GNode *node = g_node_new(vnrfile);

    GNode *tree = get_tree(SINGLE_FILE, FALSE, FALSE);
    tree = get_root_node(tree);
    GNode *bepa = assert_forward_iteration(tree, "bepa.png");

    add_node_in_tree(bepa, node);
    char* expected = KWHT TESTDIRNAME RESET " (3 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("Add node in tree ─ Tree is leaf ─ No change", expected, print_and_free_tree(tree));
    free_whole_tree(node);
    after();
}



void test_tree_addnode() {
    test_addNodeInTree_NullIn();
    test_addNodeInTree_TreesIn();
    test_addNodeInTree_DouplicateNode();
    test_addNodeInTree_TreeIsLeaf();
}
