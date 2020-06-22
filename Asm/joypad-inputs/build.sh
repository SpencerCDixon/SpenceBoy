#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o joypad.gb main.o
rgbfix -v -p 0 joypad.gb

cp ./joypad.gb ../ROMs
hexdump -C ./joypad.gb
