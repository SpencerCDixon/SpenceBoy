#!/usr/bin/env bash
set -euo pipefail

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $SCRIPTPATH/..

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    *)          machine="Unsupported:${unameOut}"
esac

if [ "$machine" == "Mac" ]; then
  cd ./Asm/Tools/pandocs/render
elif [ "$machine" == "Linux" ]; then
  cd ./third_party/pandocs/render
fi

npx vuepress dev
