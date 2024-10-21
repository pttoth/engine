#!/bin/bash

#find out script directory
pushd $(dirname "${BASH_SOURCE[0]}") > /dev/null
scriptdir=$(pwd)
popd > /dev/null

#move to project root directory
project_root=$scriptdir/../..
pushd $project_root


#set up temp install directory
tmp_dir_ptlib="/tmp/pt_install"
mkdir $tmp_dir_ptlib

tmp_dir_install="${tmp_dir_ptlib}/install"
mkdir $tmp_dir_install

pushd $tmp_dir_install


download_link=https://github.com/pttoth/ptlib
ptlib_tempdirname=ptlib
repo_version=v2.0.0b12

git clone $download_link $ptlib_tempdirname
cd $ptlib_tempdirname
git pull
git checkout $repo_version

#call the lib's build script
source ./script/build_debian.sh

popd

# backup existing 'lib/' contents, if needed
time_now=$(date "+%y-%m-%d_%H%M%S")
mv ./lib/libptlib.a    "./lib/libptlib.a.bak_${time_now}"   2> /dev/null
mv ./lib/ptlib         "./lib/ptlib.bak_${time_now}"        2> /dev/null

#create 'lib' directory
mkdir -p ./lib/ptlib
mkdir -p ./lib/ptlib/include
mkdir -p ./lib/ptlib/src

#move built libs into 'lib' directory
cp ${tmp_dir_install}/ptlib/bin/debian/libptlib.a ./lib

cp -r ${tmp_dir_install}/ptlib/include/* ./lib/ptlib/include/
cp -r ${tmp_dir_install}/ptlib/src/*     ./lib/ptlib/src/


#jump back to starting directory
popd
