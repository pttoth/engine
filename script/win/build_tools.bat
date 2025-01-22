@echo off

pushd %~dp0
call .\set_env.bat

@echo off

set git_link=https://github.com/git-for-windows/git/releases/download/v2.47.1.windows.2/PortableGit-2.47.1.2-64-bit.7z.exe
set git_zipname=PortableGit-2.47.1.2-64-bit.7z.exe

set cmake_link=https://github.com/Kitware/CMake/releases/download/v3.31.4/cmake-3.31.4-windows-x86_64.zip
set cmake_zipname=cmake-3.31.4-windows-x86_64.zip
set cmake_foldername=cmake-3.31.4-windows-x86_64

::original link (without '%' switched to '%%')
::set mingw_link=https://downloads.sourceforge.net/project/mingw-w64/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z
set mingw_link=https://downloads.sourceforge.net/project/mingw-w64/Toolchains%%20targetting%%20Win64/Personal%%20Builds/mingw-builds/8.1.0/threads-posix/seh/x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z
set mingw_zipname=x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z

set toolset_name=win64

:: figure out timestamp
for /f "delims=" %%A in ('powershell -command "Get-Date -Format yyMMdd_HHmmss"') do set timestamp=%%A
::echo %timestamp%


mkdir "%root_directory%\build_toolset" >nul 2>&1

pushd "%root_directory%\build_toolset"

if exist "%toolset_name%" (
    rename "%toolset_name%" "%toolset_name%_bak_%timestamp%"
)

mkdir "%toolset_name%" >nul 2>&1
pushd "%toolset_name%"

@echo "downloading 7zip at 'https://7-zip.org/a/7zr.exe' ..."
powershell -command "(new-object System.Net.WebClient).DownloadFile( 'https://7-zip.org/a/7zr.exe', '7zr.exe' )"

::PortableGit
mkdir PortableGit >nul 2>&1
cd PortableGit
@echo "downloading PortableGit from '%git_link%' ..."
powershell -command "(new-object System.Net.WebClient).DownloadFile( '%git_link%', '%git_zipname%' )"
@echo "extracting  PortableGit '%git_zipname%' ..."
..\7zr.exe x %git_zipname% > nul
del %git_zipname%
cd ..

::CMake
@echo "downloading CMake from '%cmake_link%' ..."
powershell -command "(new-object System.Net.WebClient).DownloadFile( '%cmake_link%', '%cmake_zipname%' )"
@echo "extracting  CMake '%cmake_zipname%' ..."
powershell -command "(Expand-Archive -Force '%cmake_zipname%' .)"
rename %cmake_foldername% cmake
del %cmake_zipname%

::MinGW
mkdir mingw >nul 2>&1
cd mingw
@echo "downloading MinGW from '%mingw_link%' ..."
powershell -command "(new-object System.Net.WebClient).DownloadFile( '%mingw_link%', '%mingw_zipname%' )"
@echo "extracting  MinGW: '%mingw_zipname%' ..."
..\7zr.exe x %mingw_zipname% > nul
del %mingw_zipname%
cd ..

del .\7zr.exe

popd
::now at 'build_toolset\'

popd
::now at 'script\win\'

popd
::now at initial dir

@echo done

@echo on
