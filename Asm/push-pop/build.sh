#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o push-pop.gb main.o
rgbfix -v -p 0 push-pop.gb

