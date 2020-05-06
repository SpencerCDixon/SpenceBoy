#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o hello-world.gb main.o
rgbfix -v -p 0 hello-world.gb
