#!/bin/bash

#TODO: fix script to enforce starting working directory to be same as script
#  currently the script fails if it is called from another directory

tmp_dir_ptlib="/tmp/OpenGLTutorials"
#tmp_dir_ptlib="./ptlib"
mkdir $tmp_dir_ptlib

tmp_dir_install="${tmp_dir_ptlib}/install"
mkdir $tmp_dir_install

wdir=$tmp_dir_install

pushd $wdir

#echo ...

git clone https://github.com/pttoth/ptlib ptlib
cd ptlib
dir_build="$(pwd)/build"
mkdir $dir_build

cd ./projects/debian
cmake . -B"$dir_build" -G"Unix Makefiles"

cd $dir_build

#get available core count
cores=$(nproc)

#build with all available cores
make -j $cores

popd
pushd ../..

#create 'lib' directory
mkdir lib
mkdir lib/ptlib
mkdir lib/ptlib/include
#mkdir lib/ptlib/bin

#move built libs into 'lib' directory
#cp ${tmp_dir_install}/ptlib/bin/debian/libptlib.a ./lib/ptlib/bin
cp ${tmp_dir_install}/ptlib/bin/debian/libptlib.a ./lib

cp -r ${tmp_dir_install}/ptlib/include/* ./lib/ptlib/include/

popd

#pushd $dirtemp





