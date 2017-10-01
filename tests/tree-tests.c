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
#include "test-utils.h"



void test_singleFile_NonExistantFile() {
    before();

    char *path = "non_existant_file.jpg";
    GError *error = NULL;

    assert_tree_is_null("Non existant file ─ Include hidden files: F ─ Recursive: F", vnr_file_load_single_uri(path, FALSE, FALSE, &error));
    assert_error_is_not_null(error);
    g_clear_error(&error);
    assert_tree_is_null("Non existant file ─ Include hidden files: F ─ Recursive: T", vnr_file_load_single_uri(path, FALSE, TRUE,  &error));
    assert_error_is_not_null(error);
    g_clear_error(&error);
    assert_tree_is_null("Non existant file ─ Include hidden files: T ─ Recursive: F", vnr_file_load_single_uri(path, TRUE,  FALSE, &error));
    assert_error_is_not_null(error);
    g_clear_error(&error);
    assert_tree_is_null("Non existant file ─ Include hidden files: T ─ Recursive: T", vnr_file_load_single_uri(path, TRUE,  TRUE,  &error));
    assert_error_is_not_null(error);
    g_error_free(error);

    after();
}

void test_singleFile_DontIncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("Single file ─ Include hidden files: F ─ Recursive: F", expected, get_printed_tree(SINGLE_FILE, FALSE, FALSE));

    after();
}

void test_singleFile_DontIncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (1 children)" RESET "\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two  (7 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
  │ └──" KWHT "subsub  (0 children)" RESET "\n\
  ├─┬" KWHT "sub_dir_one  (3 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    assert_equals("Single file ─ Include hidden files: F ─ Recursive: T", expected, get_printed_tree(SINGLE_FILE, FALSE, TRUE));

    after();
}

void test_singleFile_IncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("Single file ─ Include hidden files: T ─ Recursive: F", expected, get_printed_tree(SINGLE_FILE, TRUE, FALSE));

    after();
}

void test_singleFile_IncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (7 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (3 children)" RESET "\n\
│ ├─ .three.png\n\
│ ├─ two.jpg\n\
│ └─┬" KWHT ".secrets  (1 children)" RESET "\n\
│   └─ img.jpg\n\
└─┬" KWHT "dir_two  (7 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
  │ └──" KWHT "subsub  (0 children)" RESET "\n\
  ├─┬" KWHT "sub_dir_one  (3 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    assert_equals("Single file ─ Include hidden files: T ─ Recursive: T", expected, get_printed_tree(SINGLE_FILE, TRUE, TRUE));

    after();
}



void test_singleFolder_DontIncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("Single folder ─ Include hidden files: F ─ Recursive: F", expected, get_printed_tree(SINGLE_FOLDER, FALSE, FALSE));

    after();
}

void test_singleFolder_DontIncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (1 children)" RESET "\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two  (7 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
  │ └──" KWHT "subsub  (0 children)" RESET "\n\
  ├─┬" KWHT "sub_dir_one  (3 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    assert_equals("Single folder ─ Include hidden files: F ─ Recursive: T", expected, get_printed_tree(SINGLE_FOLDER, FALSE, TRUE));

    after();
}

void test_singleFolder_IncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("Single folder ─ Include hidden files: T ─ Recursive: F", expected, get_printed_tree(SINGLE_FOLDER, TRUE, FALSE));

    after();
}

void test_singleFolder_IncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (7 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (3 children)" RESET "\n\
│ ├─ .three.png\n\
│ ├─ two.jpg\n\
│ └─┬" KWHT ".secrets  (1 children)" RESET "\n\
│   └─ img.jpg\n\
└─┬" KWHT "dir_two  (7 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
  │ └──" KWHT "subsub  (0 children)" RESET "\n\
  ├─┬" KWHT "sub_dir_one  (3 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    assert_equals("Single folder ─ Include hidden files: T ─ Recursive: T", expected, get_printed_tree(SINGLE_FOLDER, TRUE, TRUE));

    after();
}



void test_uriList_DontIncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("Uri List ─ Include hidden files: F ─ Recursive: F", expected, get_printed_tree(VALID_LIST, FALSE, FALSE));
    reset_output();
    assert_equals("Uri List ─ Some invalid files. Include hidden files: F ─ Recursive: F", expected, get_printed_tree(SEMI_INVALID_LIST, FALSE, FALSE));
    reset_output();
    assert_equals("Uri List ─ Only invalid files. Include hidden files: F ─ Recursive: F", KWHT "<ROOT>  (0 children)" RESET "\n", get_printed_tree(COMPLETELY_INVALID_LIST, FALSE, FALSE));

    after();
}

void test_uriList_DontIncludeHidden_Recursive() {
    before();

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (7 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
  │ └──" KWHT "subsub  (0 children)" RESET "\n\
  ├─┬" KWHT "sub_dir_one  (3 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    assert_equals("Uri List ─ Include hidden files: F ─ Recursive: T", expected, get_printed_tree(VALID_LIST, FALSE, TRUE));
    reset_output();
    assert_equals("Uri List ─ Some invalid files. Include hidden files: F ─ Recursive: T", expected, get_printed_tree(SEMI_INVALID_LIST, FALSE, TRUE));
    reset_output();
    assert_equals("Uri List ─ Only invalid files. Include hidden files: F ─ Recursive: T", KWHT "<ROOT>  (0 children)" RESET "\n", get_printed_tree(COMPLETELY_INVALID_LIST, FALSE, TRUE));

    after();
}

void test_uriList_IncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT "<ROOT>  (5 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("Uri List ─ Include hidden files: T ─ Recursive: F", expected, get_printed_tree(VALID_LIST, TRUE, FALSE));
    reset_output();
    assert_equals("Uri List ─ Some invalid files. Include hidden files: T ─ Recursive: F", expected, get_printed_tree(SEMI_INVALID_LIST, TRUE, FALSE));
    reset_output();
    assert_equals("Uri List ─ Only invalid files. Include hidden files: T ─ Recursive: F", KWHT "<ROOT>  (0 children)" RESET "\n", get_printed_tree(COMPLETELY_INVALID_LIST, TRUE, FALSE));

    after();
}

void test_uriList_IncludeHidden_Recursive() {
    before();

    char* expected = KWHT "<ROOT>  (5 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (7 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
  │ └──" KWHT "subsub  (0 children)" RESET "\n\
  ├─┬" KWHT "sub_dir_one  (3 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    assert_equals("Uri List ─ Include hidden files: T ─ Recursive: T", expected, get_printed_tree(VALID_LIST, TRUE, TRUE));
    reset_output();
    assert_equals("Uri List ─ Some invalid files. Include hidden files: T ─ Recursive: T", expected, get_printed_tree(SEMI_INVALID_LIST, TRUE, TRUE));
    reset_output();
    assert_equals("Uri List ─ Only invalid files. Include hidden files: T ─ Recursive: T", KWHT "<ROOT>  (0 children)" RESET "\n", get_printed_tree(COMPLETELY_INVALID_LIST, TRUE, TRUE));

    after();
}



void test_getNextInTree_NullIn() {
    before();
    assert_tree_is_null("Get Next ─ Input is NULL", get_next_in_tree(NULL));
    after();
}

void test_getNextInTree_RootWithoutElements() {
    before();

    GNode *tree = g_node_new(NULL);
    assert_trees_equal("Get Next ─ Input is Root without elements", tree, get_next_in_tree(tree));
    free_tree(tree);

    after();
}

void test_getNextInTree_RootWithOnlyDir() {
    before();

    VnrFile* vnrfile = vnr_file_create_new(g_strdup("filepath"), g_strdup("display_name"), TRUE);
    GNode *tree = g_node_new(vnrfile);
    assert_trees_equal("Get Next ─ Input is Root with only one dir", tree, get_next_in_tree(tree));
    free_tree(tree);

    after();
}

void test_getNextInTree_SingleFolder_RootWithOnlyThreeDirs() {
    // No before!

    create_dir(TESTDIR);
    create_dir(TESTDIR "/apa");
    create_dir(TESTDIR "/bepa");
    create_dir(TESTDIR "/cepa");

    GNode *tree = single_folder(TRUE, TRUE);

    assert_trees_equal("Get Next ─ Single folder ─ Input is Root with only three dirs", tree, get_next_in_tree(tree));
    free_tree(tree);

    after();
}

void test_getNextInTree_UriList_RootWithOnlyThreeDirs() {
    // No before!

    create_dir(TESTDIR);
    create_dir(TESTDIR "/apa");
    create_dir(TESTDIR "/bepa");
    create_dir(TESTDIR "/cepa");

    GSList *uri_list = NULL;
    uri_list = g_slist_prepend(uri_list, TESTDIR "/bepa.png");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/cepa.jpg");

    GError *error = NULL;
    GNode *tree = vnr_file_load_uri_list(uri_list, TRUE, TRUE, &error);
    assert_error_is_null(error);

    assert_trees_equal("Get Next ─ Uri List ─ Input is Root with only three dirs", tree, get_next_in_tree(tree));
    free_tree(tree);

    after();
}

GNode* iteration_test(char* description_base, GNode* node, char* expected_file_name) {
    VnrFile* vnrfile = node->data;

    char* description = create_string(description_base, expected_file_name);
    assert_equals(description, expected_file_name, vnrfile->display_name);

    free(description);
    return node;
}

GNode* iteration_test_forward(GNode* node, char* expected_file_name) {
    return iteration_test("Get Next ─ Iterating ─ ", get_next_in_tree(node), expected_file_name);
}

GNode* iteration_test_backward(GNode* node, char* expected_file_name) {
    return iteration_test("Get Prev ─ Iterating ─ ", get_prev_in_tree(node), expected_file_name);
}

void test_getNextInTree_RootIn() {
    before();

    GNode *tree = single_file(TRUE, TRUE);
    iteration_test_forward(tree, ".apa.png");
    free_tree(tree);

    after();
}

void test_getNextInTree_FolderIn() {
    before();

    GNode *tree = single_file(TRUE, TRUE);
    GNode *node = tree;

    node = iteration_test_forward(node, ".apa.png");
    node = iteration_test_forward(node, ".depa.gif");
    node = iteration_test_forward(node, "bepa.png");
    node = iteration_test_forward(node, "cepa.jpg");
    node = iteration_test_forward(node, "epa.png");

    node = g_node_next_sibling(node);
    VnrFile* vnrfile = node->data;
    assert_equals("First directory should be dir_one", "dir_one", vnrfile->display_name);

    iteration_test_forward(node, ".three.png");

    free_tree(tree);

    after();
}


void test_getNextInTree_Iterate() {
    before();

    GNode *tree = single_file(TRUE, TRUE);
    GNode *node = tree;

// THIS IS THE STRUCTURE:
//
// tests  (7 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// ├─ epa.png
// ├─┬dir_one  (3 children)
// │ ├─ .three.png
// │ ├─ two.jpg
// │ └─┬.secrets  (1 children)
// │   └─ img.jpg
// └─┬dir_two  (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four  (1 children)
//   │ └──subsub  (0 children)
//   ├─┬sub_dir_one  (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three  (0 children)
//   └─┬sub_dir_two  (3 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    node = iteration_test_forward(node, ".apa.png");
    node = iteration_test_forward(node, ".depa.gif");
    node = iteration_test_forward(node, "bepa.png");
    node = iteration_test_forward(node, "cepa.jpg");
    node = iteration_test_forward(node, "epa.png");
    node = iteration_test_forward(node, ".three.png");
    node = iteration_test_forward(node, "two.jpg");
    node = iteration_test_forward(node, "img.jpg");
    node = iteration_test_forward(node, "apa.png");
    node = iteration_test_forward(node, "bepa.png");
    node = iteration_test_forward(node, "cepa.png");
    node = iteration_test_forward(node, "img0.png");
    node = iteration_test_forward(node, "img1.png");
    node = iteration_test_forward(node, "img2.png");
    node = iteration_test_forward(node, "img0.png");
    node = iteration_test_forward(node, "img1.png");
    node = iteration_test_forward(node, "img2.png");
    node = iteration_test_forward(node, "img3.png");
    // Loop from the beginning.
    node = iteration_test_forward(node, ".apa.png");
    node = iteration_test_forward(node, ".depa.gif");

    free_tree(tree);

    after();
}

void test_getPrevInTree_Iterate() {
    before();

    GNode *tree = single_file(TRUE, TRUE);
    GNode *node = tree;

// THIS IS THE STRUCTURE:
//
// tests  (7 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// ├─ epa.png
// ├─┬dir_one  (3 children)
// │ ├─ .three.png
// │ ├─ two.jpg
// │ └─┬.secrets  (1 children)
// │   └─ img.jpg
// └─┬dir_two  (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four  (1 children)
//   │ └──subsub  (0 children)
//   ├─┬sub_dir_one  (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three  (0 children)
//   └─┬sub_dir_two  (3 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    node = iteration_test_backward(node, "img3.png");
    node = iteration_test_backward(node, "img2.png");
    node = iteration_test_backward(node, "img1.png");
    node = iteration_test_backward(node, "img0.png");
    node = iteration_test_backward(node, "img2.png");
    node = iteration_test_backward(node, "img1.png");
    node = iteration_test_backward(node, "img0.png");
    node = iteration_test_backward(node, "cepa.png");
    node = iteration_test_backward(node, "bepa.png");
    node = iteration_test_backward(node, "apa.png");
    node = iteration_test_backward(node, "img.jpg");
    node = iteration_test_backward(node, "two.jpg");
    node = iteration_test_backward(node, ".three.png");
    node = iteration_test_backward(node, "epa.png");
    node = iteration_test_backward(node, "cepa.jpg");
    node = iteration_test_backward(node, "bepa.png");
    node = iteration_test_backward(node, ".depa.gif");
    node = iteration_test_backward(node, ".apa.png");
    // Loop from the end.
    node = iteration_test_backward(node, "img3.png");
    node = iteration_test_backward(node, "img2.png");

    free_tree(tree);

    after();
}

void test_getNextInTree_UriList_Iterate() {
    before();

    GNode *tree = uri_list(TRUE, TRUE);
    GNode *node = tree;

// THIS IS THE STRUCTURE:
//
// <ROOT>  (5 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// └─┬dir_two  (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four  (1 children)
//   │ └──subsub  (0 children)
//   ├─┬sub_dir_one  (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three  (0 children)
//   └─┬sub_dir_two  (4 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    node = iteration_test_forward(node, ".apa.png");
    node = iteration_test_forward(node, ".depa.gif");
    node = iteration_test_forward(node, "bepa.png");
    node = iteration_test_forward(node, "cepa.jpg");
    node = iteration_test_forward(node, "apa.png");
    node = iteration_test_forward(node, "bepa.png");
    node = iteration_test_forward(node, "cepa.png");
    node = iteration_test_forward(node, "img0.png");
    node = iteration_test_forward(node, "img1.png");
    node = iteration_test_forward(node, "img2.png");
    node = iteration_test_forward(node, "img0.png");
    node = iteration_test_forward(node, "img1.png");
    node = iteration_test_forward(node, "img2.png");
    node = iteration_test_forward(node, "img3.png");
    // Loop from the beginning.
    node = iteration_test_forward(node, ".apa.png");
    node = iteration_test_forward(node, ".depa.gif");

    free_tree(tree);

    after();
}

void test_getPrevInTree_UriList_Iterate() {
    before();

    GNode *tree = uri_list(TRUE, TRUE);
    GNode *node = tree;

// THIS IS THE STRUCTURE:
//
// <ROOT>  (5 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// └─┬dir_two  (7 children)
//   ├─ apa.png
//   ├─ bepa.png
//   ├─ cepa.png
//   ├─┬sub_dir_four  (1 children)
//   │ └──subsub  (0 children)
//   ├─┬sub_dir_one  (3 children)
//   │ ├─ img0.png
//   │ ├─ img1.png
//   │ └─ img2.png
//   ├──sub_dir_three  (0 children)
//   └─┬sub_dir_two  (4 children)
//     ├─ img0.png
//     ├─ img1.png
//     ├─ img2.png
//     └─ img3.png

    node = iteration_test_backward(node, "img3.png");
    node = iteration_test_backward(node, "img2.png");
    node = iteration_test_backward(node, "img1.png");
    node = iteration_test_backward(node, "img0.png");
    node = iteration_test_backward(node, "img2.png");
    node = iteration_test_backward(node, "img1.png");
    node = iteration_test_backward(node, "img0.png");
    node = iteration_test_backward(node, "cepa.png");
    node = iteration_test_backward(node, "bepa.png");
    node = iteration_test_backward(node, "apa.png");
    node = iteration_test_backward(node, "cepa.jpg");
    node = iteration_test_backward(node, "bepa.png");
    node = iteration_test_backward(node, ".depa.gif");
    node = iteration_test_backward(node, ".apa.png");
    // Loop from the end.
    node = iteration_test_backward(node, "img3.png");
    node = iteration_test_backward(node, "img2.png");

    free_tree(tree);

    after();
}

void assertChildIsEqual(GNode* tree, char* description, char* expected) {
    VnrFile *vnrfile = tree != NULL ? tree->data : NULL;
    assert_equals(description, expected, vnrfile != NULL ? vnrfile->path : "NULL");
}

void test_getChildInDirectory_FileExists() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);
    GNode *child = g_node_last_child(tree);
    assertChildIsEqual(child, "Get child in directory ─ Last child is dir_two", TESTDIR "/dir_two");

    child = get_child_in_directory(child, TESTDIR "/dir_two/bepa.png");

    assertChildIsEqual(child, "Get child in directory ─ File exists", TESTDIR "/dir_two/bepa.png");

    free_tree(tree);
    after();
}

void test_getChildInDirectory_FileDoesNotExist() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);
    GNode *child = g_node_last_child(tree);
    assertChildIsEqual(child, "Get child in directory ─ Last child is dir_two", TESTDIR "/dir_two");

    child = get_child_in_directory(child, TESTDIR "/dir_two/depa.png");

    assert_tree_is_null("Get child in directory ─ File does not exist", child);

    free_tree(tree);
    after();
}

void test_getChildInDirectory_DirectoryIsEmpty() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);
    GNode *child = tree;
    child = g_node_last_child(child);
    child = g_node_first_child(child);
    child = g_node_next_sibling(child);
    child = g_node_next_sibling(child);
    child = g_node_next_sibling(child);
    child = g_node_first_child(child);
    assertChildIsEqual(child, "Get child in directory ─ Last child is subsub", TESTDIR "/dir_two/sub_dir_four/subsub");

    child = get_child_in_directory(child, TESTDIR "/dir_two/sub_dir_four/subsub/apa.jpg");

    assert_tree_is_null("Get child in directory ─ Directory is empty", child);

    free_tree(tree);
    after();
}

void test_getChildInDirectory_DirectoryIsNull() {
    before();

    GNode *child = get_child_in_directory(NULL, TESTDIR "/dir_two/sub_dir_four/subsub/apa.jpg");

    assert_tree_is_null("Get child in directory ─ Directory is empty", child);

    after();
}




void tree_tests() {

    test_singleFile_NonExistantFile();
    test_singleFile_DontIncludeHidden_NotRecursive();
    test_singleFile_DontIncludeHidden_Recursive();
    test_singleFile_IncludeHidden_NotRecursive();
    test_singleFile_IncludeHidden_Recursive();

    test_singleFolder_DontIncludeHidden_NotRecursive();
    test_singleFolder_DontIncludeHidden_Recursive();
    test_singleFolder_IncludeHidden_NotRecursive();
    test_singleFolder_IncludeHidden_Recursive();

    test_uriList_DontIncludeHidden_NotRecursive();
    test_uriList_DontIncludeHidden_Recursive();
    test_uriList_IncludeHidden_NotRecursive();
    test_uriList_IncludeHidden_Recursive();

    test_getNextInTree_NullIn();
    test_getNextInTree_RootWithoutElements();
    test_getNextInTree_RootWithOnlyDir();

    test_getNextInTree_SingleFolder_RootWithOnlyThreeDirs();
    test_getNextInTree_UriList_RootWithOnlyThreeDirs();

    test_getNextInTree_RootIn();
    test_getNextInTree_FolderIn();
    test_getNextInTree_Iterate();
    test_getPrevInTree_Iterate();
    test_getNextInTree_UriList_Iterate();
    test_getPrevInTree_UriList_Iterate();

    test_getChildInDirectory_FileExists();
    test_getChildInDirectory_FileDoesNotExist();
    test_getChildInDirectory_DirectoryIsEmpty();
    test_getChildInDirectory_DirectoryIsNull();
}
