#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o dmg_boot.asm
rgblink -o boot.gb main.o

dd if=./boot.gb of=boot.bin bs=256 count=1

rm boot.gb
rm main.o

hexdump -C boot.bin
