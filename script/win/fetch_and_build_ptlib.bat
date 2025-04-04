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
set repo_version=v2.0.0b12

pushd "%temp_dir%"

@echo ------------------------------
@echo downloading '%repo_version%' from
@echo   %download_link%
@echo ------------------------------

git clone %download_link% ptlib
cd "%temp_dir%\ptlib"

::'clone' may not result in fresh state if folder already exists, so try to update it too
git pull
git checkout %repo_version%

@echo ------------------------------
@echo building ptlib
@echo ------------------------------

mkdir "%temp_dir%\ptlib\build" >nul 2>&1
cd "projects/win64/"
cmake . -B"%temp_dir%\ptlib\build" -G"MinGW Makefiles"
cd "%temp_dir%\ptlib\build"
@echo -----
@echo building with %NUMBER_OF_PROCESSORS% threads
@echo -----
mingw32-make -j %NUMBER_OF_PROCESSORS% --output-sync

::dirty, but fast hack to build the complete package with everything that needs the 'ptlib' library dependency ready
::	added the first-build error in ptlib doc as a known issue
mingw32-make -j %NUMBER_OF_PROCESSORS% --output-sync

@echo ------------------------------
@echo setting up ptlib for use
@echo   folder: "%root_directory%\lib\ptlib"
@echo ------------------------------

::NOTE: careful with directory deletion because Batch is abhorrently unpredictable with quotes in variables

::delete the old contents
::@echo DELETING FOLDER: "%root_directory%\lib\ptlib" ...
::rmdir /s "%root_directory%\lib\ptlib" /q

mkdir "%root_directory%\lib" >nul 2>&1
mkdir "%root_directory%\lib\ptlib" >nul 2>&1
mkdir "%root_directory%\lib\ptlib\include" >nul 2>&1
mkdir "%root_directory%\lib\ptlib\include\pt" >nul 2>&1
mkdir "%root_directory%\lib\ptlib\bin" >nul 2>&1

::copy include
xcopy "%temp_dir%\ptlib\include\pt\*" "%root_directory%\lib\ptlib\include\pt" /sy
::copy binaries to lib/
copy "%temp_dir%\ptlib\bin\win64\libptlib.a" "%root_directory%\lib\ptlib\bin\" /y

popd

popd

@echo --------------------------------------------------
@echo on
