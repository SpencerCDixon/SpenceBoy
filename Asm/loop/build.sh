#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o loop.gb main.o
rgbfix -v -p 0 loop.gb
