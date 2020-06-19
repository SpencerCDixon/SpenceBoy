#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o stripes.gb main.o
rgbfix -v -p 0 stripes.gb

cp ./stripes.gb ../../Tests/data
hexdump -C stripes.gb
