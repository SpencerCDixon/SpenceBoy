#!/usr/bin/env bash
set -euo pipefail

for i in *.asm; do
    [ -f "$i" ] || break
    NAME=$(echo $i | sed s/.asm//)

    echo "building $NAME"

    rgbasm -o $NAME.o $i
    rgblink -o $NAME.gb $NAME.o
    rgbfix -v -p 0 $NAME.gb

    cp $NAME.gb ../../Tests/data
    rm $NAME.gb $NAME.o
done
