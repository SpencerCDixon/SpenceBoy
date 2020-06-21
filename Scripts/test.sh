#!/usr/bin/env bash
set -Euo pipefail

mkdir -p build

pushd build
cmake ..
make
popd

pushd Tests
echo
echo "-----------------------"
echo -e "\033[32;1mRunning String tests...\033[0m"
echo "-----------------------"
echo
../build/string_test

echo
echo "-----------------------"
echo -e "\033[32;1mRunning Joypad tests...\033[0m"
echo "-----------------------"
echo
../build/joypad_test
popd

