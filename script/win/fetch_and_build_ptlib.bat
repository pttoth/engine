@echo off
pushd %~dp0
call .\set_env.bat
::gets the following variables
::  win_script_dir
::  root_directory
::  build_platform
::  temp_dir

@echo off

@echo --------------------------------------------------
@echo                        ptlib
@echo --------------------------------------------------

set download_link=https://github.com/pttoth/ptlib

pushd "%temp_dir%"

@echo ------------------------------
@echo downloading from
@echo   %download_link%
@echo ------------------------------

git clone %download_link% ptlib
cd "%temp_dir%\ptlib"

@echo ------------------------------
@echo building ptlib
@echo ------------------------------

mkdir "%temp_dir%\ptlib\build"
cd "projects/win64/"
cmake . -B"%temp_dir%\ptlib\build" -G"MinGW Makefiles"
cd "%temp_dir%\ptlib\build"
@echo -----
@echo building with %NUMBER_OF_PROCESSORS% threads
@echo -----
make -j %NUMBER_OF_PROCESSORS% --output-sync

::dirty, but fast hack to build the complete package with everything that needs the 'ptlib' library dependency ready
::	added the first-build error in ptlib doc as a known issue
make -j %NUMBER_OF_PROCESSORS% --output-sync

@echo ------------------------------
@echo setting up ptlib for use
@echo   folder: "%root_directory%\lib\ptlib"
@echo ------------------------------

::NOTE: careful with directory deletion because Batch is abhorrently unpredictable with quotes in variables

::delete the old contents
::@echo DELETING FOLDER: "%root_directory%\lib\ptlib" ...
::rmdir /s "%root_directory%\lib\ptlib" /q

mkdir "%root_directory%\lib"
mkdir "%root_directory%\lib\ptlib"
mkdir "%root_directory%\lib\ptlib\include"
mkdir "%root_directory%\lib\ptlib\include\pt"
mkdir "%root_directory%\lib\ptlib\bin"

::copy include
xcopy "%temp_dir%\ptlib\include\pt\*" "%root_directory%\lib\ptlib\include\pt" /sy
::copy binaries to lib/
copy "%temp_dir%\ptlib\bin\win64\libptlib.a" "%root_directory%\lib\ptlib\bin\" /y

popd

popd

@echo --------------------------------------------------
@echo on
