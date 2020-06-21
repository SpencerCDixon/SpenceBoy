#!/usr/bin/env bash

if [ "$1" = "" ] ; then
  echo "usage: $0 <testfile>"
  exit 1
fi

if type -pa colordiff > /dev/null; then
    FANCYDIFF=colordiff
else
    FANCYDIFF=diff
fi

PROGRAM="../build/gameboy --no-gui --test --rom"
TEST=$1
EXPECTATION="$TEST.snapshot"
RESULT=`mktemp /tmp/tmp.XXXXXX || exit 1`

$PROGRAM $TEST > $RESULT
if [ -e $EXPECTATION ]; then
    if diff -q $EXPECTATION $RESULT >/dev/null; then
        echo -ne "[ \033[32;1mPASS\033[0m ] "
    else
        echo -ne "[ \033[31;1mFAIL\033[0m ] "
        $FANCYDIFF -u $EXPECTATION $RESULT | less -R
    fi
else
    cat $RESULT > $EXPECTATION
    echo -ne "[ \033[33;1mNEW\033[0m  ] "
fi
echo $TEST

rm -f $RESULT >/dev/null
