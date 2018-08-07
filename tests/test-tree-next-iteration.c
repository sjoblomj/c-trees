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

#include "test-tree-next-iteration.h"
#include "utils.h"


static void test_getNextInTree_RootIn() {
    before();

    GNode *tree = single_file(TRUE, FALSE);
    tree = get_root_node(tree);
    assert_forward_iteration(tree, ".apa.png");
    assert_equals("Get First ─ Root in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Root in",   "epa.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    free_whole_tree(tree);

    after();
}

static void test_getNextInTree_FolderIn() {
    before();

    GNode *tree = single_file(TRUE, TRUE);
    tree = get_root_node(tree);

    tree = assert_forward_iteration(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    tree = assert_forward_iteration(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    tree = assert_forward_iteration(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    tree = assert_forward_iteration(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    tree = assert_forward_iteration(tree, "epa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = g_node_next_sibling(tree);
    VnrFile* vnrfile = tree->data;
    assert_equals("First directory should be dir_one", "dir_one", vnrfile->display_name);

    assert_forward_iteration(tree, ".three.png");

    free_whole_tree(tree);

    after();
}


static void test_getNextInTree_Iterate() {
    before();

    GNode *tree = single_file(TRUE, TRUE);

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

    tree = get_root_node(tree);
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "epa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, ".three.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "two.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "cepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    // Loop from the beginning.
    tree = assert_forward_iteration(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    free_whole_tree(tree);

    after();
}

static void test_getPrevInTree_Iterate() {
    before();

    GNode *tree = single_file(TRUE, TRUE);

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

    tree = get_root_node(tree);
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "cepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "two.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, ".three.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "epa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    // Loop from the end.
    tree = assert_backward_iteration(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));


    free_whole_tree(tree);

    after();
}

static void test_getNextInTree_UriList_Iterate() {
    before();

    GNode *tree = uri_list(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// <ROOT> (5 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
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
//   └─┬sub_dir_two (4 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    tree = get_root_node(tree);
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "cepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    // Loop from the beginning.
    tree = assert_forward_iteration(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_forward_iteration(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));


    free_whole_tree(tree);

    after();
}

static void test_getPrevInTree_UriList_Iterate() {
    before();

    GNode *tree = uri_list(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// <ROOT> (5 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
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
//   └─┬sub_dir_two (4 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    tree = get_root_node(tree);
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "cepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    // Loop from the end.
    tree = assert_backward_iteration(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = assert_backward_iteration(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));


    free_whole_tree(tree);

    after();
}



void test_tree_next_iteration() {
    test_getNextInTree_RootIn();
    test_getNextInTree_FolderIn();
    test_getNextInTree_Iterate();
    test_getPrevInTree_Iterate();
    test_getNextInTree_UriList_Iterate();
    test_getPrevInTree_UriList_Iterate();
}
