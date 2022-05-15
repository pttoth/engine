#!/bin/bash

#TODO: fix script to enforce starting working directory to be same as script
#  currently the script fails if it is called from another directory

tmp_dir_math3d="/tmp/pt_install"
mkdir $tmp_dir_math3d

tmp_dir_install="${tmp_dir_math3d}/install"
mkdir $tmp_dir_install

wdir=$tmp_dir_install

pushd $wdir

#echo ...

download_link=https://github.com/pttoth/math3d
math3d_dirname=math3d
repo_version=v1.1.0a1

git clone $download_link $math3d_dirname
cd $math3d_dirname
git pull
git co $repo_version

dir_build="$(pwd)/build"
mkdir $dir_build

#TODO: add a commented-out version that show how to define macros for build
cmake . -B"$dir_build" -G"Unix Makefiles"

cd $dir_build

#get available core count
cores=$(nproc)

#build with all available cores
make -j $cores

#jump back from the lib directory to OpenGLTutorials
popd
pushd ../..

#create 'lib' directory
mkdir lib
mkdir lib/math3d
mkdir lib/math3d/include


#move built libs into 'lib' directory
cp ${tmp_dir_install}/math3d/bin/libmath3d.a ./lib

cp -r ${tmp_dir_install}/math3d/include/* ./lib/math3d/include/

popd

#pushd $dirtemp





