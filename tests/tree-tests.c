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


static void test_singleFile_NonExistantFile() {
    before();

    char *path = NULL;
    GError *error = NULL;

    assert_tree_is_null("Null file ─ Include hidden files: F ─ Recursive: F", create_tree_from_single_uri(path, FALSE, FALSE, NULL, NULL, &error));
    assert_error_is_null(error);
    g_clear_error(&error);
    assert_tree_is_null("Null file ─ Include hidden files: F ─ Recursive: T", create_tree_from_single_uri(path, FALSE, TRUE,  NULL, NULL, &error));
    assert_error_is_null(error);
    g_clear_error(&error);
    assert_tree_is_null("Null file ─ Include hidden files: T ─ Recursive: F", create_tree_from_single_uri(path, TRUE,  FALSE, NULL, NULL, &error));
    assert_error_is_null(error);
    g_clear_error(&error);
    assert_tree_is_null("Null file ─ Include hidden files: T ─ Recursive: T", create_tree_from_single_uri(path, TRUE,  TRUE,  NULL, NULL, &error));
    assert_error_is_null(error);
    g_clear_error(&error);

    path = "non_existant_file.jpg";
    assert_tree_is_null("Non existant file ─ Include hidden files: F ─ Recursive: F", create_tree_from_single_uri(path, FALSE, FALSE, NULL, NULL, &error));
    assert_error_is_not_null(error);
    g_clear_error(&error);
    assert_tree_is_null("Non existant file ─ Include hidden files: F ─ Recursive: T", create_tree_from_single_uri(path, FALSE, TRUE,  NULL, NULL, &error));
    assert_error_is_not_null(error);
    g_clear_error(&error);
    assert_tree_is_null("Non existant file ─ Include hidden files: T ─ Recursive: F", create_tree_from_single_uri(path, TRUE,  FALSE, NULL, NULL, &error));
    assert_error_is_not_null(error);
    g_clear_error(&error);
    assert_tree_is_null("Non existant file ─ Include hidden files: T ─ Recursive: T", create_tree_from_single_uri(path, TRUE,  TRUE,  NULL, NULL, &error));
    assert_error_is_not_null(error);
    g_error_free(error);

    after();
}

static void test_singleFile_DontIncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    GNode *tree = get_tree(SINGLE_FILE, FALSE, FALSE);
    assert_numbers_equals("#Leaves Single file ─ Include hidden files: F ─ Recursive: F", 3, get_total_number_of_leaves(tree));

    assert_equals("Single file ─ Include hidden files: F ─ Recursive: F", expected, print_and_free_tree(tree));

    after();
}

static void test_singleFile_DontIncludeHidden_Recursive() {
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

    GNode *tree = get_tree(SINGLE_FILE, FALSE, TRUE);
    assert_numbers_equals("#Leaves Single file ─ Include hidden files: F ─ Recursive: T", 14, get_total_number_of_leaves(tree));

    assert_equals("Single file ─ Include hidden files: F ─ Recursive: T", expected, print_and_free_tree(tree));

    after();
}

static void test_singleFile_IncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    GNode *tree = get_tree(SINGLE_FILE, TRUE, FALSE);
    assert_numbers_equals("#Leaves Single file ─ Include hidden files: T ─ Recursive: F", 5, get_total_number_of_leaves(tree));

    assert_equals("Single file ─ Include hidden files: T ─ Recursive: F", expected, print_and_free_tree(tree));

    after();
}

static void test_singleFile_IncludeHidden_Recursive() {
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

    GNode *tree = get_tree(SINGLE_FILE, TRUE, TRUE);
    assert_numbers_equals("#Leaves Single file ─ Include hidden files: T ─ Recursive: T", 18, get_total_number_of_leaves(tree));

    assert_equals("Single file ─ Include hidden files: T ─ Recursive: T", expected, print_and_free_tree(tree));

    after();
}



static void test_singleFolder_DontIncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
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
    GNode *tree = get_tree(SINGLE_FILE, FALSE, TRUE);
    assert_numbers_equals("#Leaves Single folder ─ Include hidden files: F ─ Recursive: T", 14, get_total_number_of_leaves(tree));

    assert_equals("Single folder ─ Include hidden files: F ─ Recursive: T", expected, print_and_free_tree(tree));

    after();
}

static void test_singleFolder_IncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
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
    GNode *tree = get_tree(SINGLE_FILE, TRUE, TRUE);
    assert_numbers_equals("#Leaves Single folder ─ Include hidden files: T ─ Recursive: T", 18, get_total_number_of_leaves(tree));

    assert_equals("Single folder ─ Include hidden files: T ─ Recursive: T", expected, print_and_free_tree(tree));

    after();
}



static void test_uriList_DontIncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";
    GNode *tree = get_tree(VALID_LIST, FALSE, FALSE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: F ─ Recursive: F", 5, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Include hidden files: F ─ Recursive: F", expected, print_and_free_tree(tree));
    reset_output();


    expected = KWHT "<ROOT>  (4 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ img.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";
    tree = get_tree(SEMI_INVALID_LIST, FALSE, FALSE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: F ─ Recursive: F", 6, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Some invalid files. Include hidden files: F ─ Recursive: F", expected, print_and_free_tree(tree));
    reset_output();


    tree = get_tree(COMPLETELY_INVALID_LIST, FALSE, FALSE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: F ─ Recursive: F", 0, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Only invalid files. Include hidden files: F ─ Recursive: F", KWHT "<ROOT>  (0 children)" RESET "\n", print_and_free_tree(tree));

    after();
}

static void test_uriList_DontIncludeHidden_Recursive() {
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
    GNode *tree = get_tree(VALID_LIST, FALSE, TRUE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: F ─ Recursive: T", 12, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Include hidden files: F ─ Recursive: T", expected, print_and_free_tree(tree));
    reset_output();


    expected = KWHT "<ROOT>  (4 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ img.jpg\n\
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
    tree = get_tree(SEMI_INVALID_LIST, FALSE, TRUE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: F ─ Recursive: T", 13, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Some invalid files. Include hidden files: F ─ Recursive: T", expected, print_and_free_tree(tree));
    reset_output();


    tree = get_tree(COMPLETELY_INVALID_LIST, FALSE, TRUE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: F ─ Recursive: T", 0, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Only invalid files. Include hidden files: F ─ Recursive: T", KWHT "<ROOT>  (0 children)" RESET "\n", print_and_free_tree(tree));

    after();
}

static void test_uriList_IncludeHidden_NotRecursive() {
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
    GNode *tree = get_tree(VALID_LIST, TRUE, FALSE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: T ─ Recursive: F", 7, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Include hidden files: T ─ Recursive: F", expected, print_and_free_tree(tree));
    reset_output();


    expected = KWHT "<ROOT>  (6 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ img.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";
    tree = get_tree(SEMI_INVALID_LIST, TRUE, FALSE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: T ─ Recursive: F", 8, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Some invalid files. Include hidden files: T ─ Recursive: F", expected, print_and_free_tree(tree));
    reset_output();


    tree = get_tree(COMPLETELY_INVALID_LIST, TRUE, FALSE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: T ─ Recursive: F", 0, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Only invalid files. Include hidden files: T ─ Recursive: F", KWHT "<ROOT>  (0 children)" RESET "\n", print_and_free_tree(tree));

    after();
}

static void test_uriList_IncludeHidden_Recursive() {
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
    GNode *tree = get_tree(VALID_LIST, TRUE, TRUE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: T ─ Recursive: T", 14, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Include hidden files: T ─ Recursive: T", expected, print_and_free_tree(tree));
    reset_output();



    expected = KWHT "<ROOT>  (6 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ img.jpg\n\
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
    tree = get_tree(SEMI_INVALID_LIST, TRUE, TRUE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: T ─ Recursive: F", 15, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Some invalid files. Include hidden files: T ─ Recursive: T", expected, print_and_free_tree(tree));
    reset_output();

    tree = get_tree(COMPLETELY_INVALID_LIST, TRUE, TRUE);
    assert_numbers_equals("#Leaves Uri List ─ Include hidden files: T ─ Recursive: F", 0, get_total_number_of_leaves(tree));

    assert_equals("Uri List ─ Only invalid files. Include hidden files: T ─ Recursive: T", KWHT "<ROOT>  (0 children)" RESET "\n", print_and_free_tree(tree));

    after();
}



static void test_getNextInTree_NullIn() {
    before();
    assert_tree_is_null("Get Next ─ Input is NULL", get_next_in_tree(NULL));
    assert_tree_is_null("Get First ─ Input is NULL", get_first_in_tree(NULL));
    assert_tree_is_null("Get Last ─ Input is NULL", get_last_in_tree(NULL));
    after();
}

static void test_getNextInTree_RootWithoutElements() {
    before();

    GNode *tree = g_node_new(NULL);
    assert_trees_equal("Get Next ─ Input is Root without elements", tree, get_next_in_tree(tree));
    assert_trees_equal("Get First ─ Input is Root without elements", tree, get_first_in_tree(tree));
    assert_trees_equal("Get Last ─ Input is Root without elements", tree, get_last_in_tree(tree));
    free_whole_tree(tree);

    after();
}

static void test_getNextInTree_RootWithOnlyDir() {
    before();

    VnrFile* vnrfile = vnr_file_create_new("filepath", "display_name", TRUE);
    GNode *tree = g_node_new(vnrfile);
    assert_trees_equal("Get Next ─ Input is Root with only one dir", tree, get_next_in_tree(tree));
    assert_trees_equal("Get First ─ Input is Root with only one dir", tree, get_first_in_tree(tree));
    assert_trees_equal("Get Last ─ Input is Root with only one dir", tree, get_last_in_tree(tree));
    free_whole_tree(tree);

    after();
}

static void test_getNextInTree_SingleFolder_RootWithOnlyThreeDirs() {
    // No before!

    create_dir(TESTDIR);
    create_dir(TESTDIR "/apa");
    create_dir(TESTDIR "/bepa");
    create_dir(TESTDIR "/cepa");

    GNode *tree = single_folder(TRUE, TRUE);

    assert_trees_equal("Get Next ─ Single folder ─ Input is Root with only three dirs", tree, get_next_in_tree(tree));
    assert_trees_equal("Get First ─ Single folder ─ Input is Root with only three dirs", tree, get_first_in_tree(tree));
    assert_trees_equal("Get Last ─ Single folder ─ Input is Root with only three dirs", tree, get_last_in_tree(tree));
    free_whole_tree(tree);

    after();
}

static void test_getNextInTree_UriList_RootWithOnlyThreeDirs() {
    // No before!

    create_dir(TESTDIR);
    create_dir(TESTDIR "/apa");
    create_dir(TESTDIR "/bepa");
    create_dir(TESTDIR "/cepa");

    GSList *uri_list = NULL;
    uri_list = g_slist_prepend(uri_list, TESTDIR "/bepa.png");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/cepa.jpg");

    GError *error = NULL;
    GNode *tree = create_tree_from_uri_list(uri_list, TRUE, TRUE, NULL, NULL, &error);
    assert_error_is_null(error);
    free(error);
    g_slist_free(uri_list);

    assert_trees_equal("Get Next ─ Uri List ─ Input is Root with only three dirs", tree, get_next_in_tree(tree));
    assert_trees_equal("Get First ─ Uri List ─ Input is Root with only three dirs", tree, get_first_in_tree(tree));
    assert_trees_equal("Get Last ─ Uri List ─ Input is Root with only three dirs", tree, get_last_in_tree(tree));
    free_whole_tree(tree);

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

static void test_getNextInTree_RootIn() {
    before();

    GNode *tree = single_file(TRUE, FALSE);
    tree = get_root_node(tree);
    iteration_test_forward(tree, ".apa.png");
    assert_equals("Get First ─ Root in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Root in",   "epa.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    free_whole_tree(tree);

    after();
}

static void test_getNextInTree_FolderIn() {
    before();

    GNode *tree = single_file(TRUE, TRUE);
    tree = get_root_node(tree);

    tree = iteration_test_forward(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    tree = iteration_test_forward(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    tree = iteration_test_forward(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    tree = iteration_test_forward(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));
    tree = iteration_test_forward(tree, "epa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = g_node_next_sibling(tree);
    VnrFile* vnrfile = tree->data;
    assert_equals("First directory should be dir_one", "dir_one", vnrfile->display_name);

    iteration_test_forward(tree, ".three.png");

    free_whole_tree(tree);

    after();
}


static void test_getNextInTree_Iterate() {
    before();

    GNode *tree = single_file(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// c-trees-tests  (7 children)
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

    tree = get_root_node(tree);
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "epa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, ".three.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "two.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "cepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    // Loop from the beginning.
    tree = iteration_test_forward(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, ".depa.gif");
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
// c-trees-tests  (7 children)
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

    tree = get_root_node(tree);
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "cepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "two.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, ".three.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "epa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    // Loop from the end.
    tree = iteration_test_backward(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img2.png");
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

    tree = get_root_node(tree);
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "cepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    // Loop from the beginning.
    tree = iteration_test_forward(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_forward(tree, ".depa.gif");
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

    tree = get_root_node(tree);
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img1.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img0.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "cepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "cepa.jpg");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "bepa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, ".depa.gif");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, ".apa.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    // Loop from the end.
    tree = iteration_test_backward(tree, "img3.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));

    tree = iteration_test_backward(tree, "img2.png");
    assert_equals("Get First ─ Folder in", ".apa.png", (VNR_FILE(get_first_in_tree(tree)->data)->display_name));
    assert_equals("Get Last ─ Folder in",  "img3.png", (VNR_FILE(get_last_in_tree (tree)->data)->display_name));


    free_whole_tree(tree);

    after();
}

static void assert_child_is_equal(char* description, GNode* tree, char* expected) {
    VnrFile *vnrfile = tree != NULL ? tree->data : NULL;
    assert_equals(description, expected, vnrfile != NULL ? vnrfile->path : "NULL");
}

static void test_getChildInDirectory_FindFromRoot_FileExists() {
    before();
    GNode *tree = single_folder(TRUE, TRUE);

// THIS IS THE STRUCTURE:
//
// c-trees-tests  (7 children)
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
// c-trees-tests  (7 children)
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
// c-trees-tests  (7 children)
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
// c-trees-tests  (7 children)
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
// c-trees-tests  (7 children)
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
// c-trees-tests  (7 children)
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
// c-trees-tests  (7 children)
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



static void test_addNodeInTree_NullIn() {
    before();

    GNode *tree = NULL;
    add_node_in_tree(tree, NULL);
    assert_tree_is_null("Add node in tree ─ Tree is null", tree);

// THIS IS THE STRUCTURE:
//
// c-trees-tests  (3 children)
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
    char* expected = KWHT "c-trees-tests  (4 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
└─┬" KWHT "c-trees-tests  (3 children)" RESET "\n\
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
    char* expected = KWHT "c-trees-tests  (3 children)" RESET "\n\
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
    GNode *bepa = iteration_test_forward(tree, "bepa.png");

    add_node_in_tree(bepa, node);
    char* expected = KWHT "c-trees-tests  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("Add node in tree ─ Tree is leaf ─ No change", expected, print_and_free_tree(tree));
    free_whole_tree(node);
    after();
}



static void test_getNumberOfLeaves_NullIn() {
    before();

    gint position, total;
    get_leaf_position(NULL, &position, &total);

    assert_numbers_equals("#Leaves ─ Null input", -1, position);
    assert_numbers_equals("#Leaves ─ Null input", 0, total);
    assert_numbers_equals("#Leaves ─ Null input", 0, get_total_number_of_leaves(NULL));

    after();
}

static void test_getNumberOfLeaves_SingleFileNonRecursive_ReturnSameNumberNoMatterWhereInTheTreeWeAre() {
    before();

    int position, total;
    GNode *tree = single_file(TRUE, FALSE);
    tree = get_root_node(tree);
    GNode *node = tree;

// THIS IS THE STRUCTURE:
//
// c-trees-tests  (5 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
// └─ epa.png

    get_leaf_position(node, &position, &total);
    assert_numbers_equals("#Leaves ─ Tree root as input ─ position", 0, position);
    assert_numbers_equals("#Leaves ─ Tree root as input ─ total", 5, total);


    node = tree;
    assert_numbers_equals("#Leaves ─ Single file, non-recursive ─ Different places in tree", 5, get_total_number_of_leaves(node));
    node = iteration_test_forward(node, ".apa.png");
    assert_numbers_equals("#Leaves ─ Single file, non-recursive ─ Different places in tree", 5, get_total_number_of_leaves(node));
    node = iteration_test_forward(node, ".depa.gif");
    assert_numbers_equals("#Leaves ─ Single file, non-recursive ─ Different places in tree", 5, get_total_number_of_leaves(node));

    free_whole_tree(tree);
    after();
}

static void test_getNumberOfLeaves_UriListRecursive_ReturnSameNumberNoMatterWhereInTheTreeWeAre() {
    before();

    int position, total;
    GNode *tree = uri_list(TRUE, TRUE);

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

    tree = get_root_node(tree);
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 0, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, ".apa.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 1, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, ".depa.gif");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 2, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "bepa.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 3, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "cepa.jpg");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 4, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "apa.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 5, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "bepa.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 6, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "cepa.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 7, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "img0.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 8, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "img1.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 9, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "img2.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 10, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "img0.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 11, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "img1.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 12, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "img2.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 13, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, "img3.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 14, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    // Loop from the beginning.
    tree = iteration_test_forward(tree, ".apa.png");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 1, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    tree = iteration_test_forward(tree, ".depa.gif");
    get_leaf_position(tree, &position, &total);
    assert_numbers_equals("#Leaves ─ Node iteration ─ position", 2, position);
    assert_numbers_equals("#Leaves ─ Node iteration ─ total", 14, total);
    assert_numbers_equals("#Leaves ─ UriList, recursive ─ Different places in tree", 14, get_total_number_of_leaves(tree));

    free_whole_tree(tree);

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

    test_getChildInDirectory_FindFromRoot_FileExists();
    test_getChildInDirectory_FindFromFileAndDir_FileExists();
    test_getChildInDirectory_FindAbove_FileExists();
    test_getChildInDirectory_FileDoesNotExist();
    test_getChildInDirectory_SearchFromSubDirectoryIsEmpty();
    test_getChildInDirectory_DirectoryIsEmpty();
    test_getChildInDirectory_FindRoot();
    test_getChildInDirectory_DirectoryIsNull();

    test_addNodeInTree_NullIn();
    test_addNodeInTree_TreesIn();
    test_addNodeInTree_DouplicateNode();
    test_addNodeInTree_TreeIsLeaf();

    test_getNumberOfLeaves_NullIn();
    test_getNumberOfLeaves_SingleFileNonRecursive_ReturnSameNumberNoMatterWhereInTheTreeWeAre();
    test_getNumberOfLeaves_UriListRecursive_ReturnSameNumberNoMatterWhereInTheTreeWeAre();
}
