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

#include "test-tree-singlefile.h"
#include "test-tree-folder.h"
#include "test-tree-urilist.h"
#include "test-tree-next-nofiles.h"
#include "test-tree-next-iteration.h"
#include "test-tree-getchildindir.h"
#include "test-tree-addnode.h"
#include "test-tree-numberofleaves.h"
#include "test-filemon-create.h"
#include "test-filemon-urilist-create.h"
#include "test-filemon-delete.h"
#include "test-filemon-urilist-delete.h"
#include "test-filemon-move.h"

#include "utils.h"

int main() {
    before_all();

    test_tree_singlefile();
    test_tree_folder();
    test_tree_urilist();
    test_tree_next_nofiles();
    test_tree_next_iteration();
    test_tree_getchildindir();
    test_tree_addnode();
    test_tree_numberofleaves();
    test_filemon_create();
    test_filemon_urilist_create();
    test_filemon_delete();
    test_filemon_urilist_delete();
    test_filemon_move();

    after_all();

    if(errors > 0) {
        printf("%i tests failed", errors);
    }

    return errors == 0 ? 0 : -1;
}
