#!/bin/bash

#find out script directory
pushd $(dirname "${BASH_SOURCE[0]}") > /dev/null
scriptdir=$(pwd)
popd > /dev/null

#move to project root directory
project_root=$scriptdir/../..
pushd $project_root


#set up temp install directory
tmp_dir_sdl="/tmp/pt_install"
mkdir $tmp_dir_sdl

tmp_dir_install="${tmp_dir_sdl}/install"
mkdir $tmp_dir_install

pushd $tmp_dir_install


download_link=https://github.com/libsdl-org/SDL
repo_tempdirname=$tmp_dir_install/sdl2/repo
repo_version=release-2.0.22

git clone $download_link $repo_tempdirname
cd $repo_tempdirname
git pull
git co $repo_version

#get available core count
cores=$(nproc)

#-------------------------
#call the lib's build script
#-------------------------

#-----
#generic approach for build and/or install
#  (need to set -fPIC to be linkable)
#-----
#./configure
#make -j $cores
#make install

#copy built libs to target lib folder, if didn't do 'install'

#-----
#custom, cmake approach
#-----
repo_builddir=$tmp_dir_install/sdl2/build
mkdir $repo_builddir
cmake . -B"$repo_builddir" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS="-fPIC" -DCMAKE_CXX_FLAGS="-fPIC"
cd $repo_builddir

make -j $cores

#-------------------------

popd

# backup existing 'lib/' contents, if needed
time_now=$(date "+%y-%m-%d_%H%M%S")
mv ./lib/sdl2           "./lib/sdl2.bak_${time_now}"            2> /dev/null
mv ./lib/libSDL2.a      "./lib/libSDL2.a.bak_${time_now}"       2> /dev/null
mv ./lib/libSDL2main.a  "./lib/libSDL2main.a.bak_${time_now}"   2> /dev/null

#create 'lib' directory
mkdir -p ./lib
mkdir -p ./lib/sdl2/include/SDL2
mkdir -p ./lib/sdl2/src/SDL2
mkdir -p ./lib/sdl2/wayland-generated-protocols

#move built libs into 'lib' directory
cp $tmp_dir_install/sdl2/build/libSDL2.a        ./lib/
cp $tmp_dir_install/sdl2/build/libSDL2main.a    ./lib/

cp -r $repo_builddir/wayland-generated-protocols/*  ./lib/sdl2/wayland-generated-protocols/
cp -r $repo_builddir/include/*      ./lib/sdl2/include/SDL2/
cp -r $repo_tempdirname/include/*   ./lib/sdl2/include/SDL2/
cp -r $repo_tempdirname/include/*   ./lib/sdl2/src/SDL2/





#jump back to starting directory
popd
