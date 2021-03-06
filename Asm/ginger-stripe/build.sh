#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o ginger-bg.gb main.o
rgbfix -v -p 0 ginger-bg.gb

cp ./ginger-bg.gb ../../Tests/data
hexdump -C ginger-bg.gb
