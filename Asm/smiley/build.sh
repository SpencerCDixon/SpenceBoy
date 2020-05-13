#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o smiley.gb main.o

# -v will tell the software to fix the rom checksum
# -p 0 will pad the rom with zeros.
rgbfix -v -p 0 smiley.gb

# open ../SameBoy.app ./smiley.gb
