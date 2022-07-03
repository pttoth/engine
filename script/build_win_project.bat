@echo off
pushd %~dp0

call .\win\set_env.bat
::gets the following variables
::  win_script_dir
::  root_directory
::  build_platform
::  temp_dir
@echo off

set build_directory=%root_directory%\build

pushd "%root_directory%"

mkdir "%root_directory%\build"

cd "%root_directory%\projects\%build_platform%\"

cmake . -B"%build_directory%" -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

cd "%build_directory%"

@echo ------------------------------
@echo building with %NUMBER_OF_PROCESSORS% threads
@echo ------------------------------
make -j %NUMBER_OF_PROCESSORS% --output-sync

popd

popd
@echo on
