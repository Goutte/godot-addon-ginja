#!/usr/bin/env sh

# You'll need:
#    apt install gcc-multilib g++-multilib
#    apt install g++-mingw-w64-i686

AMOUNT_OF_CORES=${CORES:-1}
TARGET=${TARGET:-debug}

# ---------------

set -e

cd godot-cpp/

scons platform=linux   target=$TARGET arch=x86_32    -j $AMOUNT_OF_CORES
scons platform=linux   target=$TARGET arch=x86_64    -j $AMOUNT_OF_CORES

scons platform=windows target=$TARGET arch=x86_64    -j $AMOUNT_OF_CORES
scons platform=windows target=$TARGET arch=x86_32    -j $AMOUNT_OF_CORES

scons platform=osx     target=$TARGET arch=universal -j $AMOUNT_OF_CORES
scons platform=osx     target=$TARGET arch=x86_64    -j $AMOUNT_OF_CORES
# Only universal, arm64, and x86_64 are supported on macOS. Exiting.
#scons platform=osx     target=$TARGET arch=x86_32    -j $AMOUNT_OF_CORES


cd ..

scons platform=linux   target=$TARGET arch=x86_32    -j $AMOUNT_OF_CORES
scons platform=linux   target=$TARGET arch=x86_64    -j $AMOUNT_OF_CORES

scons platform=windows target=$TARGET arch=x86_64    -j $AMOUNT_OF_CORES
scons platform=windows target=$TARGET arch=x86_32    -j $AMOUNT_OF_CORES

scons platform=osx     target=$TARGET arch=x86_64    -j $AMOUNT_OF_CORES
