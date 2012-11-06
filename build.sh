#!/bin/sh

THIS_SYSTEM=$(uname -s)

if [[ "$THIS_SYSTEM" == "Darwin" ]]; then
make -f makefile.x86_64.darwin.clang3x -j
elif [[ "$THIS_SYSTEM" == "Linux" ]]; then
make -f makefile.x86_64.linux.gcc4 -j
else 
echo "Couldn't detect OS"
fi
