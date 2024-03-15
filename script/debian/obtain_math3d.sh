#!/bin/bash

#find out script directory
pushd $(dirname "${BASH_SOURCE[0]}") > /dev/null
scriptdir=$(pwd)
popd > /dev/null

#move to project root directory
project_root=$scriptdir/../..
pushd $project_root


#set up temp install directory
tmp_dir_math3d="/tmp/pt_install"
mkdir $tmp_dir_math3d

tmp_dir_install="${tmp_dir_math3d}/install"
mkdir $tmp_dir_install

pushd $tmp_dir_install


download_link=https://github.com/pttoth/math3d
math3d_tempdirname=math3d
repo_version=v1.2.0

git clone $download_link $math3d_tempdirname
cd $math3d_tempdirname
git pull
git co $repo_version

#call the lib's build script
source ./script/build_debian.sh

popd

#create 'lib' directory
mkdir -p lib/math3d
mkdir -p lib/math3d/include


#move built libs into 'lib' directory
cp ${tmp_dir_install}/math3d/bin/libmath3d.a ./lib

cp -r ${tmp_dir_install}/math3d/include/* ./lib/math3d/include/


#jump back to starting directory
popd
