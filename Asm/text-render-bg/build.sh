#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o text-render.gb main.o
rgbfix -v -p 0 text-render.gb

