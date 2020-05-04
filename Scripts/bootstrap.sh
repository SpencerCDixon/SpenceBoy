#!/usr/bin/env bash
set -Exuo pipefail

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    *)          machine="Unsupported:${unameOut}"
esac

if [ "$machine" == "Mac" ]; then
  echo "detected machine is macOS... installing deps"
  brew install rgbds
else 
  echo "detected machine is Linux... installing deps"
  pushd ..
  mkdir -p third_party
  pushd third_party
  sudo apt-get install byacc flex pkg-config libpng-dev
  git clone git@github.com:rednex/rgbds.git
  pushd rgbds
  make
  make install
  popd # rgbds
  popd # third_party
  popd # root
fi
