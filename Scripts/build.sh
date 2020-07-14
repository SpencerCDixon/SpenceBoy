#!/usr/bin/env bash
set -exuo pipefail

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $SCRIPTPATH/..

mkdir -p build

pushd build
cmake -GNinja ..
ninja
popd
