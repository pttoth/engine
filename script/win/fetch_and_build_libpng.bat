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
@echo                      libpng16
@echo --------------------------------------------------

set download_link=https://github.com/glennrp/libpng
set zlib_download_link=https://github.com/madler/zlib

set libpng_commit=a37d4836519517bdce6cb9d956092321eca3e73b
set zlib_commit=cacf7f1d4e3d44d871b605da3b647f07d718623f

set target_dir_debug=%root_directory%\bin\%build_platform%_debug
set target_dir_release=%root_directory%\bin\%build_platform%

pushd %temp_dir%

@echo ------------------------------
@echo downloading from
@echo   %download_link%
@echo ------------------------------

::fetch libpng
::git clone --single-branch --branch libpng16 %download_link% libpng
git clone -n %download_link% libpng
cd libpng
git pull
git checkout %libpng_commit%

set libpng_project_path=%temp_dir%\libpng

@echo ------------------------------
@echo fetching and building dependencies
@echo ------------------------------

@echo --------------------
@echo         zlib
@echo --------------------

@echo ----------
@echo downloading from
@echo   %zlib_download_link%
@echo ----------

::fetch and build dependencies
	::zlib
		::fetch
			mkdir "%temp_dir%\libpng\dependencies"
			cd "%temp_dir%\libpng\dependencies"
			git clone -n %zlib_download_link% zlib
			cd zlib
			git pull
			git checkout %zlib_commit%
			set zlib_project_path=%libpng_project_path%\dependencies\zlib
		::build
			@echo ----------
			@echo building zlib
			@echo ----------

			cd "%zlib_project_path%"
			mkdir "%zlib_project_path%\build"
			cmake . -B"%zlib_project_path%\build" -G"MinGW Makefiles"

			cd "%zlib_project_path%\build"
			@echo -----
			@echo building with %NUMBER_OF_PROCESSORS% threads
			@echo -----
			mingw32-make -j %NUMBER_OF_PROCESSORS% --output-sync


@echo ------------------------------
@echo setting up zlib for use
@echo   folder: %root_directory%\lib\zlib
@echo ------------------------------

popd

::delete the old contents
@echo DELETING FOLDER: '%root_directory%\lib\zlib' ...
rmdir /s "%root_directory%\lib\zlib" /q

mkdir "%root_directory%\lib"
mkdir "%root_directory%\lib\zlib"
mkdir "%root_directory%\lib\zlib\bin"

copy "%zlib_project_path%\build\libzlib.dll" "%root_directory%\lib\zlib\bin\libzlib.dll" /y
copy "%zlib_project_path%\build\libzlib.dll.a" "%root_directory%\lib\zlib\bin\libzlib.dll.a" /y
copy "%zlib_project_path%\build\libzlibstatic.a" "%root_directory%\lib\zlib\bin\libzlibstatic.a" /y

copy "%zlib_project_path%\build\libzlib.dll" "%target_dir_debug%\libzlib.dll" /y
copy "%zlib_project_path%\build\libzlib.dll" "%target_dir_release%\libzlib.dll" /y

@echo ------------------------------
@echo building libpng16
@echo ------------------------------

pushd "%libpng_project_path%"
mkdir "%libpng_project_path%\build"
cmake . -B"%libpng_project_path%\build" -DZLIB_LIBRARY:FILEPATH="%zlib_project_path%\build\libzlib.dll" -DZLIB_INCLUDE_DIR:PATH="%zlib_project_path%\build" -G"MinGW Makefiles"

@echo -----
@echo building with %NUMBER_OF_PROCESSORS% threads
@echo -----

cd "%libpng_project_path%\build"
mingw32-make -j %NUMBER_OF_PROCESSORS% --output-sync
popd


::careful, when modifying this!! (the folder is deleted without prompts)
::  'set' mustn't have quotes in value
::  'set' handles spaces correctly
set libpng_target_dir=%root_directory%\lib\libpng16

::remove quotes (jesus!)
::set libpng_target_dir=%libpng_target_dir:"=%

@echo ------------------------------
@echo setting up libpng for use
@echo   folder: %root_directory%\lib\libpng16
@echo ------------------------------

::NOTE: careful with directory deletion because Batch is abhorrently unpredictable with quotes and spaces in variables

::delete the old contents
::@echo DELETING FOLDER: "%libpng_target_dir%" ...
::rmdir /s "%libpng_target_dir%" /q

::set up directories
mkdir "%root_directory%\lib"
mkdir "%libpng_target_dir%"
mkdir "%libpng_target_dir%\bin"
mkdir "%target_dir_debug%"
mkdir "%target_dir_release%"

mkdir "%libpng_target_dir%\include"
mkdir "%libpng_target_dir%\include\libpng"
mkdir "%libpng_target_dir%\include\libpng\contrib"
mkdir "%libpng_target_dir%\include\libpng\contrib\gregbook"
mkdir "%libpng_target_dir%\include\libpng\contrib\libtests"
mkdir "%libpng_target_dir%\include\libpng\contrib\pngminim"
mkdir "%libpng_target_dir%\include\libpng\contrib\pngminim\decoder"
mkdir "%libpng_target_dir%\include\libpng\contrib\pngminim\encoder"
mkdir "%libpng_target_dir%\include\libpng\contrib\pngminim\preader"
mkdir "%libpng_target_dir%\include\libpng\contrib\tools"
mkdir "%libpng_target_dir%\include\libpng\contrib\visupng"
::copy headers
copy "%temp_dir%\libpng\png.h" "%libpng_target_dir%\include\libpng\png.h"
copy "%temp_dir%\libpng\pngconf.h" "%libpng_target_dir%\include\libpng\pngconf.h"
copy "%temp_dir%\libpng\pngdebug.h" "%libpng_target_dir%\include\libpng\pngdebug.h"
copy "%temp_dir%\libpng\pnginfo.h" "%libpng_target_dir%\include\libpng\pnginfo.h"
copy "%temp_dir%\libpng\build\pnglibconf.h" "%libpng_target_dir%\include\libpng\pnglibconf.h"
copy "%temp_dir%\libpng\pngpriv.h" "%libpng_target_dir%\include\libpng\pngpriv.h"
copy "%temp_dir%\libpng\pngstruct.h" "%libpng_target_dir%\include\libpng\pngstruct.h"
copy "%temp_dir%\libpng\contrib\gregbook\readpng.h" "%libpng_target_dir%\include\libpng\contrib\gregbook\readpng.h"
copy "%temp_dir%\libpng\contrib\gregbook\readpng2.h" "%libpng_target_dir%\include\libpng\contrib\gregbook\readpng2.h"
copy "%temp_dir%\libpng\contrib\gregbook\writepng.h" "%libpng_target_dir%\include\libpng\contrib\gregbook\writepng.h"
copy "%temp_dir%\libpng\contrib\libtests\pngstest-errors.h" "%libpng_target_dir%\include\libpng\contrib\libtests\pngstest-errors.h"
copy "%temp_dir%\libpng\contrib\pngminim\decoder\pngusr.h" "%libpng_target_dir%\include\libpng\contrib\pngminim\decoder\pngusr.h"
copy "%temp_dir%\libpng\contrib\pngminim\encoder\pngusr.h" "%libpng_target_dir%\include\libpng\contrib\pngminim\encoder\pngusr.h"
copy "%temp_dir%\libpng\contrib\pngminim\preader\pngusr.h" "%libpng_target_dir%\include\libpng\contrib\pngminim\preader\pngusr.h"
copy "%temp_dir%\libpng\contrib\tools\sRGB.h" "%libpng_target_dir%\include\libpng\contrib\tools\sRGB.h"
copy "%temp_dir%\libpng\contrib\visupng\cexcept.h" "%libpng_target_dir%\include\libpng\contrib\visupng\cexcept.h"
copy "%temp_dir%\libpng\contrib\visupng\PngFile.h" "%libpng_target_dir%\include\libpng\contrib\visupng\PngFile.h"
copy "%temp_dir%\libpng\contrib\visupng\resource.h" "%libpng_target_dir%\include\libpng\contrib\visupng\resource.h"
::copy binaries
copy "%temp_dir%\libpng\build\libpng.a" "%libpng_target_dir%\bin\libpng.a"
copy "%temp_dir%\libpng\build\libpng.dll.a" "%libpng_target_dir%\bin\libpng.dll.a"
copy "%temp_dir%\libpng\build\libpng16.a" "%libpng_target_dir%\bin\libpng16.a"
copy "%temp_dir%\libpng\build\libpng16.dll.a" "%libpng_target_dir%\bin\libpng16.dll.a"

copy "%temp_dir%\libpng\build\libpng16.dll" "%libpng_target_dir%\bin\libpng16.dll"

copy "%temp_dir%\libpng\build\libpng16.dll" "%target_dir_debug%\libpng16.dll"
copy "%temp_dir%\libpng\build\libpng16.dll" "%target_dir_release%\libpng16.dll"

popd

@echo --------------------------------------------------

@echo on
::pause
::copy binaries to ../lib
