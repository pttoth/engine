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
@echo                        SDL2
@echo --------------------------------------------------

::Mercurial (old)
::set download_link=https://hg.libsdl.org/SDL

::Git
set download_link=https://github.com/libsdl-org/SDL
set repo_version=release-2.30.5

set target_dir_debug=%root_directory%\bin\%build_platform%_debug
set target_dir_release=%root_directory%\bin\%build_platform%

pushd "%temp_dir%"

@echo ------------------------------
@echo downloading from
@echo   %download_link%
@echo ------------------------------

::Mercurial (old)
::hg clone %download_link% SDL

::Git

set sdl_commit_current=ecd922b5cf9a6acff91f952b8da3068c776e57a1

git clone -n %download_link% SDL
cd SDL
git checkout %sdl_commit_current%
@echo ------------------------------
@echo building SDL2
@echo ------------------------------

mkdir "%temp_dir%\SDL_build"
cd "%temp_dir%\SDL"
cmake . -B"%temp_dir%\SDL_build" -G"MinGW Makefiles"
cd "%temp_dir%\SDL_build"
@echo -----
@echo building with %NUMBER_OF_PROCESSORS% threads
@echo -----
mingw32-make -j %NUMBER_OF_PROCESSORS% --output-sync

@echo ------------------------------
@echo setting up SDL2 for use
@echo   folder: "%root_directory%\lib\SDL2"
@echo ------------------------------

::NOTE: careful with directory deletion because Batch is abhorrently unpredictable with quotes and spaces in variables

::delete the old contents
::@echo DELETING FOLDER: "%root_directory%\lib\SDL2" ...
::rmdir /s "%root_directory%\lib\SDL2" /q

popd

mkdir "%root_directory%\bin"
mkdir "%target_dir_debug%"
mkdir "%target_dir_release%"

mkdir "%root_directory%\lib\"
mkdir "%root_directory%\lib\SDL2"
mkdir "%root_directory%\lib\SDL2\include"
mkdir "%root_directory%\lib\SDL2\include\SDL2"
mkdir "%root_directory%\lib\SDL2\bin"

::copy include
xcopy "%temp_dir%\SDL\include\*.h" "%root_directory%\lib\SDL2\include\SDL2\" /sy
::copy binaries to lib/
copy "%temp_dir%\SDL_build\libSDL2.a" "%root_directory%\lib\SDL2\bin\libSDL2-static.a"
copy "%temp_dir%\SDL_build\libSDL2.dll.a" "%root_directory%\lib\SDL2\bin\libSDL2.dll"
copy "%temp_dir%\SDL_build\libSDL2main.a" "%root_directory%\lib\SDL2\bin\libSDL2main.a"
::copy "%temp_dir%\SDL_build\libSDL2-static.a" "%root_directory%\lib\SDL2\bin\libSDL2-static.a"
copy "%temp_dir%\SDL_build\SDL2.dll" "%root_directory%\lib\SDL2\bin\SDL2.dll"

::copy binaries to bin/
copy "%temp_dir%\SDL_build\SDL2.dll" "%target_dir_debug%\SDL2.dll" /y
copy "%temp_dir%\SDL_build\SDL2.dll" "%target_dir_release%\SDL2.dll" /y



popd

@echo --------------------------------------------------

@echo on
::pause
::copy binaries to ../lib
