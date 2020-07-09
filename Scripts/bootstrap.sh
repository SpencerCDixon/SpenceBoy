#!/usr/bin/env bash
set -Exuo pipefail

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $SCRIPTPATH/..

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    *)          machine="Unsupported:${unameOut}"
esac

echo "Machine detected as: $machine"

if [ "$machine" == "Mac" ]; then
  echo "detected machine is macOS... installing deps"
  brew install rgbds
  brew install colordiff
  brew install cmake
elif [ "$machine" == "Linux" ]; then
  echo "detected machine is Linux... installing deps"
  mkdir -p third_party
  pushd third_party
  sudo apt-get install byacc flex pkg-config libpng-dev
  git clone git@github.com:rednex/rgbds.git
  pushd rgbds
  make
  make install
  popd # rgbds
  popd # third_party
  echo "installing SDL dependencies"
  sudo apt install cmake libsdl2-dev libsdl2-image-dev g++
else
  echo "your platform is not currently supported :-("
fi

echo "copying git pre-push hook"
cp ./Assets/git-pre-push-template ./.git/hooks/pre-push
