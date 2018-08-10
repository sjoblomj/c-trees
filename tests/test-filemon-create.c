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

#include "test-filemon-create.h"
#include "utils.h"


static void test_filemonitor_create_file_in_folder_nonrecursive() {
    before();

    monitor_test_tree = single_folder(FALSE, FALSE);
    pretty_print_tree(monitor_test_tree, output);

    char* expected = KWHT TESTDIRNAME RESET " (3 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before create in root ─ Include hidden files: F ─ Recursive: F", expected, output);


    create_file(testdir_path, "/fepa.jpg");

    char* expected_after = KWHT TESTDIRNAME RESET " (4 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
└─ fepa.jpg\n\
";

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after create in root ─ Include hidden files: F ─ Recursive: F", expected_after, output);
    assert_file_system_changes_at_least(1);

    after();
}

static void test_filemonitor_create_file_in_folder_recursive() {
    before();

    monitor_test_tree = single_folder(FALSE, TRUE);
    pretty_print_tree(monitor_test_tree, output);

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

    assert_equals("File monitor before create in subdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    create_file(testdir_path, "/dir_two/sub_dir_one/img3.png");

    char* expected_after = KWHT TESTDIRNAME RESET " (5 children)\n\
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
  ├─┬" KWHT "sub_dir_one" RESET " (4 children)\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ ├─ img2.png\n\
  │ └─ img3.png\n\
  ├──" KWHT "sub_dir_three" RESET " (0 children)\n\
  └─┬" KWHT "sub_dir_two" RESET " (4 children)\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after create in subdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);
    assert_file_system_changes_at_least(1);

    after();
}

static void test_filemonitor_create_hidden_file_in_folder_nonrecursive() {
    before();

    monitor_test_tree = single_folder(FALSE, FALSE);
    pretty_print_tree(monitor_test_tree, output);

    char* expected = KWHT TESTDIRNAME RESET " (3 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before create hidden in root ─ Include hidden files: F ─ Recursive: F", expected, output);


    create_file(testdir_path, "/.epa.jpg");

    char* expected_after = KWHT TESTDIRNAME RESET " (3 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";


    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after create hidden in root ─ Include hidden files: F ─ Recursive: F", expected_after, output);
    assert_file_system_changes_at_least(0);

    after();
}

static void test_filemonitor_create_multiple_files_in_folder_recursive() {
    before();

    monitor_test_tree = single_folder(FALSE, TRUE);
    pretty_print_tree(monitor_test_tree, output);

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

    assert_equals("File monitor before create multiple files in subdirs ─ Include hidden files: F ─ Recursive: T", expected, output);


    create_file(testdir_path, "/dir_two/sub_dir_one/img3.png");
    create_file(testdir_path, "/dir_two/depa.png");
    create_file(testdir_path, "/dir_two/sub_dir_four/subsub/img.png");
    create_file(testdir_path, "/apa.png");

    char* expected_after = KWHT TESTDIRNAME RESET " (6 children)\n\
├─ apa.png\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one" RESET " (1 children)\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two" RESET " (8 children)\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─ depa.png\n\
  ├─┬" KWHT "sub_dir_four" RESET " (1 children)\n\
  │ └─┬" KWHT "subsub" RESET " (1 children)\n\
  │   └─ img.png\n\
  ├─┬" KWHT "sub_dir_one" RESET " (4 children)\n\
  │ ├─ img0.png\n\
  │ ├─ img1.png\n\
  │ ├─ img2.png\n\
  │ └─ img3.png\n\
  ├──" KWHT "sub_dir_three" RESET " (0 children)\n\
  └─┬" KWHT "sub_dir_two" RESET " (4 children)\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after create multiple files in subdirs ─ Include hidden files: F ─ Recursive: T", expected_after, output);
    assert_file_system_changes_at_least(4);

    after();
}

static void test_filemonitor_create_file_in_folder_sorted() {
    before();

    monitor_test_tree = single_folder(FALSE, FALSE);
    pretty_print_tree(monitor_test_tree, output);

    char* expected = KWHT TESTDIRNAME RESET " (3 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before create file in folder sorted ─ Include hidden files: F ─ Recursive: F", expected, output);


    create_file(testdir_path, "/bepb.png");

    char* expected_after = KWHT TESTDIRNAME RESET " (4 children)\n\
├─ bepa.png\n\
├─ bepb.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";


    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after create file in folder sorted ─ Include hidden files: F ─ Recursive: F", expected_after, output);
    assert_file_system_changes_at_least(1);

    after();
}

static void test_filemonitor_create_folder_in_folder_sorted() {
    before();

    monitor_test_tree = single_folder(FALSE, TRUE);
    pretty_print_tree(monitor_test_tree, output);

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

    assert_equals("File monitor before create folder in folder sorted ─ Include hidden files: F ─ Recursive: T", expected, output);


    create_dir (testdir_path, "/dir_onf");

    char* expected_after = KWHT TESTDIRNAME RESET " (6 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one" RESET " (1 children)\n\
│ └─ two.jpg\n\
├──" KWHT "dir_onf" RESET " (0 children)\n\
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

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after create folder in folder sorted ─ Include hidden files: F ─ Recursive: T", expected_after, output);
    assert_file_system_changes_at_least(1);

    after();
}


static void test_filemonitor_recursive_create_dir_then_create_files_in_it() {
    before();

    monitor_test_tree = single_folder(FALSE, TRUE);
    pretty_print_tree(monitor_test_tree, output);

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

    assert_equals("File monitor before create dir with files ─ Include hidden files: F ─ Recursive: T", expected, output);


    create_dir (testdir_path, "/dir_w00t");
    create_file(testdir_path, "/dir_w00t/apa.png");
    create_file(testdir_path, "/dir_w00t/bepa.png");

    char* expected_after = KWHT TESTDIRNAME RESET " (6 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one" RESET " (1 children)\n\
│ └─ two.jpg\n\
├─┬" KWHT "dir_two" RESET " (7 children)\n\
│ ├─ apa.png\n\
│ ├─ bepa.png\n\
│ ├─ cepa.png\n\
│ ├─┬" KWHT "sub_dir_four" RESET " (1 children)\n\
│ │ └──" KWHT "subsub" RESET " (0 children)\n\
│ ├─┬" KWHT "sub_dir_one" RESET " (3 children)\n\
│ │ ├─ img0.png\n\
│ │ ├─ img1.png\n\
│ │ └─ img2.png\n\
│ ├──" KWHT "sub_dir_three" RESET " (0 children)\n\
│ └─┬" KWHT "sub_dir_two" RESET " (4 children)\n\
│   ├─ img0.png\n\
│   ├─ img1.png\n\
│   ├─ img2.png\n\
│   └─ img3.png\n\
└─┬" KWHT "dir_w00t" RESET " (2 children)\n\
  ├─ apa.png\n\
  └─ bepa.png\n\
";

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after create dir with files ─ Include hidden files: F ─ Recursive: T", expected_after, output);
    assert_file_system_changes_at_least(1); // Files were added immediately after the the directory was created.
    // Once the file monitor for the directory was setup, the directory
    // may already have been populated.

    after();
}


static void test_filemonitor_nonrecursive_create_dir_then_create_files_in_it() {
    before();

    monitor_test_tree = single_folder(FALSE, FALSE);
    pretty_print_tree(monitor_test_tree, output);

    char* expected = KWHT TESTDIRNAME RESET " (3 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before create dir with files ─ Include hidden files: F ─ Recursive: F", expected, output);


    create_dir (testdir_path, "/dir_three");
    create_file(testdir_path, "/dir_three/apa.png");
    create_file(testdir_path, "/dir_three/bepa.png");

    char* expected_after = KWHT TESTDIRNAME RESET " (3 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after create dir with files ─ Include hidden files: F ─ Recursive: F", expected_after, output);
    assert_file_system_changes_at_least(0); // Not recursive -- directory ignored.

    after();
}



static gboolean file_monitor_tests(gpointer data) {
    test_filemonitor_create_file_in_folder_nonrecursive();
    test_filemonitor_create_file_in_folder_recursive();
    test_filemonitor_create_hidden_file_in_folder_nonrecursive();
    test_filemonitor_create_multiple_files_in_folder_recursive();
    test_filemonitor_create_file_in_folder_sorted();
    test_filemonitor_create_folder_in_folder_sorted();
    test_filemonitor_recursive_create_dir_then_create_files_in_it();
    test_filemonitor_nonrecursive_create_dir_then_create_files_in_it();

    g_main_loop_quit((GMainLoop*)data);
    return FALSE;
}

void test_filemon_create() {
    GMainLoop* loop = g_main_loop_new(NULL, FALSE);
    g_timeout_add(1000, file_monitor_tests, loop);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
}
