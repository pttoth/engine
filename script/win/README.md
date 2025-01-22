# Setup Windows build environment

### Deprecation notice

This will be replaced to a Debian -> Windows cross-compilation environment using WSL in the near future for the following reasons:
- MinGW is not maintained beyond 8.1.0 on Windows by trustworthy sources
- manual setup for toolchain installation is not reliable enough
- automated portable setup is a tricky workaround rather than a neat solution
- Batch and Powershell languages are abhorrently bad/slow/stupid, better ones are not natively supported
- unreliable dependency providers

### WARNING:

Some compilers don't always take whitespace or Unicode characters in file paths. Because of this, not all scripts will work with Windows usernames (__C:\Users\\\<username>__) that contain whitespace or Unicode characters.
Renaming users won't rename their folders, so I recommend reinstalling your entire system with an ASCII username AND without whitespaces if that system is intended for any kind of program development!

## Dependencies

The project toolchain consists of the following tools and dependencies.

### Tools

- 7zr (A lightweight 7-zip-only extractor)
- Git for Windows portable (git, curl)
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

The toolchain setup is fully automated, it requires no installation, it is created in a fully portable way.

For a system-wide toolchain install, use this [guide](systemwide_install/README.md)

## Older systems

__Windows 7__: use the instructions [here](systemwide_install/win7/README.md).

__Windows 8__: upgrade your OS.

## Windows 10 - 11

### Download and set up toolchain
- run `script/win/setup_toolchain.bat`
  + This will create a 'build_toolset' directory in your project root. All tools used by the project will be inside. No installations.
### Download and build dependency libraries
- run `script/win/fetch_and_build_all.bat`
  + This will populate the `lib/` and `bin/` directories with the required .lib-s and .dll-s
- (optional) clean up temporary build files
  + remove the `pt_install` folder in `%LocalAppData%\Temp`
  + keep it, if you frequently want to rebuild the dependencies for some reason
### Build project
- run `script/build_win.sh` or `script/build_win_debug.sh`
- the built binaries can be found in `bin/win64/` or `bin/win64_debug/`
- use the startup script `start_OpenGL_test.bat` next to the binary
  + ensures correct working directory and PATH variable during runtime
