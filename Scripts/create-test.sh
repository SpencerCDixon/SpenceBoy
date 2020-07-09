#!/usr/bin/env bash
###
### create-test — quickly create an ASM test file 
###
### Usage:
###   create-test <test-file-name>
###
### Options:
###   <test-file-name>   Name of test case

set -euo pipefail

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

cd $SCRIPTPATH/..

help() {
    awk '/^###/' "$0"
}

if [[ $# == 0 ]] || [[ "$1" == "-h" ]]; then
    help
    exit 1
fi

echo "Creating test case: $1"
cp ./Assets/test-template.asm "./Asm/test-cases/$1.asm"
