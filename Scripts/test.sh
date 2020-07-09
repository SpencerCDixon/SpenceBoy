#!/usr/bin/env bash
set -euo pipefail

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

cd $SCRIPTPATH/..

mkdir -p build

echo
echo "-----------------------"
echo -e "\033[33;1mBuilding SpenceBoy...\033[0m"
echo "-----------------------"
echo

pushd build
cmake -GNinja ..
ninja
popd

pushd Asm
pushd test-cases
echo
echo "-----------------------"
echo -e "\033[33;1mBuilding Test ROMs...\033[0m"
echo "-----------------------"
echo
./build-all.sh
popd
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

echo
echo "-----------------------"
echo -e "\033[32;1mRunning Vector tests...\033[0m"
echo "-----------------------"
echo
../build/vector_test

echo
echo "-----------------------"
echo -e "\033[32;1mRunning ROM tests...\033[0m"
echo "-----------------------"
echo
./run_all.sh
popd

