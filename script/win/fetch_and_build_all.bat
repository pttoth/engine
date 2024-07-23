@echo off
pushd %~dp0
call .\set_env.bat
::gets the following variables
::  win_script_dir
::  root_directory
::  build_platform
::  temp_dir

@echo off

call .\fetch_and_build_SDL.bat
@echo off
call .\fetch_and_build_glew.bat
@echo off
call .\fetch_and_build_libpng.bat
@echo off
::call .\fetch_and_build_libjpeg-turbo.bat
@echo off
call .\fetch_and_build_assimp.bat
@echo off
call .\fetch_and_build_math3d.bat
@echo off
call .\fetch_and_build_ptlib.bat
@echo off

::open explorer at the build target
pushd "%root_directory%\bin\%build_platform%"
explorer .
popd
::because this doesn't work, because why would anything work on Windows...
::explorer "%root_directory%\bin\%build_platform%"


popd
@echo on
