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

#include "test-filemon-delete.h"
#include "utils.h"


static void test_filemonitor_delete_file_in_folder_root() {
    before();

    monitor_test_tree = single_folder(TRUE, FALSE);
    pretty_print_tree(monitor_test_tree, output);

    char* expected = KWHT TESTDIRNAME RESET " (5 children)\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before delete in dirroot ─ Include hidden files: T ─ Recursive: F", expected, output);


    unlink(TESTDIR "/bepa.png");

    char* expected_after = KWHT TESTDIRNAME RESET " (4 children)\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after delete in dirroot ─ Include hidden files: T ─ Recursive: F", expected_after, output);
    assert_file_system_changes_at_least(1);

    after();
}

static void test_filemonitor_delete_hidden_file_in_folder_root() {
    before();

    monitor_test_tree = single_folder(TRUE, FALSE);
    pretty_print_tree(monitor_test_tree, output);

    char* expected = KWHT TESTDIRNAME RESET " (5 children)\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    assert_equals("File monitor before delete hidden ─ Include hidden files: T ─ Recursive: F", expected, output);


    unlink(TESTDIR "/.apa.png");

    char* expected_after = KWHT TESTDIRNAME RESET " (4 children)\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
└─ epa.png\n\
";

    wait_until_file_system_changes_is_as_expected(1);
    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after delete hidden ─ Include hidden files: T ─ Recursive: F", expected_after, output);
    assert_file_system_changes_at_least(1);

    after();
}

static void test_filemonitor_delete_file_in_sub_folder() {
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

    assert_equals("File monitor before delete in subdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    unlink(TESTDIR "/dir_two/sub_dir_one/img1.png");

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
  ├─┬" KWHT "sub_dir_one" RESET " (2 children)\n\
  │ ├─ img0.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three" RESET " (0 children)\n\
  └─┬" KWHT "sub_dir_two" RESET " (4 children)\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after delete in subdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);
    assert_file_system_changes_at_least(1);

    after();
}

static void test_filemonitor_delete_multiple_files() {
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

    assert_equals("File monitor before delete in subdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    unlink(TESTDIR "/dir_two/sub_dir_one/img1.png");
    unlink(TESTDIR "/dir_two/bepa.png");
    unlink(TESTDIR "/dir_two/sub_dir_two/img2.png");

    char* expected_after = KWHT TESTDIRNAME RESET " (5 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one" RESET " (1 children)\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two" RESET " (6 children)\n\
  ├─ apa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four" RESET " (1 children)\n\
  │ └──" KWHT "subsub" RESET " (0 children)\n\
  ├─┬" KWHT "sub_dir_one" RESET " (2 children)\n\
  │ ├─ img0.png\n\
  │ └─ img2.png\n\
  ├──" KWHT "sub_dir_three" RESET " (0 children)\n\
  └─┬" KWHT "sub_dir_two" RESET " (3 children)\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    └─ img3.png\n\
";

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);

    assert_equals("File monitor after delete in subdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);
    assert_file_system_changes_at_least(3);

    after();
}

static void test_filemonitor_delete_subsub_folder() {
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

    assert_equals("File monitor before delete subsubdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    remove_directory(TESTDIR "/dir_two/sub_dir_one/");

    char* expected_after = KWHT TESTDIRNAME RESET " (5 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
├─┬" KWHT "dir_one" RESET " (1 children)\n\
│ └─ two.jpg\n\
└─┬" KWHT "dir_two" RESET " (6 children)\n\
  ├─ apa.png\n\
  ├─ bepa.png\n\
  ├─ cepa.png\n\
  ├─┬" KWHT "sub_dir_four" RESET " (1 children)\n\
  │ └──" KWHT "subsub" RESET " (0 children)\n\
  ├──" KWHT "sub_dir_three" RESET " (0 children)\n\
  └─┬" KWHT "sub_dir_two" RESET " (4 children)\n\
    ├─ img0.png\n\
    ├─ img1.png\n\
    ├─ img2.png\n\
    └─ img3.png\n\
";

    wait_until_file_system_changes_is_as_expected(5);
    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);


    assert_equals("File monitor after delete subsubdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);
    assert_file_system_changes_at_least(4);

    after();
}

static void test_filemonitor_delete_sub_folder() {
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

    assert_equals("File monitor before delete subdir ─ Include hidden files: F ─ Recursive: T", expected, output);


    remove_directory(TESTDIR "/dir_two/");

    char* expected_after = KWHT TESTDIRNAME RESET " (4 children)\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
├─ epa.png\n\
└─┬" KWHT "dir_one" RESET " (1 children)\n\
  └─ two.jpg\n\
";

    wait_until_tree_is_as_expected(monitor_test_tree, expected_after);


    assert_equals("File monitor after delete subdir ─ Include hidden files: F ─ Recursive: T", expected_after, output);
    assert_file_system_changes_at_least(16);

    after();
}

static void test_filemonitor_delete_root_folder() {
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

    assert_equals("File monitor before delete root ─ Include hidden files: F ─ Recursive: T", expected, output);


    remove_directory(TESTDIR);


    wait_until_tree_is_null();

    assert_tree_is_null("File monitor after delete root ─ Include hidden files: F ─ Recursive: T", monitor_test_tree);
    assert_file_system_changes_at_least(22);

    after();
}



static gboolean file_monitor_tests(gpointer data) {
    test_filemonitor_delete_file_in_folder_root();
    test_filemonitor_delete_hidden_file_in_folder_root();
    test_filemonitor_delete_file_in_sub_folder();
    test_filemonitor_delete_multiple_files();
    test_filemonitor_delete_subsub_folder();
    test_filemonitor_delete_sub_folder();
    test_filemonitor_delete_root_folder();

    g_main_loop_quit((GMainLoop*)data);
    return FALSE;
}

void test_filemon_delete() {
    GMainLoop* loop = g_main_loop_new(NULL, FALSE);
    g_timeout_add(1000, file_monitor_tests, loop);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
}
