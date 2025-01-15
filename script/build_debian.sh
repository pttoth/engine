#!/bin/bash

#find out script directory
pushd $(dirname "${BASH_SOURCE[0]}") > /dev/null
scriptdir=$(pwd)
popd > /dev/null

#move to project root directory
projrootdir="${scriptdir}"/..
pushd "${projrootdir}"

platformname=debian
builddir="./build/${platformname}"
cmake "./projects/${platformname}" -B"${builddir}" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release

pushd "${builddir}"

#get available core count
cores=$(nproc)

make -j $cores

# pop from builddir
popd


echo "stripping binary 'bin/debian/libindicus.so'"
strip ./bin/debian/libindicus.so

echo "stripping binary 'bin/debian/OpenGL_test'"
strip "./bin/debian/OpenGL_test"


#--------------------------------------------------
# create start script (ensures correct starting directory)
startupscript_subpath="bin/${platformname}/start_OpenGL_test.sh"
echo "creating startup script '${startupscript_subpath}'"
#--------------------------------------------------
cp "${scriptdir}/data/start_OpenGL_test.sh.model" "${projrootdir}/${startupscript_subpath}"
#--------------------------------------------------
chmod +x "${projrootdir}/${startupscript_subpath}"
#--------------------------------------------------

# pop from project root
popd
