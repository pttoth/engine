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
@echo                        math3d
@echo --------------------------------------------------

set download_link=https://github.com/pttoth/math3d
set repo_version=v1.2.6

pushd "%temp_dir%"

@echo ------------------------------
@echo downloading from
@echo   %download_link%
@echo ------------------------------

git clone %download_link% math3d
cd "%temp_dir%\math3d"

::'clone' may not result in fresh state if folder already existed, so try to update it too
git pull
git checkout %repo_version%

@echo ------------------------------
@echo building math3d
@echo ------------------------------

mkdir "%temp_dir%\math3d\build"
cmake . -B"%temp_dir%\math3d\build" -G"MinGW Makefiles"
cd "%temp_dir%\math3d\build"
@echo -----
@echo building with %NUMBER_OF_PROCESSORS% threads
@echo -----
make -j %NUMBER_OF_PROCESSORS% --output-sync

@echo ------------------------------
@echo setting up math3d for use
@echo   folder: "%root_directory%\lib\math3d"
@echo ------------------------------

::NOTE: careful with directory deletion because Batch is abhorrently unpredictable with quotes and spaces in variables

::delete the old contents
::@echo DELETING FOLDER: "%root_directory%\lib\math3d" ...
::rmdir /s "%root_directory%\lib\math3d" /q

mkdir "%root_directory%\lib"
mkdir "%root_directory%\lib\math3d"
mkdir "%root_directory%\lib\math3d\include"
mkdir "%root_directory%\lib\math3d\bin"

::copy include
xcopy "%temp_dir%\math3d\include\*" "%root_directory%\lib\math3d\include" /sy
::copy binaries to lib/
copy "%temp_dir%\math3d\bin\libmath3d.a" "%root_directory%\lib\math3d\bin\" /y

popd

popd

@echo --------------------------------------------------
@echo on
