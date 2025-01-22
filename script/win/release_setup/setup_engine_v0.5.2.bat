@echo off

pushd %~dp0

set media_zipname=media_v0.5.2.zip
set engine_link=https://github.com/pttoth/engine

:: set up tool dir in %tmp%
set setup_dir=pt_engine_setup

set gitexecutable="%tmp%\%setup_dir%\PortableGit\bin\git.exe"

set git_link=https://github.com/git-for-windows/git/releases/download/v2.47.1.windows.2/PortableGit-2.47.1.2-64-bit.7z.exe
set git_zipname=PortableGit-2.47.1.2-64-bit.7z.exe

:: figure out timestamp
for /f "delims=" %%A in ('powershell -command "Get-Date -Format yyMMdd_HHmmss"') do set timestamp=%%A
::echo %timestamp%


:: check media zip next to script
if not exist "%media_zipname%" (
    echo Could not find '%media_zipname%'
    start "" "https://drive.proton.me/urls/JRRD1QYG1M#MLzdrZ3h4qNn"

    echo Download '%media_zipname%', place it next to the script file, then run the script again

    goto end_segment
) else (
    echo Found '%media_zipname%'
)


:: check for Git
git --version >nul 2>&1
IF ERRORLEVEL 1 (
    echo Could not find Git on system

    if not exist "%gitexecutable%" (
        @echo off
        echo Downloading PortableGit
        pushd %tmp%
        if exist .\%setup_dir% (
            rd /s /q .\%setup_dir%
        )
        mkdir %setup_dir%
        cd %setup_dir%

        ::get PortableGit
        echo "downloading 7zip at 'https://7-zip.org/a/7zr.exe' ..."
        powershell -command "(new-object System.Net.WebClient).DownloadFile( 'https://7-zip.org/a/7zr.exe', '7zr.exe' )"

        ::PortableGit
        mkdir PortableGit
        cd PortableGit
        echo "downloading PortableGit from '%git_link%' ..."
        powershell -command "(new-object System.Net.WebClient).DownloadFile( '%git_link%', '%git_zipname%' )"
        echo "extracting  PortableGit '%git_zipname%' ..."
        ..\7zr.exe x %git_zipname% > nul
        del %git_zipname%
        cd ..

        :: back to initial dir
        popd
    ) else (
        echo ... but found PortableGit
    )

) else (
    echo Found Git
    set gitexecutable=git
)


rename engine engine_bak_%timestamp% > nul 2>&1
"%gitexecutable%" clone %engine_link% engine
pushd engine
"%gitexecutable%" checkout v0.5.2

:: delete the temporary PortableGit instance, that cloned 'engine'
pushd %tmp%
::rd /s /q .\pt_engine_setup
popd

call .\script\win\setup_toolchain.bat
call .\script\win\fetch_and_build_all.bat
call .\script\build_win.bat

popd
:: now in script dir

powershell -command "(Expand-Archive -Force '%media_zipname%' engine/)"

call .\engine\bin\win64\start_OpenGL_test.bat

popd
:: now in initial dir

:: 'goto' bookmark
:end_segment
pause

@echo on
