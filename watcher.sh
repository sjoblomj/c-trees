#!/bin/bash

COMPILE="gcc tests/test.c src/vnrfile.c src/tree.c `pkg-config --cflags --libs gtk+-2.0` -o test"
TEST="./test"
SUCCESS_ICON="icons/success.svg"  # https://commons.wikimedia.org/wiki/File:CrystalClearActionApply.svg
WARNING_ICON="icons/warning.svg"  # https://commons.wikimedia.org/wiki/File:Crystal_Clear_app_error.svg
ERROR_ICON="icons/error.svg"      # https://commons.wikimedia.org/wiki/File:Crystal_128_error.svg
NOTIFICATION_TIMEOUT=5000   # in milliseconds

function notify {
    icon="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/$1"
    notify-send -t $NOTIFICATION_TIMEOUT -i $icon "$2" "$3"
}

function run_test {
    exec 3>&1                       # Save the place that stdout (1) points to.
    result=$($TEST 2>&1 1>&3)       # Run command.  stderr is captured.
    failures="$?"
    exec 3>&-                       # Close FD #3.

    if [[ $failures -eq 0 ]]; then
        notify $SUCCESS_ICON "Tests OK" "All tests passed"
    elif [[ $failures -eq 139 ]]; then
        # http://arstechnica.com/civis/viewtopic.php?f=16&t=113430
        notify $ERROR_ICON "Segmentation fault!" ""
    else
        notify $ERROR_ICON "$failures tests failed!" "$result"
    fi
}

function compile {
    exec 3>&1                       # Save the place that stdout (1) points to.
    out=$($COMPILE 2>&1 1>&3)       # Run command.  stderr is captured.
    failures="$?"
    exec 3>&-                       # Close FD #3.
    printf "$out"

    if [[ $failures -eq 0 ]]; then
        run_test
        if grep "warning:" <<<"${out}" >/dev/null ; then
            notify $WARNING_ICON "Compilation passed with warnings" ""
        fi
    else
        notify $ERROR_ICON "Compilation Error" "Error compiling"
    fi
}

function daemon {
    dir=$1
    chsum1=""

    while [[ true ]]
    do
        chsum2=$(find $dir \( -iname \*.c -o -iname \*.h \) -type f -exec md5sum {} \;)
        if [[ $chsum1 != $chsum2 ]] ; then
            echo ""
            echo -e "\e[1m=======================================\e[0m"
            echo -e "\e[1m============== Compiling ==============\e[0m"
            echo -e "\e[1m=======================================\e[0m"
            compile
            chsum1=$chsum2
        fi
        sleep 2
    done
}

daemon $1
