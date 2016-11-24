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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../src/tree.h"
#include "../src/vnrfile.h"
#include "tree-utils.h"

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\033[1m\033[37m"
#define RESET "\x1B[0m"
#define OUTPUTSIZE 2048
#define TESTDIR "c-trees-tests"

typedef enum {SINGLE_FILE, SINGLE_FOLDER, VALID_LIST, SEMI_INVALID_LIST, COMPLETELY_INVALID_LIST} inputtype;

int errors = 0;
char* output;

void assertErrorIsNull(GError* error);



void destroy_node(GNode *node, gpointer data) {
    vnr_file_destroy_data(node->data);
}

void free_tree(GNode *tree) {
    g_node_children_foreach(tree, G_TRAVERSE_ALL, destroy_node, NULL);
    g_node_destroy(tree);
}


GNode* single_file(gboolean include_hidden, gboolean recursive) {
    GNode *tree = NULL;
    char *path = TESTDIR "/bepa.png";
    GError *error = NULL;

    tree = vnr_file_load_single_uri(path, include_hidden, recursive, &error);
    assertErrorIsNull(error);
    return tree;
}

GNode* single_folder(gboolean include_hidden, gboolean recursive) {
    GNode *tree = NULL;
    char *path = TESTDIR;
    GError *error = NULL;

    tree = vnr_file_load_single_uri(path, include_hidden, recursive, &error);
    assertErrorIsNull(error);
    return tree;
}

GNode* uri_list(gboolean include_hidden, gboolean recursive) {
    GNode *tree = NULL;

    GSList *uri_list = NULL;
    uri_list = g_slist_prepend(uri_list, TESTDIR "/.apa.png");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/bepa.png");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/cepa.jpg");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/.depa.gif");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/test.txt");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/dir_two");

    GError *error = NULL;

    tree = vnr_file_load_uri_list(uri_list, include_hidden, recursive, &error);
    assertErrorIsNull(error);
    return tree;
}

GNode* uri_list_with_some_invalid_entries(gboolean include_hidden, gboolean recursive) {
    GNode *tree = NULL;

    GSList *uri_list = NULL;
    uri_list = g_slist_prepend(uri_list, TESTDIR "/.apa.png");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/bepa.png");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/NON_EXISTANT_FILE.jpg");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/cepa.jpg");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/.depa.gif");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/test.txt");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/dir_two");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/NON_EXISTANT_DIR");

    GError *error = NULL;

    tree = vnr_file_load_uri_list(uri_list, include_hidden, recursive, &error);
    assertErrorIsNull(error);
    return tree;
}

GNode* uri_list_with_only_invalid_entries(gboolean include_hidden, gboolean recursive) {
    GNode *tree = NULL;

    GSList *uri_list = NULL;
    uri_list = g_slist_prepend(uri_list, TESTDIR "/NON_EXISTANT_FILE.jpg");
    uri_list = g_slist_prepend(uri_list, TESTDIR "/NON_EXISTANT_DIR");

    GError *error = NULL;

    tree = vnr_file_load_uri_list(uri_list, include_hidden, recursive, &error);
    assertErrorIsNull(error);
    return tree;
}

char* get_printed_tree(inputtype type, gboolean include_hidden, gboolean recursive) {
    GNode* tree;
    if(type == SINGLE_FILE) {
        tree = single_file(include_hidden, recursive);
    } else if(type == SINGLE_FOLDER) {
        tree = single_folder(include_hidden, recursive);
    } else if(type == SEMI_INVALID_LIST) {
        tree = uri_list_with_some_invalid_entries(include_hidden, recursive);
    } else if(type == COMPLETELY_INVALID_LIST) {
        tree = uri_list_with_only_invalid_entries(include_hidden, recursive);
    } else {
        tree = uri_list(include_hidden, recursive);
    }

    pretty_print_tree(tree, output);

    free_tree(tree);
    return output;
}



///////////////////////////////


void create_dir(const char *path) {
    struct stat st = {0};

    if(stat(path, &st) == -1) {
        mkdir(path, 0700);
    }
}

void create_file(const char *path) {
    fclose(fopen(path, "w"));
}

void create_file_structure() {

    create_dir (TESTDIR "");
    create_file(TESTDIR "/test.txt");
    create_file(TESTDIR "/cepa.jpg");
    create_file(TESTDIR "/.apa.png");
    create_file(TESTDIR "/not_an_image.yo");
    create_file(TESTDIR "/.depa.gif");
    create_file(TESTDIR "/bepa.png");

    create_dir (TESTDIR "/dir_one");
    create_file(TESTDIR "/dir_one/one.txt");
    create_file(TESTDIR "/dir_one/two.jpg");
    create_file(TESTDIR "/dir_one/.three.png");

    create_dir (TESTDIR "/dir_one/.secrets");
    create_file(TESTDIR "/dir_one/.secrets/img.jpg");

    create_dir (TESTDIR "/dir_two");
    create_file(TESTDIR "/dir_two/bepa.png");

    create_dir (TESTDIR "/dir_two/sub_dir_two");
    create_file(TESTDIR "/dir_two/sub_dir_two/img0.png");
    create_file(TESTDIR "/dir_two/sub_dir_two/img2.png");
    create_file(TESTDIR "/dir_two/sub_dir_two/img1.png");
    create_file(TESTDIR "/dir_two/sub_dir_two/img3.png");

    create_dir (TESTDIR "/dir_two/sub_dir_one");
    create_file(TESTDIR "/dir_two/sub_dir_one/img0.png");
    create_file(TESTDIR "/dir_two/sub_dir_one/img2.png");
    create_file(TESTDIR "/dir_two/sub_dir_one/img1.png");

    create_file(TESTDIR "/dir_two/apa.png");
    create_file(TESTDIR "/dir_two/cepa.png");

    create_dir (TESTDIR "/dir_two/sub_dir_three");
    create_dir (TESTDIR "/dir_two/sub_dir_four");
    create_dir (TESTDIR "/dir_two/sub_dir_four/subsub");
}


void remove_file_structure() {
    remove_directory(TESTDIR);
}

int remove_directory(const char *path) {
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if(d) {
      struct dirent *p;
      r = 0;

      while(!r && (p = readdir(d))) {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if(!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
             continue;

          len = path_len + strlen(p->d_name) + 2;
          buf = malloc(len);

          if(buf) {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if(!stat(buf, &statbuf)) {
                if(S_ISDIR(statbuf.st_mode))
                   r2 = remove_directory(buf);
                else
                   r2 = unlink(buf);
             }

             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }

   if(!r)
      r = rmdir(path);

   return r;
}



///////////////////////////////



void before() {
    output = (char*) malloc(sizeof(char) * OUTPUTSIZE);
    create_file_structure();
}

void after() {
    if(output != NULL) {
        free(output);
    }
    output = NULL;
    remove_file_structure();
}

void assertEquals(char* description, char* expected, char* actual) {
    if(strcmp(expected, actual) == 0) {
        printf("  " KGRN "[PASS]  %s\n" RESET, description);
    } else {
        printf("\n");
        printf("  " KRED "[FAIL]  %s\n" RESET, description);
        printf("Expected:\n'%s'\n\nActual:\n'%s'\n\n", expected, actual);
        fprintf(stderr, "* %s\n", description);
        errors++;
    }
}

void assertTreesEqual(char* description, GNode* expected, GNode* actual) {
    if(expected == actual) {
        printf("  " KGRN "[PASS]  %s\n" RESET, description);
    } else {
        printf("\n");
        printf("  " KRED "[FAIL]  %s\n" RESET, description);
        fprintf(stderr, "* %s\n", description);
        errors++;
    }
}

void assertTreeIsNull(char* description, GNode* tree) {
    if(tree == NULL) {
        printf("  " KGRN "[PASS]  %s\n" RESET, description);
    } else {
        printf("\n");
        printf("  " KRED "[FAIL]  %s\n" RESET, description);
        fprintf(stderr, "* %s\n", description);
        errors++;
    }
}

void assertErrorIsNull(GError* error) {
    if(error != NULL) {
        printf("  " KRED "[FAIL]  Error should not be set! Was: %s\n" RESET, error->message);
        fprintf(stderr, "* Error should not be set! Was: %s\n", error->message);
        errors++;
    }
}

void assertErrorIsNotNull(GError* error) {
    if(error == NULL) {
        printf("  " KRED "[FAIL]  Error should be set!\n" RESET);
        fprintf(stderr, "* Error should be set!\n");
        errors++;
    }
}

//////////////////

void test_singleFile_NonExistantFile() {
    before();

    char *path = "non_existant_file.jpg";
    GError *error = NULL;

    assertTreeIsNull("Non existant file ─ Include hidden files: F ─ Recursive: F", vnr_file_load_single_uri(path, FALSE, FALSE, &error));
    assertErrorIsNotNull(error);
    g_clear_error(&error);
    assertTreeIsNull("Non existant file ─ Include hidden files: F ─ Recursive: T", vnr_file_load_single_uri(path, FALSE, TRUE,  &error));
    assertErrorIsNotNull(error);
    g_clear_error(&error);
    assertTreeIsNull("Non existant file ─ Include hidden files: T ─ Recursive: F", vnr_file_load_single_uri(path, TRUE,  FALSE, &error));
    assertErrorIsNotNull(error);
    g_clear_error(&error);
    assertTreeIsNull("Non existant file ─ Include hidden files: T ─ Recursive: T", vnr_file_load_single_uri(path, TRUE,  TRUE,  &error));
    assertErrorIsNotNull(error);
    g_error_free(error);

    after();
}

void test_singleFile_DontIncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIR "  (2 children)" RESET "\n\
├─ bepa.png\n\
└─ cepa.jpg\n\
";

    assertEquals("Single file ─ Include hidden files: F ─ Recursive: F", expected, get_printed_tree(SINGLE_FILE, FALSE, FALSE));

    after();
}

void test_singleFile_DontIncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIR "  (4 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
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

    assertEquals("Single file ─ Include hidden files: F ─ Recursive: T", expected, get_printed_tree(SINGLE_FILE, FALSE, TRUE));

    after();
}

void test_singleFile_IncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIR "  (4 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
└─ cepa.jpg\n\
";

    assertEquals("Single file ─ Include hidden files: T ─ Recursive: F", expected, get_printed_tree(SINGLE_FILE, TRUE, FALSE));

    after();
}

void test_singleFile_IncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIR "  (6 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
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

    assertEquals("Single file ─ Include hidden files: T ─ Recursive: T", expected, get_printed_tree(SINGLE_FILE, TRUE, TRUE));

    after();
}



void test_singleFolder_DontIncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIR "  (2 children)" RESET "\n\
├─ bepa.png\n\
└─ cepa.jpg\n\
";

    assertEquals("Single folder ─ Include hidden files: F ─ Recursive: F", expected, get_printed_tree(SINGLE_FOLDER, FALSE, FALSE));

    after();
}

void test_singleFolder_DontIncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIR "  (4 children)" RESET "\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
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

    assertEquals("Single folder ─ Include hidden files: F ─ Recursive: T", expected, get_printed_tree(SINGLE_FOLDER, FALSE, TRUE));

    after();
}

void test_singleFolder_IncludeHidden_NotRecursive() {
    before();

    char* expected = KWHT TESTDIR "  (4 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
└─ cepa.jpg\n\
";

    assertEquals("Single folder ─ Include hidden files: T ─ Recursive: F", expected, get_printed_tree(SINGLE_FOLDER, TRUE, FALSE));

    after();
}

void test_singleFolder_IncludeHidden_Recursive() {
    before();

    char* expected = KWHT TESTDIR "  (6 children)" RESET "\n\
├─ .apa.png\n\
├─ .depa.gif\n\
├─ bepa.png\n\
├─ cepa.jpg\n\
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

    assertEquals("Single folder ─ Include hidden files: T ─ Recursive: T", expected, get_printed_tree(SINGLE_FOLDER, TRUE, TRUE));

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

    assertEquals("Uri List ─ Include hidden files: F ─ Recursive: F", expected, get_printed_tree(VALID_LIST, FALSE, FALSE));
    after(); before();
    assertEquals("Uri List ─ Some invalid files. Include hidden files: F ─ Recursive: F", expected, get_printed_tree(SEMI_INVALID_LIST, FALSE, FALSE));
    after(); before();
    assertEquals("Uri List ─ Only invalid files. Include hidden files: F ─ Recursive: F", KWHT "<ROOT>  (0 children)" RESET "\n", get_printed_tree(COMPLETELY_INVALID_LIST, FALSE, FALSE));

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

    assertEquals("Uri List ─ Include hidden files: F ─ Recursive: T", expected, get_printed_tree(VALID_LIST, FALSE, TRUE));
    after(); before();
    assertEquals("Uri List ─ Some invalid files. Include hidden files: F ─ Recursive: T", expected, get_printed_tree(SEMI_INVALID_LIST, FALSE, TRUE));
    after(); before();
    assertEquals("Uri List ─ Only invalid files. Include hidden files: F ─ Recursive: T", KWHT "<ROOT>  (0 children)" RESET "\n", get_printed_tree(COMPLETELY_INVALID_LIST, FALSE, TRUE));

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

    assertEquals("Uri List ─ Include hidden files: T ─ Recursive: F", expected, get_printed_tree(VALID_LIST, TRUE, FALSE));
    after(); before();
    assertEquals("Uri List ─ Some invalid files. Include hidden files: T ─ Recursive: F", expected, get_printed_tree(SEMI_INVALID_LIST, TRUE, FALSE));
    after(); before();
    assertEquals("Uri List ─ Only invalid files. Include hidden files: T ─ Recursive: F", KWHT "<ROOT>  (0 children)" RESET "\n", get_printed_tree(COMPLETELY_INVALID_LIST, TRUE, FALSE));

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

    assertEquals("Uri List ─ Include hidden files: T ─ Recursive: T", expected, get_printed_tree(VALID_LIST, TRUE, TRUE));
    after(); before();
    assertEquals("Uri List ─ Some invalid files. Include hidden files: T ─ Recursive: T", expected, get_printed_tree(SEMI_INVALID_LIST, TRUE, TRUE));
    after(); before();
    assertEquals("Uri List ─ Only invalid files. Include hidden files: T ─ Recursive: T", KWHT "<ROOT>  (0 children)" RESET "\n", get_printed_tree(COMPLETELY_INVALID_LIST, TRUE, TRUE));

    after();
}



void test_getNextInTree_NullIn() {
    before();
    assertTreeIsNull("Get Next ─ Input is NULL", get_next_in_tree(NULL));
    after();
}

void test_getNextInTree_RootWithoutElements() {
    before();

    GNode *tree = g_node_new(NULL);
    assertTreesEqual("Get Next ─ Input is Root without elements", tree, get_next_in_tree(tree));
    free_tree(tree);

    after();
}

void test_getNextInTree_RootWithOnlyDir() {
    before();

    VnrFile* vnrfile = vnr_file_create_new("filepath", "display_name", TRUE);
    GNode *tree = g_node_new(vnrfile);
    assertTreesEqual("Get Next ─ Input is Root with only one dir", tree, get_next_in_tree(tree));
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

    assertTreesEqual("Get Next ─ Single folder ─ Input is Root with only three dirs", tree, get_next_in_tree(tree));
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
    assertErrorIsNull(error);

    assertTreesEqual("Get Next ─ Uri List ─ Input is Root with only three dirs", tree, get_next_in_tree(tree));
    free_tree(tree);

    after();
}

GNode* iteration_test(char* description_base, GNode* node, char* expected_file_name) {
    VnrFile* vnrfile = node->data;

    char* description = create_string(description_base, expected_file_name);
    assertEquals(description, expected_file_name, vnrfile->display_name);

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

    node = g_node_next_sibling(node);
    VnrFile* vnrfile = node->data;
    assertEquals("First directory should be dir_one", "dir_one", vnrfile->display_name);

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
// tests  (6 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
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
// tests  (6 children)
// ├─ .apa.png
// ├─ .depa.gif
// ├─ bepa.png
// ├─ cepa.jpg
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



int main()
{
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

    return errors;
}
