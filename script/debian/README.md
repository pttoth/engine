# Setup Debian build environment

## Dependencies

This tutorial will guide you through installing the following tools and compiling the following libraries.

### Tools

#### Apt package dependencies
- git
- cmake
- build-essential
- libglew-dev
- libsdl2-dev
- libpng-dev
- libassimp-dev

### Additional libraries
- ptlib (https://github.com/pttoth/ptlib)
- math3d (https://github.com/pttoth/math3d)

# Setup steps

- install system dependencies
  * (sudo) run `script/debian/install_dependencies.sh`

- download and build project dependencies
  * run `script/debian/obtain_math3d.sh`
  * run `script/debian/obtain_ptlib.sh`

- build project
  * run `script/build_debian.sh` or `script/build_debian_debug.sh`

- the built binaries can be found in `bin/debian/` or `bin/debian_debug/`
- use the startup script `start_OpenGL_test.sh` next to the binary (ensures correct working directory during runtime)
