gcc tests/tree-utils.c  tests/monitor-tests.c tests/tree-tests.c tests/test-utils.c src/vnrfile.c src/tree.c `pkg-config --cflags --libs gtk+-2.0` -o test -Wall -Wextra
