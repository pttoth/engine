@echo off

pushd %~dp0

set win_script_dir=%~dp0
set script_dir=%win_script_dir%\..
set root_directory=%win_script_dir%\..\..

::--------------------------------------------------
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
::--------------------------------------------------

if not defined pt_path_set (
    set pt_path_set=0
)

REM ::NOTE - Win 11 compatibility
REM ::    Take care, that these variables don't contain parentheses, specifically ')'
REM ::    Win 11 batch will interpret it as the closing ')' of the 'if' block, where they will be dereferred... -.-
REM ::    Also don't refer to PATH, because it will most likely contain 'Program Files (x86)'
set path_engine_sys_minimal=C:\Windows\system32;C:\Windows\system32\Windows;C:\Windows\system32\WindowsPowerShell\v1.0
set path_engine_sys_tools=C:\tools\mingw\x86_64-8.1.0-posix-seh\mingw64\bin;C:\Program Files\Git\bin;C:\tools\CMake\bin;C:\Program Files\7-Zip
set path_engine_local_toolset=%root_directory%\build_toolset\win64\mingw\mingw64\bin;%root_directory%\build_toolset\win64\cmake\bin;%root_directory%\build_toolset\win64\PortableGit\bin

if not "%pt_path_set%"=="1" (
	set PATH=%path_engine_local_toolset%;%path_engine_sys_tools%;%path_engine_sys_minimal%
    set pt_path_set=1
)

set build_platform=win64

::set temp_dir=%root_directory%\install\win\temp
::set temp_dir=%userprofile%\AppData\Local\Temp\pt_install
::set temp_dir=%LocalAppData%\Temp\pt_install
set temp_dir=%temp%\pt_install

::remove quotes from variable
::set temp_dir=%temp_dir:"=%

@echo win_script_dir   = %win_script_dir%
@echo script_dir       = %script_dir%
@echo root_directory   = %root_directory%
@echo temp_dir         = %temp_dir%
@echo build_platform   = %build_platform%
@echo PATH             = %PATH%

mkdir "%temp_dir%"

popd

@echo on
