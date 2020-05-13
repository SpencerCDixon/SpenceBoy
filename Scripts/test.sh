#!/usr/bin/env bash
set -Exuo pipefail

mkdir -p build

pushd build
cmake ..
make
popd

pushd Tests
../build/cpu_test
popd

