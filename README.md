# 3D Game Engine

A hobby game engine project for learning and testing out ideas in OpenGL, and game and engine design.

# Previews (contains third-party assets)

[View in HD on Youtube](https://www.youtube.com/playlist?list=PL_6vYfViRgM7shw6Fb5T2fc9Z8cZAG7DK)

https://github.com/user-attachments/assets/b34cf6e7-8ec5-4456-bfd4-617cf3d102dc

https://github.com/user-attachments/assets/4cabe620-3647-40d2-bee5-a675dcdf6745

https://github.com/user-attachments/assets/5ba26a74-a279-41b5-ab5a-f5ba09f3dbb9

https://github.com/user-attachments/assets/125eb950-52fe-4fae-84d0-2b297fba488c

## Releases
### v0.5.2 (latest)
- [Windows](https://drive.proton.me/urls/ZQJ2KZVGX0#ExbAZfNXqrTT)

## Dependencies
  * SDL2 | OpenGL 3.3 | Assimp | libPNG

## Features

- Multiplatform - Windows / Linux (Debian) 
  * Currently uses OpenGL 3.3, which will be switched up to 4.3, possibly even 4.6.
- Fully open-source (all features and all dependencies)
- Development design is programmer-oriented, not GUI-heavy.
- Configurable Assimp import rules per model
  * Assimp import flags are configurable in meta files ("< filename >.assimp.txt") next to models in directory
- Support for GLTF model format
  * Engine is designed for GLTF, but with Assimp, it can load almost anything.
  * There's also a "sort-of" support for MD5 format just to support idtech4 engine's models.
  * Some FBX support may(!) be added later (proper support conflicts with 'open-source-only' design).
- Support for PNG image format
- Hierarchical Actor and Component systems
- Skybox (skydome)
- Basic dynamic lighting

### Planned features

- Support for TGA image format
- Shadow Mapping
- Support for online multiplayer connections
- Motion Blur
- Audio support
- Animations
- Adding a Valve .vmf file parser to more-or-less support Hammer Editor
- Integrating Newton Dynamics physics engine
- Scripting support in AngelScript
- Performance profiling code tools
- Particle systems
- Support for XBox/PlayStation controllers
- Support for Valve Index and Knuckles
- Add custom memory manager
  * Support for frame-time temporary memory
  * Prealloc and keep chunks to avoid repeated system calls for (re)allocations
- Support for multithreaded execution

## Setting up project
### Acquire the media assets
 - download [media_v0.5.0.zip](https://drive.proton.me/urls/ZVFAK2V5FR#jjY9o83RMyUL)
 - merge the extracted 'media/' folder with the one found in the project's root directory

### Debian

- Follow this [guide](script/debian/README.md)

### Windows

- Follow this [guide](script/win/README.md)
