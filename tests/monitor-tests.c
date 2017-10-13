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



static void test_filemonitor_create_file_in_folder_nonrecursive() {
    before();

    GNode* tree = single_folder(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before create in root ─ Include hidden files: F ─ Recursive: F", expected, output);


    create_file(TESTDIR "/fepa.jpg");

    char* expected_after = KWHT TESTDIRNAME "  (4 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
└─ fepa.jpg\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after create in root ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_create_file_in_folder_recursive() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before create in subdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    create_file(TESTDIR "/dir_two/sub_dir_one/img3.png");

    char* expected_after = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
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
  ├─┬" KWHT "sub_dir_one  (4 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ ├─ img2.png\n\
  │ └─ img3.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after create in subdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_create_hidden_file_in_folder_nonrecursive() {
    before();

    GNode* tree = single_folder(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before create hidden in root ─ Include hidden files: F ─ Recursive: F", expected, output);


    create_file(TESTDIR "/.epa.jpg");

    char* expected_after = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";


    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after create hidden in root ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_create_multiple_files_in_folder_recursive() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before create multiple files in subdirs ─ Include hidden files: F ─ Recursive: T", expected, output);


    create_file(TESTDIR "/dir_two/sub_dir_one/img3.png");
    create_file(TESTDIR "/dir_two/depa.png");
    create_file(TESTDIR "/dir_two/sub_dir_four/subsub/img.png");
    create_file(TESTDIR "/apa.png");

    char* expected_after = KWHT TESTDIRNAME "  (6 children)" RESET "\n\
├─ apa.png\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (1 children)" RESET "\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two  (8 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─ depa.png\n\
  ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
  │ └─┬" KWHT "subsub  (1 children)" RESET "\n\
  │   └─ img.png\n\
  ├─┬" KWHT "sub_dir_one  (4 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ ├─ img2.png\n\
  │ └─ img3.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after create multiple files in subdirs ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_create_file_in_folder_sorted() {
    before();

    GNode* tree = single_folder(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before create file in folder sorted ─ Include hidden files: F ─ Recursive: F", expected, output);


    create_file(TESTDIR "/bepb.png");

    char* expected_after = KWHT TESTDIRNAME "  (4 children)" RESET "\n\
├─ bepa.png\n\
├─ bepb.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";


    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after create file in folder sorted ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_create_folder_in_folder_sorted() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before create folder in folder sorted ─ Include hidden files: F ─ Recursive: T", expected, output);


    create_dir (TESTDIR "/dir_onf");

    char* expected_after = KWHT TESTDIRNAME "  (6 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (1 children)" RESET "\n\
│ └─ two.jpg\n\
├──" KWHT "dir_onf  (0 children)" RESET "\n\
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

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after create folder in folder sorted ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}


static void test_filemonitor_recursive_create_dir_then_create_files_in_it() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before create dir with files ─ Include hidden files: F ─ Recursive: T", expected, output);


    create_dir (TESTDIR "/dir_w00t");
    create_file(TESTDIR "/dir_w00t/apa.png");
    create_file(TESTDIR "/dir_w00t/bepa.png");

    char* expected_after = KWHT TESTDIRNAME "  (6 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (1 children)" RESET "\n\
│ └─ two.jpg\n\
├─┬" KWHT "dir_two  (7 children)" RESET "\n\
│ ├─ apa.png\n\
│ ├─ bepa.png\n\
│ ├─ cepa.png\n\
│ ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
│ │ └──" KWHT "subsub  (0 children)" RESET "\n\
│ ├─┬" KWHT "sub_dir_one  (3 children)" RESET "\n\
│ │ ├─ img0.png\n\
│ │ ├─ img1.png\n\
│ │ └─ img2.png\n\
│ ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
│ └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
│   ├─ img0.png\n\
│   ├─ img1.png\n\
│   ├─ img2.png\n\
│   └─ img3.png\n\
└─┬" KWHT "dir_w00t  (2 children)" RESET "\n\
  ├─ apa.png\n\
  └─ bepa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after create dir with files ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}


static void test_filemonitor_nonrecursive_create_dir_then_create_files_in_it() {
    before();

    GNode* tree = single_folder(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before create dir with files ─ Include hidden files: F ─ Recursive: F", expected, output);


    create_dir (TESTDIR "/dir_three");
    create_file(TESTDIR "/dir_three/apa.png");
    create_file(TESTDIR "/dir_three/bepa.png");

    char* expected_after = KWHT TESTDIRNAME "  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after create dir with files ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}




/////////////////////

static void test_filemonitor_uriList_create_file_in_root() {
    before();

    GNode* tree = uri_list(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("File monitor before urilist create file in root ─ Include hidden files: F ─ Recursive: F", expected, output);


    create_file(TESTDIR "/apa.png");

    char* expected_after = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist create file in root ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}


static void test_filemonitor_uriList_create_dir_in_root() {
    before();

    GNode* tree = uri_list(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("File monitor before urilist create dir in root ─ Include hidden files: F ─ Recursive: F", expected, output);



    create_dir (TESTDIR "/dir_three");
    create_file(TESTDIR "/dir_three/apa.png");
    create_file(TESTDIR "/dir_three/bepa.png");

    char* expected_after = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist create dir in root ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_uriList_create_file_in_watched_dir() {
    before();

    GNode* tree = uri_list(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("File monitor before urilist create file in watched dir ─ Include hidden files: F ─ Recursive: F", expected, output);



    create_file(TESTDIR "/dir_two/depa.png");

    char* expected_after = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (4 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  └─ depa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist create file in watched dir ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_uriList_create_file_in_nonwatched_dir() {
    before();

    GNode* tree = uri_list(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("File monitor before urilist create file in nonwatched dir ─ Include hidden files: F ─ Recursive: F", expected, output);



    create_file(TESTDIR "/dir_one/depa.png");

    char* expected_after = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist create file in nonwatched dir ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_uriList_create_file_in_empty_root() {
    before();

    GNode* tree = uri_list_with_no_entries(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (0 children)" RESET "\n\
";

    assert_equals("File monitor before urilist create file in empty dir ─ Include hidden files: F ─ Recursive: F", expected, output);



    create_file(TESTDIR "/depa.png");

    char* expected_after = KWHT "<ROOT>  (0 children)" RESET "\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist create file in empty dir ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}






static void test_filemonitor_delete_file_in_folder_root() {
    before();

    GNode* tree = single_folder(TRUE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before delete in dirroot ─ Include hidden files: T ─ Recursive: F", expected, output);


    unlink(TESTDIR "/bepa.png");

    char* expected_after = KWHT TESTDIRNAME "  (4 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after delete in dirroot ─ Include hidden files: T ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_delete_hidden_file_in_folder_root() {
    before();

    GNode* tree = single_folder(TRUE, FALSE);
tree = get_root_node(tree); // TODO: Tempfix, fix this properly!
    pretty_print_tree(tree, output);

    char* expected = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before delete hidden ─ Include hidden files: T ─ Recursive: F", expected, output);


    unlink(TESTDIR "/.apa.png");

    char* expected_after = KWHT TESTDIRNAME "  (4 children)" RESET "\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after delete hidden ─ Include hidden files: T ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_delete_file_in_sub_folder() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before delete in subdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    unlink(TESTDIR "/dir_two/sub_dir_one/img1.png");

    char* expected_after = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
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
  ├─┬" KWHT "sub_dir_one  (2 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after delete in subdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_delete_multiple_files() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before delete in subdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    unlink(TESTDIR "/dir_two/sub_dir_one/img1.png");
    unlink(TESTDIR "/dir_two/bepa.png");
    unlink(TESTDIR "/dir_two/sub_dir_two/img2.png");

    char* expected_after = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (1 children)" RESET "\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two  (6 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
  │ └──" KWHT "subsub  (0 children)" RESET "\n\
  ├─┬" KWHT "sub_dir_one  (2 children)" RESET "\n\
  │ ├─ img0.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (3 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    └─ img3.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after delete in subdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_delete_subsub_folder() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before delete subsubdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    remove_directory(TESTDIR "/dir_two/sub_dir_one/");

    char* expected_after = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (1 children)" RESET "\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two  (6 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four  (1 children)" RESET "\n\
  │ └──" KWHT "subsub  (0 children)" RESET "\n\
  ├──" KWHT "sub_dir_three  (0 children)" RESET "\n\
  └─┬" KWHT "sub_dir_two  (4 children)" RESET "\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);


    assert_equals("File monitor after delete subsubdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_delete_sub_folder() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before delete subdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    remove_directory(TESTDIR "/dir_two/");

    char* expected_after = KWHT TESTDIRNAME "  (4 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
└─┬" KWHT "dir_one  (1 children)" RESET "\n\
  └─ two.jpg\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);


    assert_equals("File monitor after delete subdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_delete_root_folder() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
tree = get_root_node(tree); // TODO: Tempfix, fix this properly!
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before delete root ─ Include hidden files: F ─ Recursive: T", expected, output);


    remove_directory(TESTDIR);

//char* expected_after = KWHT TESTDIRNAME "  (0 children)" RESET "\n\ // TODO: Tempfix, fix this properly!
    char* expected_after = KWHT "(null)  (0 children)" RESET "\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);


    assert_equals("File monitor after delete root ─ Include hidden files: F ─ Recursive: T", expected_after, output);

//    free_whole_tree(tree); //TODO: Tempfix, fix this properly!
    after();
}



static void test_filemonitor_uriList_delete_file_under_root() {
    before();

    GNode* tree = uri_list(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("File monitor before urilist delete file in root ─ Include hidden files: F ─ Recursive: F", expected, output);



    unlink(TESTDIR "/bepa.png");

    char* expected_after = KWHT "<ROOT>  (2 children)" RESET "\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

tree = get_root_node(tree); // TODO: Tempfix, fix this properly!
    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist delete file in root ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_uriList_delete_nonwatched_file_under_root() {
    before();

    GNode* tree = uri_list(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("File monitor before urilist delete nonwatched file in root ─ Include hidden files: F ─ Recursive: F", expected, output);



    unlink(TESTDIR "/epa.png");

    char* expected_after = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist delete nonwatched file in root ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_uriList_delete_nonwatched_file_under_empty_root() {
    before();

    GNode* tree = uri_list_with_no_entries(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (0 children)" RESET "\n\
";

    assert_equals("File monitor before urilist delete file in empty root ─ Include hidden files: F ─ Recursive: F", expected, output);



    unlink(TESTDIR "/epa.png");

    char* expected_after = KWHT "<ROOT>  (0 children)" RESET "\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist delete file in empty root ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_uriList_delete_file_under_watched_dir() {
    before();

    GNode* tree = uri_list(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("File monitor before urilist delete file in watched dir ─ Include hidden files: F ─ Recursive: F", expected, output);



    unlink(TESTDIR "/dir_two/bepa.png");

    char* expected_after = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (2 children)" RESET "\n\
  ├─ apa.png\n\
  └─ cepa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist delete file in watched dir ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_uriList_delete_file_under_nonwatched_dir() {
    before();

    GNode* tree = uri_list(FALSE, FALSE);
    pretty_print_tree(tree, output);

    char* expected = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    assert_equals("File monitor before urilist delete file in nonwatched dir ─ Include hidden files: F ─ Recursive: F", expected, output);



    unlink(TESTDIR "/dir_one/two.jpg");

    char* expected_after = KWHT "<ROOT>  (3 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─┬" KWHT "dir_two  (3 children)" RESET "\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  └─ cepa.png\n\
";

    wait_until_tree_is_as_expected(tree, expected_after);

    assert_equals("File monitor after urilist delete file in nonwatched dir ─ Include hidden files: F ─ Recursive: F", expected_after, output);

    free_whole_tree(tree);
    after();
}



static void test_filemonitor_move_file_in_same_dir() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before move file in same dir ─ Include hidden files: F ─ Recursive: T", expected, output);


    rename(TESTDIR "/cepa.jpg", TESTDIR "/depa.jpg");

    char* expected_after = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ bepa.png\n\
├─ depa.jpg\n\
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


    wait_until_tree_is_as_expected(tree, expected_after);


    assert_equals("File monitor after move file in same dir ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_move_file_to_subdir() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before move file to subdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    rename(TESTDIR "/cepa.jpg", TESTDIR "/dir_one/depa.jpg");

    char* expected_after = KWHT TESTDIRNAME "  (4 children)" RESET "\n\
├─ bepa.png\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (2 children)" RESET "\n\
│ ├─ depa.jpg\n\
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


    wait_until_tree_is_as_expected(tree, expected_after);


    assert_equals("File monitor after move file to subdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_move_file_to_parent_dir() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before move file in subdir to root ─ Include hidden files: F ─ Recursive: T", expected, output);


    rename(TESTDIR "/dir_two/apa.png", TESTDIR "/apa.png");

    char* expected_after = KWHT TESTDIRNAME "  (6 children)" RESET "\n\
├─ apa.png\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one  (1 children)" RESET "\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two  (6 children)" RESET "\n\
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


    wait_until_tree_is_as_expected(tree, expected_after);


    assert_equals("File monitor after move file in subdir to root ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}

static void test_filemonitor_move_dir_in_same_dir() {
    before();

    GNode* tree = single_folder(FALSE, TRUE);
    pretty_print_tree(tree, output);

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

    assert_equals("File monitor before move dir in root ─ Include hidden files: F ─ Recursive: T", expected, output);


    rename(TESTDIR "/dir_one", TESTDIR "/dir_moved");

    char* expected_after = KWHT TESTDIRNAME "  (5 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_moved  (1 children)" RESET "\n\
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


    wait_until_tree_is_as_expected(tree, expected_after);


    assert_equals("File monitor after move dir in root ─ Include hidden files: F ─ Recursive: T", expected_after, output);

    free_whole_tree(tree);
    after();
}



gboolean file_monitor_tests(gpointer data) {

    test_filemonitor_create_file_in_folder_nonrecursive();
    test_filemonitor_create_file_in_folder_recursive();
    test_filemonitor_create_hidden_file_in_folder_nonrecursive();
    test_filemonitor_create_multiple_files_in_folder_recursive();
    test_filemonitor_create_file_in_folder_sorted();
    test_filemonitor_create_folder_in_folder_sorted();

    test_filemonitor_recursive_create_dir_then_create_files_in_it();
    test_filemonitor_nonrecursive_create_dir_then_create_files_in_it();

    test_filemonitor_uriList_create_file_in_root();
    test_filemonitor_uriList_create_dir_in_root();
    test_filemonitor_uriList_create_file_in_watched_dir();
    test_filemonitor_uriList_create_file_in_nonwatched_dir();
    test_filemonitor_uriList_create_file_in_empty_root();

    test_filemonitor_delete_file_in_folder_root();
    test_filemonitor_delete_hidden_file_in_folder_root();
    test_filemonitor_delete_file_in_sub_folder();
    test_filemonitor_delete_multiple_files();
    test_filemonitor_delete_subsub_folder();
    test_filemonitor_delete_sub_folder();
    test_filemonitor_delete_root_folder();

    test_filemonitor_uriList_delete_file_under_root();
    test_filemonitor_uriList_delete_nonwatched_file_under_root();
    test_filemonitor_uriList_delete_nonwatched_file_under_empty_root();
    test_filemonitor_uriList_delete_file_under_watched_dir();
    test_filemonitor_uriList_delete_file_under_nonwatched_dir();

    test_filemonitor_move_file_in_same_dir();
    test_filemonitor_move_file_to_subdir();
    test_filemonitor_move_file_to_parent_dir();
    test_filemonitor_move_dir_in_same_dir();


    g_main_loop_quit((GMainLoop*)data);
    return FALSE;
}
