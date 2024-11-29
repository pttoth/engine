@echo off
pushd %~dp0

call .\win\set_env.bat
::gets the following variables
::  win_script_dir
::  root_directory
::  build_platform
::  temp_dir
@echo off

set build_directory=%root_directory%\build\%build_platform%_debug

pushd "%root_directory%"
mkdir "%build_directory%"

cd "%root_directory%\projects\%build_platform%\"
cmake . -B"%build_directory%" -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug

@echo ------------------------------
@echo building with %NUMBER_OF_PROCESSORS% threads
@echo ------------------------------
pushd "%build_directory%"
mingw32-make -j %NUMBER_OF_PROCESSORS% --output-sync
:: return from build dir
popd



:: return from project rootdir
popd

:: return from script dir
popd
@echo on
