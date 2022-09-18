#!/bin/bash

set -xe

# Setup emsdk
if [ ! -d "emsdk" ]; then
    wget https://github.com/emscripten-core/emsdk/archive/refs/heads/main.zip -O emscripten.zip
    unzip emscripten.zip
    mv emsdk-main emsdk
    rm -rf emscripten.zip
fi

source ./emsdk/emsdk_env.sh
emsdk update
emsdk install latest
emsdk activate latest
source ./emsdk/emsdk_env.sh

# Setup web build
emcmake cmake -S misc -B build_web -DCMAKE_BUILD_TYPE=Release
