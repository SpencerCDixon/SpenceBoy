#!/usr/bin/env bash
set -Exuo pipefail

mkdir -p build

pushd build
cmake ..
make test
popd

