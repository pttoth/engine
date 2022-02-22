
@echo off

@echo --------------------------------------------------
@echo                   libjpeg-turbo
@echo --------------------------------------------------

call .\setenv.bat

pushd %~dp0
set libjpeg_turbo_build_script_dir=%~dp0

mkdir temp
cd temp

@echo ------------------------------
@echo downloading from
@echo https://github.com/libjpeg-turbo/libjpeg-turbo
@echo ------------------------------
git clone https://github.com/libjpeg-turbo/libjpeg-turbo

@echo ------------------------------
@echo building libjpeg-turbo
@echo ------------------------------

cd libjpeg-turbo
cmake -G"MinGW Makefiles"
make -j 8 --output-sync

cd ..\..
@echo ------------------------------
@echo copying libpng16 libraries to
@echo ./lib/libpng16
@echo ------------------------------

mkdir ..\lib
mkdir ..\lib\libjpeg-turbo
copy temp\libjpeg-turbo\libjpeg.a ..\lib\libjpeg-turbo\ /y
copy temp\libjpeg-turbo\libjpeg.dll.a ..\lib\libjpeg-turbo\ /y
copy temp\libjpeg-turbo\libjpeg-62.dll ..\lib\libjpeg-turbo\ /y
copy temp\libjpeg-turbo\libturbojpeg.a ..\lib\libjpeg-turbo\ /y
copy temp\libjpeg-turbo\libturbojpeg.dll ..\lib\libjpeg-turbo\ /y
copy temp\libjpeg-turbo\libturbojpeg.dll.a ..\lib\libjpeg-turbo\ /y

popd

@echo --------------------------------------------------
::pause
