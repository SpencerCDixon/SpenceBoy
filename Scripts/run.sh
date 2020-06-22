#!/usr/bin/env bash
set -Exuo pipefail

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

cd $SCRIPTPATH/..

mkdir -p build

pushd build
cmake ..
make
./gameboy --rom $SCRIPTPATH/../Asm/ROMs/joypad.gb --asset-dir $SCRIPTPATH/../Assets
popd
