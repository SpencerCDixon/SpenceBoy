#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o dmg_boot.asm
rgblink -o dmg_boot.gb main.o

dd if=./dmg_boot.gb of=dmg_boot.bin bs=256 count=1

rm dmg_boot.gb
rm main.o

hexdump -C dmg_boot.bin
