#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o smiley.gb main.o
rgbfix -v -p 0 smiley.gb

cp ./smiley.gb ../../Tests/data
hexdump -C smiley.gb
