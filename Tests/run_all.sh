#!/usr/bin/env bash
set -euo pipefail

# SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

for i in ./precompiled/*.gb; do
  ./run_test.sh "$i"
done

