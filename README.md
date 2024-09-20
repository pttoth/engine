# 3D Game Engine

A hobby game engine project for learning and testing out ideas in OpenGL, and game and engine design.

### Dependencies
  * SDL2 | OpenGL 3.3 | Assimp | libPNG

## Features

- Multiplatform - Windows / Linux (Debian) 
  * OpenGL 3.3 graphics for compatibility reasons.
  * Will be moved up to 4.3, possibly even 4.6.

- Fully open-source (all features and all dependencies)
- Development design is programmer-oriented, not GUI-heavy.
- configurable Assimp import rules per model
  * Assimp import flags are configurable in meta files ("< filename >.assimp.txt") next to models in directory

- Support for GLTF model format
  * Engine is designed for GLTF, but with Assimp, it can load almost anything.
  * There's also a "sort-of" support for MD5 format just to support idtech4 engine's formats.
  * Some FBX support may(!) be added later (proper support conflicts with 'open-source-only' design).

- support for PNG image format
  * PLANNED - support for TGA.

- hierarchical Actor and Component systems
- Skybox (skydome)
- Basic dynamic lighting
- WIP - Shadow Mapping
- PLANNED - support for online multiplayer connections
- PLANNED - Motion Blur
- PLANNED - Audio support
- PLANNED - Animations
- PLANNED - parsing Valve Hammer .vmf files to quickly have an Editor
- PLANNED - Newton Dynamics physics engine
- PLANNED - scripting support in AngelScript
- PLANNED - performance profiling code tools
- PLANNED - particle systems
- PLANNED - support for XBox/PlayStation controllers
- PLANNED - support for Valve Index and Knuckles
- PLANNED - advanced memory management
  * Support for frame-time temporary memory
  * avoiding repeated system calls for (re)allocations

- PLANNED - support for multithreaded execution

## Setting up project

## Legal
