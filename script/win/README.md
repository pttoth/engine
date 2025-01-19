# Setup Windows build environment

This will be replaced to a Debian -> Windows cross-compilation environment using WSL in the near future for the following reasons:
- Setting up the Windows environment can't be safely and reliably automated on everyone's system
- Batch and Powershell languages are abhorrently bad, others are not natively supported
- MinGW is not maintained beyond 8.1.0 on Windows
- manual setup is not reliable enough
- unreliable dependency providers

### WARNING:

Some compilers don't always take whitespace or Unicode characters in file paths. Because of this, not all scripts will work with Windows usernames (__C:\Users\\\<username>__) that contain whitespace or Unicode characters.
Renaming users won't rename their folders, so I recommend reinstalling your entire system with an ASCII username AND without whitespaces if that system is intended for any kind of program development!

## Dependencies

This tutorial will guide you through installing the following tools and compiling the following libraries

### Tools

- 7-zip
- Git for Windows (git, curl)
- CMake
- MinGW 8.1.0 toolset (mingw32-make, gcc, g++, gdb, strip, bash interpreter)

### Libraries

- glew
- assimp
- libpng
- zlib
- ptlib (https://github.com/pttoth/ptlib)
- math3d (https://github.com/pttoth/math3d)

# Setup steps

I recommend following these steps to the letter to avoid potential security or functionality issues!

## Older systems

__Windows 7__: use the instructions [here](win7/README.md).

__Windows 8__: upgrade your OS.

## Windows 10 - 11

### OPTIONAL (security): Prepare 'C:/tools' folder
It's important to have __cmake__, __make__ and __mingw__ in a path, which contains no spaces.

These usually install into the `C:\tools` folder by default, BUT this folder is NOT protected by Windows' TrustedInstaller security policy

Recommend:
 - installing them into `C:\Program Files\tools`
 - then creating a junction to that folder as `C:\tools`
   + open cmd __WITH__ admin privileges
   + type:
     * `mkdir "C:\Program Files\tools"`
     * `mklink /D "C:\tools" "C:\Program Files\tools"`

Now you'll have a `C:\tools` folder(symlink), pointing to `C:\Program Files\tools` AND with TrustedInstaller protection.

### Install tools

### Git
- download [here](https://github.com/git-for-windows/git/releases/download/v2.47.1.windows.1/Git-2.47.1-64-bit.exe)
- install under `C:\Program Files\Git`
- during install, make sure it is added to PATH, so the 'git' command is available in the Command Line
  + allows IDE-s to find it automatically
  
### 7-zip
- download [here](https://www.7-zip.org/a/7z2409-x64.exe)
- install under `C:\Program Files\7-Zip`
  + No need to add this folder to PATH, the scripts that need it will do it locally
  + Although, if you used a different install path, than the one above, definitely add it!

### CMake
- download [here](https://github.com/Kitware/CMake/releases/download/v3.31.4/cmake-3.31.4-windows-x86_64.msi)
- install to `C:\tools\CMake`
  + it is important, that this path does not have spaces in it!
- during install, make sure that __"Add CMake to the PATH environment variable"__ is selected
  + allows IDE-s to find it automatically

### MinGW 8.1.0 toolset
- download [here](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z/download)
- open Command Line
  + `mkdir C:\tools\mingw`
  + `mkdir C:\tools\mingw\x86_64-8.1.0-posix-seh`
  + it is important, that this path does not have spaces in it!
- extract the downloaded .7z archive to `C:\tools\mingw\x86_64-8.1.0-posix-seh`
- verify that the following path indeed exists: `C:\tools\mingw\x86_64-8.1.0-posix-seh\mingw64\bin`

- (optional) Add MinGW to your __PATH__ environment variable
  + add `C:\tools\mingw\x86_64-8.1.0-posix-seh\mingw64\bin` to your __PATH__ variable
  + although this step is skippable, it has the following drawbacks
    * the built executables won't be able to find the MinGW toolset dll-s and they will require the startup scripts, that are generated in the same folder
    * IDE-s won't be able to find __mingw32-make__, __gcc__, __g++__, etc. automatically

### IDE of your choice
- [QtCreator](https://download.qt.io/official_releases/qtcreator)

---

### Download and build dependency libraries
- run `script/win/fetch_and_build_all.bat`
  + This will populate the `lib/` and `bin/` directories with the required .lib-s and .dll-s
- (optional) clean up temporary build files
  + remove the `pt_install` folder in `%LocalAppData%\Temp`
### Build project
- run `script/build_win.sh` or `script/build_win_debug.sh`
- the built binaries can be found in `bin/win64/` or `bin/win64_debug/`
- use the startup script `start_OpenGL_test.bat` next to the binary (ensures correct working directory during runtime)
