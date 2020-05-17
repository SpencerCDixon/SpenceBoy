#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o increments.gb main.o
rgbfix -v -p 0 increments.gb
