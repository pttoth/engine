#!/bin/bash

#find out script directory
pushd $(dirname "${BASH_SOURCE[0]}") > /dev/null
scriptdir=$(pwd)
popd > /dev/null

#move to project root directory
pushd $scriptdir/..

cmake ./projects/debian -Bbuild -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release

cd build

#get available core count
cores=$(nproc)

make -j $cores

cd ..

popd
