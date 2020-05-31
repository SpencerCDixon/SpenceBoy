#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o complex-routine.gb main.o
rgbfix -v -p 0 complex-routine.gb

cp ./complex-routine.gb ../../Tests/data
hexdump -C ./complex-routine.gb
