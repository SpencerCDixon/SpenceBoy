#!/usr/bin/env bash
set -euo pipefail

rgbasm -o main.o main.asm
rgblink -o dec-jumps.gb main.o
rgbfix -v -p 0 dec-jumps.gb

cp ./dec-jumps.gb ../../Tests/data
open ../Tools/SameBoy.app ./dec-jumps.gb
