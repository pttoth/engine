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
repo_version=v2.0.0b9
#repo_version=EventAddMoveCtor

git clone $download_link $ptlib_tempdirname
cd $ptlib_tempdirname
git pull
git co $repo_version

#call the lib's build script
source ./script/build_debian.sh

popd

#create 'lib' directory
mkdir lib
mkdir lib/ptlib
mkdir lib/ptlib/include


#move built libs into 'lib' directory
cp ${tmp_dir_install}/ptlib/bin/debian/libptlib.a ./lib

cp -r ${tmp_dir_install}/ptlib/include/* ./lib/ptlib/include/


#jump back to starting directory
popd
