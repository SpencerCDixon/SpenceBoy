#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o serenity.gb main.o
rgbfix -v -p 0 serenity.gb

cp ./serenity.gb ../ROMs
hexdump -C serenity.gb

