#!/bin/bash

cmake ./projects/debian -Bbuild -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release

cd build

make -j 8

cd ..


