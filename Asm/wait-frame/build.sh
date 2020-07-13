#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o wait-frame.gb main.o
rgbfix -v -p 0 wait-frame.gb

cp ./wait-frame.gb ../ROMs
hexdump -C ./wait-frame.gb
