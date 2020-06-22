#!/usr/bin/env bash
set -euo pipefail

for i in ./data/*.gb; do
  ./run_test.sh "$i"
done
