#!/bin/bash

cmake ./projects/debian -Bbuild -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug

cd build

#get available core count
cores=$(nproc)

make -j $cores

cd ..


