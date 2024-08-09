cmake_minimum_required(VERSION 3.5)

project(indicusengine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_BUILD_PARALLEL_LEVEL $ENV{$(nproc)})
#set(CMAKE_BUILD_PARALLEL_LEVEL 16)

set(MY_PROJ_ROOT ${PROJECT_SOURCE_DIR}/../..)
set(MY_BUILD_DIR ${MY_PROJ_ROOT}/build)
set(MY_OUTPUT_DIR_WIN32 ${MY_PROJ_ROOT}/bin/win32)
set(MY_OUTPUT_DIR_WIN64 ${MY_PROJ_ROOT}/bin/win64)
set(MY_OUTPUT_DIR_DEBIAN ${MY_PROJ_ROOT}/bin/debian)

#set up actual output directory
set(MY_OUTPUT_DIR ${MY_OUTPUT_DIR_DEBIAN})
set(MY_OUTPUT_DIR_DEBUG ${MY_OUTPUT_DIR}_debug)

#default output directory (when building from cmd with everything default)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MY_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MY_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MY_OUTPUT_DIR})

#release build directory settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MY_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${MY_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${MY_OUTPUT_DIR})

#debug build directory settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${MY_OUTPUT_DIR_DEBUG})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${MY_OUTPUT_DIR_DEBUG})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${MY_OUTPUT_DIR_DEBUG})

set(CMAKE_CACHEFILE_DIR ${MY_BUILD_DIR})

#macros inherited from 'ptlib':
#  PT_DEBUG_ENABLED:
#    enables additional debug logic and verbose logging
#    !! not having 'NDEBUG' defined will enable 'PT_DEBUG_ENABLED' !!
#  PT_DEBUG_NOAUTO:
#    prevents auto-defining 'PT_DEBUG_ENABLED' based on 'NDEBUG'
#project-defined macros:
#  ENGINE_DEBUG_ENABLED
#    enables additional debug logic and verbose logging
#    !! not having 'NDEBUG' defined will enable 'ENGINE_DEBUG_ENABLED' !!
#  ENGINE_DEBUG_NOAUTO:
#    prevents auto-defining debug macros based on 'NDEBUG'
#  ENGINE_GL_DEBUG_ALL:     verbose debug logging for all gl:: classes
#  ENGINE_GL_DEBUG_BUFFER:  verbose debug logging for gl::Buffer class
#  ENGINE_GL_DEBUG_BUFFER_SKIP_DYNAMIC:  disable per-frame logging of 'DYNAMIC_*' hinted buffers
#  ENGINE_GL_DEBUG_BUFFER_SKIP_STREAM:   disable per-frame logging of 'STREAM_*' hinted buffers
#  ENGINE_GL_UNBIND_ENABLED: Enables the macro-functions 'PT_GL_UnbindBuffer' and 'PT_GL_UnbindTexture'
set(MY_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wl,--no-as-needed -lpthread -ldl")
set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -g -O0 -fno-inline")
#set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -DPT_DEBUG_NOAUTO")
#set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -DENGINE_DEBUG_ENABLED -DENGINE_GL_DEBUG_ALL")
set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -DENGINE_GL_DEBUG_BUFFER_SKIP_STREAM ")
#set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -DENGINE_GL_DEBUG_BUFFER_SKIP_DYNAMIC")
#set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -DMATH_ENABLE_SWIZZLE ")

set(CMAKE_C_FLAGS ${MY_COMPILE_FLAGS})
set(CMAKE_CXX_FLAGS ${MY_COMPILE_FLAGS})

#--------------------------------------------------
# engine library project setup
#--------------------------------------------------

#all generic engine source files go in this variable
set(INDICUS_ENGINE_SOURCE_FILES
    ${MY_PROJ_ROOT}/src/algo/BFS.cpp
    ${MY_PROJ_ROOT}/src/algo/BFSNode.cpp
    ${MY_PROJ_ROOT}/src/algo/Edge.cpp
    ${MY_PROJ_ROOT}/src/algo/Graph.cpp
    ${MY_PROJ_ROOT}/src/algo/Node.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/AssimpConfig.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/Def.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/GlWrapper.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/Material.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/MaterialConfig.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/Mesh.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/Shader.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/ShaderProgram.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/Texture2d.cpp
    #${MY_PROJ_ROOT}/src/engine/gl/UniformBlock.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/UniformBlockFrameInfo.cpp
    ${MY_PROJ_ROOT}/src/engine/gl/UniformBlockLightingInfo.cpp
    ${MY_PROJ_ROOT}/src/engine/actor/Actor.cpp
    ${MY_PROJ_ROOT}/src/engine/actor/Camera.cpp
    ${MY_PROJ_ROOT}/src/engine/actor/CameraOrtographic.cpp
    ${MY_PROJ_ROOT}/src/engine/actor/CameraPerspective.cpp
    #${MY_PROJ_ROOT}/src/engine/actor/Light.cpp
    #${MY_PROJ_ROOT}/src/engine/actor/LightAmbient.cpp
    ${MY_PROJ_ROOT}/src/engine/actor/LightCone.cpp
    ${MY_PROJ_ROOT}/src/engine/component/AxisDisplayComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/component/BillboardComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/component/Component.cpp
    ${MY_PROJ_ROOT}/src/engine/component/LightConeComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/component/MeshComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/component/PositionComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/component/RealComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/component/WorldComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/experimental/SDLBillboardComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/experimental/SDLComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/experimental/SphereComponent.cpp
    ${MY_PROJ_ROOT}/src/engine/experimental/World.cpp
    ${MY_PROJ_ROOT}/src/engine/service/AssetManager.cpp
    ${MY_PROJ_ROOT}/src/engine/service/DrawingManager.cpp
    ${MY_PROJ_ROOT}/src/engine/service/SDLManager.cpp
    ${MY_PROJ_ROOT}/src/engine/service/SerialScheduler.cpp
    ${MY_PROJ_ROOT}/src/engine/service/SystemManager.cpp
    ${MY_PROJ_ROOT}/src/engine/Engine.cpp
    ${MY_PROJ_ROOT}/src/engine/MeshLoader.cpp
    #${MY_PROJ_ROOT}/src/engine/Scene.cpp
    ${MY_PROJ_ROOT}/src/engine/SDLApplication.cpp
    ${MY_PROJ_ROOT}/src/engine/Services.cpp
    ${MY_PROJ_ROOT}/src/engine/StandardShaderProgram.cpp
    ${MY_PROJ_ROOT}/src/engine/Utility.cpp
    ${MY_PROJ_ROOT}/src/SDLWrapper.cpp
    #${MY_PROJ_ROOT}/src/ShaderProgram_old.cpp

    #${MY_PROJ_ROOT}/src/test/assimp_test/Game.cpp
    )

#all generic engine header files go in this variable
set(INDICUS_ENGINE_HEADER_FILES
    ${MY_PROJ_ROOT}/include/algo/BFS.h
    ${MY_PROJ_ROOT}/include/algo/BFSNode.h
    ${MY_PROJ_ROOT}/include/algo/Edge.h
    ${MY_PROJ_ROOT}/include/algo/Graph.h
    ${MY_PROJ_ROOT}/include/algo/Node.h
    ${MY_PROJ_ROOT}/include/engine/gl/AssimpConfig.h
    ${MY_PROJ_ROOT}/include/engine/gl/Buffer.hpp
    ${MY_PROJ_ROOT}/include/engine/gl/Def.h
    ${MY_PROJ_ROOT}/include/engine/gl/GlWrapper.h
    ${MY_PROJ_ROOT}/include/engine/gl/Material.h
    ${MY_PROJ_ROOT}/include/engine/gl/MaterialConfig.h
    ${MY_PROJ_ROOT}/include/engine/gl/Mesh.h
    ${MY_PROJ_ROOT}/include/engine/gl/Shader.h
    ${MY_PROJ_ROOT}/include/engine/gl/ShaderProgram.h
    ${MY_PROJ_ROOT}/include/engine/gl/Texture2d.h
    ${MY_PROJ_ROOT}/include/engine/gl/Uniform.hpp
    #${MY_PROJ_ROOT}/include/engine/gl/UniformBlock.h
    ${MY_PROJ_ROOT}/include/engine/gl/UniformBlockFrameInfo.h
    ${MY_PROJ_ROOT}/include/engine/gl/UniformBlockLightingInfo.h
    ${MY_PROJ_ROOT}/include/engine/actor/Actor.h
    ${MY_PROJ_ROOT}/include/engine/actor/Camera.h
    ${MY_PROJ_ROOT}/include/engine/actor/CameraOrtographic.h
    ${MY_PROJ_ROOT}/include/engine/actor/CameraPerspective.h
    #${MY_PROJ_ROOT}/include/engine/actor/Light.h
    #${MY_PROJ_ROOT}/include/engine/actor/LightAmbient.h
    ${MY_PROJ_ROOT}/include/engine/actor/LightCone.h
    ${MY_PROJ_ROOT}/include/engine/component/AxisDisplayComponent.h
    ${MY_PROJ_ROOT}/include/engine/component/BillboardComponent.h
    ${MY_PROJ_ROOT}/include/engine/component/Component.h
    ${MY_PROJ_ROOT}/include/engine/component/LightConeComponent.h
    ${MY_PROJ_ROOT}/include/engine/component/MeshComponent.h
    ${MY_PROJ_ROOT}/include/engine/component/PositionComponent.h
    ${MY_PROJ_ROOT}/include/engine/component/RealComponent.h
    ${MY_PROJ_ROOT}/include/engine/component/WorldComponent.h
    ${MY_PROJ_ROOT}/include/engine/experimental/Networking.h
    ${MY_PROJ_ROOT}/include/engine/experimental/SDLBillboardComponent.h
    ${MY_PROJ_ROOT}/include/engine/experimental/SDLComponent.h
    ${MY_PROJ_ROOT}/include/engine/experimental/SphereComponent.h
    ${MY_PROJ_ROOT}/include/engine/experimental/World.h
    ${MY_PROJ_ROOT}/include/engine/service/AssetControl.h
    ${MY_PROJ_ROOT}/include/engine/service/AssetManager.h
    ${MY_PROJ_ROOT}/include/engine/service/DrawingControl.h
    ${MY_PROJ_ROOT}/include/engine/service/DrawingManager.h
    ${MY_PROJ_ROOT}/include/engine/service/EngineControl.h
    ${MY_PROJ_ROOT}/include/engine/service/Scheduler.h
    ${MY_PROJ_ROOT}/include/engine/service/SDLControl.h
    ${MY_PROJ_ROOT}/include/engine/service/SDLManager.h
    ${MY_PROJ_ROOT}/include/engine/service/SerialScheduler.h
    ${MY_PROJ_ROOT}/include/engine/service/SystemControl.h
    ${MY_PROJ_ROOT}/include/engine/service/SystemManager.h
    ${MY_PROJ_ROOT}/include/engine/Def.h
    ${MY_PROJ_ROOT}/include/engine/Engine.h
    ${MY_PROJ_ROOT}/include/engine/EngineEvent.h
    ${MY_PROJ_ROOT}/include/engine/MeshLoader.h
    #${MY_PROJ_ROOT}/include/engine/Scene.h
    ${MY_PROJ_ROOT}/include/engine/SDLApplication.h
    ${MY_PROJ_ROOT}/include/engine/Services.h
    ${MY_PROJ_ROOT}/include/engine/StandardShaderProgram.h
    ${MY_PROJ_ROOT}/include/engine/Utility.h
    #${MY_PROJ_ROOT}/include/test/assimp_test/Game.h
    ${MY_PROJ_ROOT}/include/SDLWrapper.h
    #${MY_PROJ_ROOT}/include/ShaderProgram_old.h
    )

#all engine include directories go in this variable
set(INDICUS_ENGINE_INCLUDE_DIRECTORIES
    ${MY_PROJ_ROOT}/include
    ${MY_PROJ_ROOT}/lib/ptlib/include
    ${MY_PROJ_ROOT}/lib/math3d/include
    ${MY_PROJ_ROOT}/lib/sdl2/include
    /usr/include
)

#all libraries shared by indicus lib and executables that link to indicus lib go in this variable
set(INDICUS_SHARED_LINK_SETTINGS
    -L${MY_PROJ_ROOT}/lib
    -lSDL2main
    -lSDL2
    -lGLEW
    -lGL
    -lpng
    -lassimp
#    -L"/usr/local/lib"  #when using "make install" made libraries
    -L"/usr/lib"
)

#all libraries statically linked to indicus lib go in this variable
#  note that only here should ptlib be linked to (statically)
#    executables linking to indicus lib shouldn't themselves link directly to 'ptlib'
#    rather access 'ptlib' contents through linking to indicus lib
#
#    (the double-linked 'ptlib' logging code may execute a double free somewhere during memory cleanup
#      after the last line of the program, causing a crash right before exiting
#      VERY VERY weird issue)
set(INDICUS_ENGINE_STATIC_LINK_SETTINGS
    -lptlib
    -lmath3d
)


set(INDICUS_ENGINE_COMPILE_DEFINITIONS
#'math3d' macros (need to be same as the lib was build with)
#    MATH_ENABLE_SWIZZLE=1

#disables assertions, define this for release versions
#    NDEBUG
)


#--------------------------------------------------
# engine test project setup
#--------------------------------------------------

#all test source files go in this variable
#set(INDICUS_TEST_SOURCE_FILES
    #${MY_PROJ_ROOT}/src/engine/Main.cpp
    #${MY_PROJ_ROOT}/src/engine/test/ActorTester/ColorRectComponent.cpp
    #${MY_PROJ_ROOT}/src/engine/test/ActorTester/Game.cpp
    #${MY_PROJ_ROOT}/src/engine/test/ActorTester/PlayerPawn.cpp
    #${MY_PROJ_ROOT}/src/engine/test/SDLKeytester.cpp
    #${MY_PROJ_ROOT}/src/engine/test/ogltester01/Game.cpp
#)

#all test header files go in this variable
#set(INDICUS_TEST_HEADER_FILES
    #${MY_PROJ_ROOT}/include/engine/test/ActorTester/ColorRectComponent.h
    #${MY_PROJ_ROOT}/include/engine/test/ActorTester/Game.h
    #${MY_PROJ_ROOT}/include/engine/test/ActorTester/PlayerPawn.h
    #${MY_PROJ_ROOT}/include/engine/test/SDLKeytester.h
    #${MY_PROJ_ROOT}/include/engine/test/ogltester01/Game.h
#)

#all test include directories go in this variable
set(INDICUS_TEST_INCLUDE_DIRECTORIES
    /usr/include
    ${MY_PROJ_ROOT}/include
)

#all libraries only linked to engine testing go in this variable
#set(INDICUS_TEST_LINK_SETTINGS
#)

#--------------------------------------------------
# dynamic engine library
#--------------------------------------------------

#build shared engine library (.so / DLL)
add_library(indicus SHARED
    ${INDICUS_ENGINE_SOURCE_FILES}
    ${INDICUS_ENGINE_HEADER_FILES}
)

target_include_directories(indicus PRIVATE
    ${INDICUS_ENGINE_INCLUDE_DIRECTORIES}
)

set_target_properties(indicus PROPERTIES COMPILE_FLAGS
    "${MY_COMPILE_FLAGS} -fPIC"
)

target_link_libraries(indicus PUBLIC
    "${INDICUS_SHARED_LINK_SETTINGS}"
    "${INDICUS_ENGINE_STATIC_LINK_SETTINGS}"
)

target_compile_definitions(indicus PRIVATE
    ${INDICUS_ENGINE_COMPILE_DEFINITIONS}
)

#--------------------------------------------------
# engine test executable
#--------------------------------------------------

#(linux) sets up the executable to look for libs in the same directory during runtime
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "$\{ORIGIN\}")

add_executable(OpenGL_test
    ${MY_PROJ_ROOT}/include/test/opengl_test/BillboardActor.h
    ${MY_PROJ_ROOT}/include/test/opengl_test/Game.h
    ${MY_PROJ_ROOT}/include/test/opengl_test/Pawn.h
    ${MY_PROJ_ROOT}/include/test/opengl_test/WorldGeometry.h
    ${MY_PROJ_ROOT}/include/test/opengl_test/WorldAxisActor.h
    ${MY_PROJ_ROOT}/src/test/opengl_test/BillboardActor.cpp
    ${MY_PROJ_ROOT}/src/test/opengl_test/Game.cpp
    ${MY_PROJ_ROOT}/src/test/opengl_test/main.cpp
    ${MY_PROJ_ROOT}/src/test/opengl_test/Pawn.cpp
    ${MY_PROJ_ROOT}/src/test/opengl_test/WorldGeometry.cpp
    ${MY_PROJ_ROOT}/src/test/opengl_test/WorldAxisActor.cpp
)

target_include_directories(OpenGL_test PRIVATE
    ${INDICUS_TEST_INCLUDE_DIRECTORIES}
    ${INDICUS_ENGINE_INCLUDE_DIRECTORIES}
)

#TODO: branch out based on debug or release build
#       and only look for libs in the appropriate folder
target_link_libraries(OpenGL_test
    ${INDICUS_SHARED_LINK_SETTINGS}
#    ${INDICUS_TEST_LINK_SETTINGS}
    -L"${MY_OUTPUT_DIR}"
    -L"${MY_OUTPUT_DIR_DEBUG}"
    -lindicus
)

target_compile_definitions(OpenGL_test PRIVATE
    ${INDICUS_ENGINE_COMPILE_DEFINITIONS}
)

add_dependencies(OpenGL_test indicus)

#--------------------------------------------------



#add_executable(indicus_test
#    ${INDICUS_TEST_SOURCE_FILES}
#    ${INDICUS_TEST_HEADER_FILES}
#)

#target_include_directories(indicus_test PRIVATE
#    ${INDICUS_TEST_INCLUDE_DIRECTORIES}
#    ${INDICUS_ENGINE_INCLUDE_DIRECTORIES}
#)

#TODO: branch out based on debug or release build
#       and only look for libs in the appropriate folder
#target_link_libraries(indicus_test
#    ${INDICUS_SHARED_LINK_SETTINGS}
##    ${INDICUS_TEST_LINK_SETTINGS}
#    -L"${MY_OUTPUT_DIR}"
#    -L"${MY_OUTPUT_DIR_DEBUG}"
#    -lindicus
#)

#target_compile_definitions(indicus_test PRIVATE
#    ${INDICUS_ENGINE_COMPILE_DEFINITIONS}
#)

#add_dependencies(indicus_test indicus)

#--------------------------------------------------

