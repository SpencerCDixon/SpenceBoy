#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o lcd-status.gb main.o
rgbfix -v -p 0 lcd-status.gb

cp ./lcd-status.gb ../../Tests/data
hexdump -C lcd-status.gb

