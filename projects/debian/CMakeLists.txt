cmake_minimum_required(VERSION 3.5)

project(indicusengine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(MY_PROJ_ROOT ${PROJECT_SOURCE_DIR}/../..)

set(MY_OUTPUT_DIR_WIN32 ${MY_PROJ_ROOT}/bin/win32)
set(MY_OUTPUT_DIR_WIN64 ${MY_PROJ_ROOT}/bin/win64)
set(MY_OUTPUT_DIR_DEBIAN ${MY_PROJ_ROOT}/bin/debian)

#set up actual output directory
set(MY_OUTPUT_DIR ${MY_OUTPUT_DIR_DEBIAN})
set(MY_OUTPUT_DIR_DEBUG ${MY_OUTPUT_DIR}_debug)

#default output directory (when cuilding from cmd with everything default)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MY_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${MY_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${MY_OUTPUT_DIR})

#release build directory settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MY_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${MY_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${MY_OUTPUT_DIR})

#debug build directory settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${MY_OUTPUT_DIR_DEBUG})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${MY_OUTPUT_DIR_DEBUG})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${MY_OUTPUT_DIR_DEBUG})


#--------------------------------------------------
# engine library project setup

#all generic engine source files go in this variable
set(INDICUS_ENGINE_SOURCE_FILES

)

#all generic engine source files go in this variable
set(INDICUS_ENGINE_INCLUDE_DIRECTORIES
    /usr/include
#    /usr/include/SDL
    ${MY_PROJ_ROOT}/include
    ${MY_PROJ_ROOT}/lib/ptlib/include
    ${MY_PROJ_ROOT}/lib/math3d/include
)

#all libraries linked to engine go in this variable
set(INDICUS_ENGINE_LINK_SETTINGS
    -L${MY_PROJ_ROOT}/lib
    -lGLEW
    -lGL
    -lSDL2main
    -lSDL2
    -lpng
    -lassimp
    -lptlib
    -lmath3d
)

set(INDICUS_ENGINE_COMPILE_DEFINITIONS
#'math3d' macros (need to be same as the lib was build with)
#    MATH_REMOVE_PT_NAMESPACE=1
#    MATH_ENABLE_SWIZZLE=1
)


#--------------------------------------------------
# engine test project setup

#all generic engine source files go in this variable
set(INDICUS_TEST_SOURCE_FILES
    ${MY_PROJ_ROOT}/src/main.cpp
    ${MY_PROJ_ROOT}/src/testclass.cpp
    ${MY_PROJ_ROOT}/include/testclass.h
)

#all generic engine source files go in this variable
set(INDICUS_TEST_INCLUDE_DIRECTORIES
    /usr/include
    ${MY_PROJ_ROOT}/include
)

#--------------------------------------------------
# static engine library

#build static engine library (.a)
#add_library(indicus_static
#    ${INDICUS_ENGINE_SOURCE_FILES}
#)

#target_include_directories(indicus_static PRIVATE
#    ${INDICUS_ENGINE_INCLUDE_DIRECTORIES}
#)

#target_link_libraries(indicus_static
#    ${INDICUS_ENGINE_LINK_SETTINGS}
#)

#target_compile_definitions(indicus_static PRIVATE
#    ${INDICUS_ENGINE_COMPILE_DEFINITIONS}
#)

#--------------------------------------------------
# dynamic engine library

#build shared engine library (.so / DLL)
add_library(indicus SHARED
    ${INDICUS_ENGINE_SOURCE_FILES}
)

target_include_directories(indicus PRIVATE
    ${INDICUS_ENGINE_INCLUDE_DIRECTORIES}
)

target_link_libraries(indicus
    ${INDICUS_ENGINE_LINK_SETTINGS}
)

target_compile_definitions(indicus PRIVATE
    ${INDICUS_ENGINE_COMPILE_DEFINITIONS}
)

#--------------------------------------------------
# engine test executable

add_executable(indicus_test
    ${INDICUS_TEST_SOURCE_FILES}
)

target_include_directories(indicus_test PRIVATE
    ${INDICUS_TEST_INCLUDE_DIRECTORIES}
)

target_link_libraries(indicus
    -L${MY_PROJ_ROOT}/lib
#    -lindicus_static
    -lindicus
)

target_compile_definitions(indicus_test PRIVATE
    ${INDICUS_ENGINE_COMPILE_DEFINITIONS}
)


#--------------------------------------------------

