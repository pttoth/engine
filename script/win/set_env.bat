@echo off

pushd %~dp0

set win_script_dir=%~dp0
set root_directory=%win_script_dir%\..\..

:: NOTE: the following files are both present in MinGW-8.1.0 and Git-s own MinGW environment
::  libgcc_s_seh-1.dll
::  libssp-0.dll
::  libstdc++-6.dll
::  libwinpthread-1.dll
:: Paths:
::  C:\tools\mingw\x86_64-8.1.0-posix-seh\mingw64\bin
::  C:\Program Files\Git\mingw64\bin

::  Git's MinGW environment is currently only used by the project to access 'curl'
::  There probably won't be any more conflicts as the windows build env is moving to a cross-build Docker container soon
set PATH="C:\tools\mingw\x86_64-8.1.0-posix-seh\mingw64\bin;C:\Program Files\Git\bin;C:\tools\CMake\bin;C:\Program Files\7-Zip;%PATH%"

set build_platform=win64

::set temp_dir=%root_directory%\install\win\temp
::set temp_dir=%userprofile%\AppData\Local\Temp\pt_install
::set temp_dir=%LocalAppData%\Temp\pt_install
set temp_dir=%temp%\pt_install

::remove quotes from variable
::set temp_dir=%temp_dir:"=%

@echo win_script_dir   = %win_script_dir%
@echo root_directory   = %root_directory%
@echo temp_dir         = %temp_dir%
@echo build_platform   = %build_platform%

mkdir "%temp_dir%"

popd

@echo on

