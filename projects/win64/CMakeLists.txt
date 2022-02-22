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

