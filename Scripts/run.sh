#!/usr/bin/env bash
set -Exuo pipefail

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

mkdir -p build

pushd build
cmake ..
make
./gameboy --rom $SCRIPTPATH/../Tests/data/joypad.gb --asset-dir $SCRIPTPATH/../Assets
popd

