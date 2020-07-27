#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o dmg_neviksti.asm
rgblink -o dmg_neviksti.gb main.o

dd if=./dmg_neviksti.gb of=dmg_neviksti.bin bs=256 count=1

rm dmg_neviksti.gb
rm main.o

hexdump -C dmg_neviksti.bin
