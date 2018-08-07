gcc `pkg-config --cflags --libs gtk+-2.0` -o test -Wall -Wextra \
  tests/test-filemon-create.c \
  tests/test-filemon-delete.c \
  tests/test-filemon-move.c \
  tests/test-filemon-urilist-create.c \
  tests/test-filemon-urilist-delete.c \
  tests/test-tree-addnode.c \
  tests/test-tree-folder.c \
  tests/test-tree-getchildindir.c \
  tests/test-tree-next-iteration.c \
  tests/test-tree-next-nofiles.c \
  tests/test-tree-numberofleaves.c \
  tests/test-tree-singlefile.c \
  tests/test-tree-urilist.c \
  tests/tree-printer.c \
  tests/utils.c \
  tests/run-all-tests.c \
  src/vnrfile.c \
  src/tree.c
