#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o bit-masks.gb main.o
rgbfix -v -p 0 bit-masks.gb
