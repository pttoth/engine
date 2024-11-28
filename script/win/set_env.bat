@echo off

pushd %~dp0

::deprecated (removed chocolatey-based dependencies)
::set path="C:\Program Files\CMake\bin";C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin;%path%

set win_script_dir=%~dp0
set root_directory=%win_script_dir%\..\..

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

