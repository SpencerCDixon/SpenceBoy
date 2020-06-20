#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o loading.gb main.o
rgbfix -v -p 0 loading.gb

cp ./loading.gb ../../Tests/data
hexdump -C loading.gb


