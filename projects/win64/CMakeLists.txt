#--------------------------------------------------
#   syntax samples
#--------------------------------------------------
#default include directories (legacy, cmake 2.8.10 or older)
#   include_directories(<directory> [<directory> ...])

#default include directories
#!!! add this AFTER specifying the target !!!
#   target_include_directories(<target> PRIVATE <directory> [<directory> ...])


#variable reference syntax:
#   ${<variable>}

#-----
#useful variables:
#-----
#project build configurations
#   DEBUG
#   RELEASE
#   MINSIZEREL
#   RELWITHDEBINFO

#   PROJECT_SOURCE_DIR
#   CMAKE_BINARY_DIR
#   CMAKE_RUNTIME_OUTPUT_DIRECTORY  # for executable targets
#   CMAKE_ARCHIVE_OUTPUT_DIRECTORY  # for .a files (static libs)
#   CMAKE_LIBRARY_OUTPUT_DIRECTORY  # for .so files (shared libraries / DLL-s)

#   EXECUTABLE_OUTPUT_PATH          # !!! this is old, don't use !!!


#-----
#define version
#note: ALWAYS call before 'project()' call!
#   cmake_minimum_required(VERSION 3.5)

#project(<name> [<language-name>...])
#   sets project name
#       has to be one direct all at the top level AFTER cmake_minimum_required() call
#   defines the enabled languages (C and CXX default)
#       C, CXX (i.e. C++), CUDA, OBJC (i.e. Objective-C), OBJCXX, Fortran, ISPC, and ASM.
#   and sets variables
#      PROJECT_NAME
#      CMAKE_PROJECT_NAME
#      PROJECT_SOURCE_DIR
#         <PROJECT-NAME>_SOURCE_DIR
#      PROJECT_BINARY_DIR
#         <PROJECT-NAME>_BINARY_DIR

#create executable as target
#   add_executable(<executable> <sourcefile> [<sourcefile> ...] [<header> ...])

#create lib as target
#   add_library(<library> <sourcefile> [<sourcefile> ...] [<header> ...])

#create DLL as target
#   add_library(<library> SHARED <sourcefile> [<sourcefile> ...] [<header> ...])

#link lib-s and DLL-s to executable
#   target_link_libraries(<target> <library> [<library> ...])


#set up target build directory
#   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#OR
#   set_target_properties( targets...
#       PROPERTIES
#       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#   )

#   You can append _[CONFIG] to the variable/property name to make the output directory apply to a specific configuration
#       eg: CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG
#           CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE



#cmake -DCMAKE_BUILD_TYPE=Debug path/to/source
#cmake -DCMAKE_BUILD_TYPE=Release path/to/source
#CMAKE_BUILD_TYPE

#-----
#fill out later
#-----

# include hierarchies
#   visibility
#     PUBLIC | PRIVATE | INTERFACE
#     relevant, when targets refer eachother as dependencies

#   interface includes
#     interface_include_directories( ... )

# install target
#   install(TARGETS <target>)



#--------------------------------------------------
#   Project
#--------------------------------------------------

cmake_minimum_required(VERSION 3.5)

project(cmake_template_win64 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(MY_PROJ_ROOT ${PROJECT_SOURCE_DIR}/../..)

set(MY_OUTPUT_DIR_WIN32 ${MY_PROJ_ROOT}/bin/win32)
set(MY_OUTPUT_DIR_WIN64 ${MY_PROJ_ROOT}/bin/win64)
set(MY_OUTPUT_DIR_DEBIAN ${MY_PROJ_ROOT}/bin/debian)

#set up actual output directory
set(MY_OUTPUT_DIR ${MY_OUTPUT_DIR_WIN64})
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


#build an archive (.a), for static linking
add_library(applib
    ${MY_PROJ_ROOT}/src/testclass.cpp
    ${MY_PROJ_ROOT}/include/testclass.h
)

target_include_directories(applib PRIVATE
    ${MY_PROJ_ROOT}/include
)

#build a shared library (.so / DLL)
add_library(applib2 SHARED
    ${MY_PROJ_ROOT}/src/testclass.cpp
    ${MY_PROJ_ROOT}/include/testclass.h
)

target_include_directories(applib2 PRIVATE
    ${MY_PROJ_ROOT}/include
)

add_executable(cmaketest
    ${MY_PROJ_ROOT}/src/main.cpp
    ${MY_PROJ_ROOT}/src/testclass.cpp
    ${MY_PROJ_ROOT}/include/testclass.h
)

target_include_directories(cmaketest PRIVATE
    ${MY_PROJ_ROOT}/include
)



#set up library linking paths
#  directories and library names both come here
#target_link_libraries(cmaketest
#   -L<directory>
#   -l<applib>
#)

