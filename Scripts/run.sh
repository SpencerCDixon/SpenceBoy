#!/usr/bin/env bash
set -exuo pipefail

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

cd $SCRIPTPATH/..

mkdir -p build

pushd build
cmake -GNinja ..
ninja
# ./gameboy --rom $SCRIPTPATH/../Asm/ROMs/serenity.gb --asset-dir $SCRIPTPATH/../Assets

# Example running in debug mode which will hit breakpoints and do step debugging
# ./gameboy --rom $SCRIPTPATH/../Asm/ROMs/joypad.gb --asset-dir $SCRIPTPATH/../Assets 

# Example running in debug mode which will hit breakpoints and do step debugging
./gameboy --rom $SCRIPTPATH/../Asm/ROMs/joypad.gb --asset-dir $SCRIPTPATH/../Assets --debug

# Example running a test case in debug mode
# ./gameboy --rom $SCRIPTPATH/../Tests/data/increments.gb --asset-dir $SCRIPTPATH/../Assets --debug
popd
