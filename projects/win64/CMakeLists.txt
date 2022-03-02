cmake_minimum_required(VERSION 3.5)

project(indicusengine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(MY_PROJ_ROOT ${PROJECT_SOURCE_DIR}/../..)
set(MY_BUILD_DIR ${MY_PROJ_ROOT}/build)
set(MY_OUTPUT_DIR_WIN32 ${MY_PROJ_ROOT}/bin/win32)
set(MY_OUTPUT_DIR_WIN64 ${MY_PROJ_ROOT}/bin/win64)
set(MY_OUTPUT_DIR_DEBIAN ${MY_PROJ_ROOT}/bin/debian)

#set up actual output directory
set(MY_OUTPUT_DIR ${MY_OUTPUT_DIR_WIN64})
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

set(MY_COMPILE_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS ${MY_COMPILE_FLAGS})
set(CMAKE_CXX_FLAGS ${MY_COMPILE_FLAGS})

#--------------------------------------------------
# engine library project setup
#--------------------------------------------------

#all generic engine source files go in this variable
set(INDICUS_ENGINE_SOURCE_FILES
    ${MY_PROJ_ROOT}/src/main.cpp
    ${MY_PROJ_ROOT}/src/component.cpp
    ${MY_PROJ_ROOT}/src/engine.cpp
    ${MY_PROJ_ROOT}/src/entity.cpp
    ${MY_PROJ_ROOT}/src/realcomponent.cpp
    ${MY_PROJ_ROOT}/src/services.cpp
    ${MY_PROJ_ROOT}/src/sdlapplication.cpp
    ${MY_PROJ_ROOT}/src/sdlcontrol.cpp
    ${MY_PROJ_ROOT}/src/world.cpp
    ${MY_PROJ_ROOT}/src/worldcomponent.cpp
)

#all generic engine header files go in this variable
set(INDICUS_ENGINE_HEADER_FILES
    ${MY_PROJ_ROOT}/include/component.h
    ${MY_PROJ_ROOT}/include/entity.h
    ${MY_PROJ_ROOT}/include/engine.h
    ${MY_PROJ_ROOT}/include/enginecontrol.h
    ${MY_PROJ_ROOT}/include/game_events.h
    ${MY_PROJ_ROOT}/include/networking.h
    ${MY_PROJ_ROOT}/include/realcomponent.h
    ${MY_PROJ_ROOT}/include/services.h
    ${MY_PROJ_ROOT}/include/sdlapplication.h
    ${MY_PROJ_ROOT}/include/sdlcontrol.h
    ${MY_PROJ_ROOT}/include/world.h
    ${MY_PROJ_ROOT}/include/worldcomponent.h
)

#all engine include directories go in this variable
set(INDICUS_ENGINE_INCLUDE_DIRECTORIES
    ${MY_PROJ_ROOT}/include
    ${MY_PROJ_ROOT}/lib/ptlib/include
    ${MY_PROJ_ROOT}/lib/math3d/include
)

#all libraries linked to engine go in this variable
set(INDICUS_ENGINE_LINK_SETTINGS
#     -L${MY_PROJ_ROOT}/lib
#     -lGLEW
#     -lGL
#     -lSDL2main
#     -lSDL2
#     -lpng
#     -lassimp
#     -lptlib
#     -lmath3d
)

set(INDICUS_ENGINE_COMPILE_DEFINITIONS
#'math3d' macros (need to be same as the lib was build with)
#    MATH_REMOVE_PT_NAMESPACE=1
#    MATH_ENABLE_SWIZZLE=1

#disables assertions, define this for release versions
#    NDEBUG
)


#--------------------------------------------------
# engine test project setup
#--------------------------------------------------

#all test source files go in this variable
set(INDICUS_TEST_SOURCE_FILES
    ${MY_PROJ_ROOT}/src/main.cpp
    ${MY_PROJ_ROOT}/src/test/sdl_keytester.cpp
    ${MY_PROJ_ROOT}/src/test/ticktester_game.cpp
    ${MY_PROJ_ROOT}/src/test/ticktester_printercomponent.cpp
    ${MY_PROJ_ROOT}/src/test/ticktester_printerentity.cpp
)

#all test header files go in this variable
set(INDICUS_TEST_HEADER_FILES
    ${MY_PROJ_ROOT}/include/test/sdl_keytester.h
    ${MY_PROJ_ROOT}/include/test/ticktester_game.h
    ${MY_PROJ_ROOT}/include/test/ticktester_printercomponent.h
    ${MY_PROJ_ROOT}/include/test/ticktester_printerentity.h
)

#all test include directories go in this variable
set(INDICUS_TEST_INCLUDE_DIRECTORIES
    /usr/include
    ${MY_PROJ_ROOT}/include
)

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

target_link_libraries(indicus
    ${INDICUS_ENGINE_LINK_SETTINGS}
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


add_executable(indicus_test
    ${INDICUS_TEST_SOURCE_FILES}
    ${INDICUS_TEST_HEADER_FILES}
    ${INDICUS_ENGINE_HEADER_FILES}
)

target_include_directories(indicus_test PRIVATE
    ${INDICUS_TEST_INCLUDE_DIRECTORIES}
    ${INDICUS_ENGINE_INCLUDE_DIRECTORIES}
)

target_link_libraries(indicus_test
    ${INDICUS_ENGINE_LINK_SETTINGS}
    -L"${MY_OUTPUT_DIR}"
    -L"${MY_OUTPUT_DIR_DEBUG}"
    -lindicus
)

target_compile_definitions(indicus_test PRIVATE
    ${INDICUS_ENGINE_COMPILE_DEFINITIONS}
)


#--------------------------------------------------

