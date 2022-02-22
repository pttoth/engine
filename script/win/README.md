# Windows Install Scripts

Scripts to install project dependencies automatically.

See the following files for directions:

- '1_manual_dl_dependency_installers.txt'
- '2_manual_install_dependencies.txt'


#### WARNING:

Some compilers don't always take Unicode or space characters in file paths. Because of this, not all scripts will work with Windows usernames (_C:\Users\\\<username>_) that contain Unicode characters.
Renaming users won't rename their folders, so I recommend reinstalling your entire system with an ASCII username AND without spaces if that system is intended for any kind of program development!

## Details

#### The project depends on the following packages:

- glew
- assimp (custom source: https://github.com/pttoth/assimp)
- libpng
- zlib
- ptlib (https://github.com/pttoth/ptlib)
- math3d (https://github.com/pttoth/math3d)

#### The build environment consists of:

- MinGW 8.1.0 (will soon be updated to newer)
- cmake
- make (mingw32-make, part of the MinGW 8.1.0 package)
- git

#### Script dependencies

The following dependencies have to be available in the PATH environment variable for the scripts to run correctly:

- curl
- 7-zip
- git
- cmake
- make
- g++
