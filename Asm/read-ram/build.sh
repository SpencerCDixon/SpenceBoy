#!/usr/bin/env bash
set -euo pipefail

rgbasm -Wall -Wextra -o main.o main.asm
rgblink -o ram.gb main.o

# -v will tell the software to fix the rom checksum
# -p 0 will pad the rom with zeros.
rgbfix -t "RAM Read" -v -p 0 ram.gb
