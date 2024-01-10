#!/bin/bash

#find out script directory
pushd $(dirname "${BASH_SOURCE[0]}") > /dev/null
scriptdir=$(pwd)
popd > /dev/null

#move to project root directory
pushd "${scriptdir}"/..

builddir="./build/debian_debug"
cmake ./projects/debian -B"${builddir}" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug

pushd "${builddir}"

#get available core count
cores=$(nproc)

make -j $cores

popd

popd
