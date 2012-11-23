#!/bin/bash

THIS_SYSTEM=$(uname -s)

if [ "$THIS_SYSTEM" == "Darwin" ]; then
make -f makefile.x86_64.darwin.clang3x clean
elif [ "$THIS_SYSTEM" == "Linux" ]; then
make -f makefile.x86_64.linux.gcc4 clean
else 
echo "Couldn't detect OS"
fi
