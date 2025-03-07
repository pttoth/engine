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
@echo                     glew 2.1.0
@echo --------------------------------------------------

set download_link=https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip
set download_glew_filename=glew-2.1.0.zip
set glew_filename_library_dir=%temp_dir%\glew-2.1.0

set target_dir_debug=%root_directory%\bin\%build_platform%_debug
set target_dir_release=%root_directory%\bin\%build_platform%

pushd "%temp_dir%"

@echo ------------------------------
@echo downloading from
@echo   %download_link%
@echo ------------------------------
::github repo doesnt contain correct sources for some reason
::git clone https://github.com/nigels-com/glew glew

::download Glew (wget - old)
::wget --no-clobber %download_link%

::download Glew
::    use 'curl' in local toolset
::"%root_directory%\build_toolset\win64\PortableGit\mingw64\bin\curl.exe" %download_link% -L -O
::    use 'curl' in system installed toolset
::"C:\Program Files\Git\mingw64\bin\curl.exe" %download_link% -L -O
::    use PowerShell
powershell -command "(new-object System.Net.WebClient).DownloadFile( '%download_link%', '%download_glew_filename%' )"

@echo --------------------
@echo extracting file
@echo   %download_glew_filename%
@echo --------------------
powershell -command "(Expand-Archive -Force '%download_glew_filename%' .)"
::7z x -y "./%download_glew_filename%"

@echo ------------------------------
@echo building glew 2.1.0.
@echo ------------------------------

cd "%glew_filename_library_dir%"
mkdir "%glew_filename_library_dir%\lib" >nul 2>&1

@echo on

gcc -DGLEW_NO_GLU -O2 -Wall -W -Wno-cast-function-type -Iinclude  -DGLEW_BUILD -o src/glew.o -c src/glew.c
gcc -Wno-cast-function-type -nostdlib -shared -Wl,-soname,libglew32.dll -Wl,--out-implib,lib/libglew32.dll.a    -o lib/glew32.dll src/glew.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32
ar cr lib/libglew32.a src/glew.o
gcc -DGLEW_NO_GLU -DGLEW_MX -O2 -Wall -W -Wno-cast-function-type -Iinclude  -DGLEW_BUILD -o src/glew.mx.o -c src/glew.c
gcc -Wno-cast-function-type -nostdlib -shared -Wl,-soname,libglew32mx.dll -Wl,--out-implib,lib/libglew32mx.dll.a -o lib/glew32mx.dll src/glew.mx.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32
ar cr lib/libglew32mx.a src/glew.mx.o

@echo off

popd

@echo ------------------------------
@echo copying glew 2.1.0. libraries to
@echo 	"%root_directory%/lib/glew-2.1.0"
@echo ------------------------------

mkdir "%root_directory%\bin" >nul 2>&1
mkdir "%target_dir_debug%" >nul 2>&1
mkdir "%target_dir_release%" >nul 2>&1

::NOTE: careful with directory deletion because Batch is abhorrently unpredictable with quotes and spaces in variables

::delete the old contents
::@echo DELETING FOLDER: '%root_directory%\lib\glew-2.1.0' ...
::rmdir /s "%root_directory%\lib\glew-2.1.0" /q

mkdir "%root_directory%\lib" >nul 2>&1
mkdir "%root_directory%\lib\glew-2.1.0" >nul 2>&1
mkdir "%root_directory%\lib\glew-2.1.0\include" >nul 2>&1
mkdir "%root_directory%\lib\glew-2.1.0\include\GL" >nul 2>&1
mkdir "%root_directory%\lib\glew-2.1.0\bin" >nul 2>&1
::copy include/
copy "%temp_dir%\glew-2.1.0\include\GL\eglew.h" "%root_directory%\lib\glew-2.1.0\include\GL\eglew.h" /y
copy "%temp_dir%\glew-2.1.0\include\GL\glew.h" "%root_directory%\lib\glew-2.1.0\include\GL\glew.h" /y
copy "%temp_dir%\glew-2.1.0\include\GL\glxew.h" "%root_directory%\lib\glew-2.1.0\include\GL\glxew.h" /y
copy "%temp_dir%\glew-2.1.0\include\GL\wglew.h" "%root_directory%\lib\glew-2.1.0\include\GL\wglew.h" /y
::copy binaries to lib/
copy "%temp_dir%\glew-2.1.0\lib\glew32.dll" "%root_directory%\lib\glew-2.1.0\bin\glew32.dll" /y
copy "%temp_dir%\glew-2.1.0\lib\glew32mx.dll" "%root_directory%\lib\glew-2.1.0\bin\glew32mx.dll" /y
copy "%temp_dir%\glew-2.1.0\lib\libglew32.a" "%root_directory%\lib\glew-2.1.0\bin\libglew32.a" /y
copy "%temp_dir%\glew-2.1.0\lib\libglew32.dll.a" "%root_directory%\lib\glew-2.1.0\bin\libglew32.dll.a" /y
copy "%temp_dir%\glew-2.1.0\lib\libglew32mx.a" "%root_directory%\lib\glew-2.1.0\bin\libglew32mx.a" /y
copy "%temp_dir%\glew-2.1.0\lib\libglew32mx.dll.a" "%root_directory%\lib\glew-2.1.0\bin\libglew32mx.dll.a" /y
::copy binaries to bin/
copy "%temp_dir%\glew-2.1.0\lib\glew32.dll" "%target_dir_debug%\glew32.dll" /y
copy "%temp_dir%\glew-2.1.0\lib\glew32.dll" "%target_dir_release%\glew32.dll" /y

popd

@echo --------------------------------------------------

@echo on
::pause
