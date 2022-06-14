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

set(MY_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter")
set(CMAKE_C_FLAGS ${MY_COMPILE_FLAGS})
set(CMAKE_CXX_FLAGS ${MY_COMPILE_FLAGS})

#--------------------------------------------------
# engine library project setup
#--------------------------------------------------

#all generic engine source files go in this variable
set(INDICUS_ENGINE_SOURCE_FILES
    ${MY_PROJ_ROOT}/src/BasicPositionComponent.cpp
    ${MY_PROJ_ROOT}/src/Camera.cpp
    ${MY_PROJ_ROOT}/src/Camera2D.cpp
    ${MY_PROJ_ROOT}/src/Component.cpp
    ${MY_PROJ_ROOT}/src/DrawableComponent.cpp
    ${MY_PROJ_ROOT}/src/DrawingManager.cpp
    ${MY_PROJ_ROOT}/src/Engine.cpp
    ${MY_PROJ_ROOT}/src/Entity.cpp
    ${MY_PROJ_ROOT}/src/RealComponent.cpp
    ${MY_PROJ_ROOT}/src/SerialScheduler.cpp
    ${MY_PROJ_ROOT}/src/Services.cpp
    ${MY_PROJ_ROOT}/src/SDLApplication.cpp
    ${MY_PROJ_ROOT}/src/SDLControl.cpp
    ${MY_PROJ_ROOT}/src/SDLBillboardComponent.cpp
    ${MY_PROJ_ROOT}/src/World.cpp
    ${MY_PROJ_ROOT}/src/WorldComponent.cpp
)

#all generic engine header files go in this variable
set(INDICUS_ENGINE_HEADER_FILES
    ${MY_PROJ_ROOT}/include/BasicPositionComponent.h
    ${MY_PROJ_ROOT}/include/Camera.h
    ${MY_PROJ_ROOT}/include/Camera2D.h
    ${MY_PROJ_ROOT}/include/Common.h
    ${MY_PROJ_ROOT}/include/Component.h
    ${MY_PROJ_ROOT}/include/DrawingControl.h
    ${MY_PROJ_ROOT}/include/DrawableComponent.h
    ${MY_PROJ_ROOT}/include/DrawingManager.h
    ${MY_PROJ_ROOT}/include/Entity.h
    ${MY_PROJ_ROOT}/include/Engine.h
    ${MY_PROJ_ROOT}/include/EngineEvent.h
    ${MY_PROJ_ROOT}/include/Networking.h
    ${MY_PROJ_ROOT}/include/RealComponent.h
    ${MY_PROJ_ROOT}/include/Scheduler.h
    ${MY_PROJ_ROOT}/include/SerialScheduler.h
    ${MY_PROJ_ROOT}/include/Services.h
    ${MY_PROJ_ROOT}/include/SDLApplication.h
    ${MY_PROJ_ROOT}/include/SDLBillboardComponent.h
    ${MY_PROJ_ROOT}/include/SDLControl.h
    ${MY_PROJ_ROOT}/include/SDLDrawableComponent.h
    ${MY_PROJ_ROOT}/include/Ticker.h
    ${MY_PROJ_ROOT}/include/World.h
    ${MY_PROJ_ROOT}/include/WorldComponent.h
)

#all engine include directories go in this variable
set(INDICUS_ENGINE_INCLUDE_DIRECTORIES
    /usr/include
    ${MY_PROJ_ROOT}/include
    ${MY_PROJ_ROOT}/lib/ptlib/include
    ${MY_PROJ_ROOT}/lib/math3d/include
)

#all libraries shared by indicus lib and executables that link to indicus lib go in this variable
set(INDICUS_SHARED_LINK_SETTINGS
    -L"/usr/lib"
    -L${MY_PROJ_ROOT}/lib
    -lSDL2main
    -lSDL2
    -lGLEW
    -lGL
#    -lpng
#    -lassimp
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
    ${MY_PROJ_ROOT}/src/Main.cpp
    ${MY_PROJ_ROOT}/src/test/SDLKeytester.cpp
    ${MY_PROJ_ROOT}/src/test/ticktester/ColorRectComponent.cpp
    ${MY_PROJ_ROOT}/src/test/ticktester/Game.cpp
    ${MY_PROJ_ROOT}/src/test/ticktester/PlayerPawn.cpp
    ${MY_PROJ_ROOT}/src/test/ticktester/PrinterComponent.cpp
    ${MY_PROJ_ROOT}/src/test/ticktester/PrinterEntity.cpp
)

#all test header files go in this variable
set(INDICUS_TEST_HEADER_FILES
    ${MY_PROJ_ROOT}/include/test/SDLKeytester.h
    ${MY_PROJ_ROOT}/include/test/ticktester/ColorRectComponent.h
    ${MY_PROJ_ROOT}/include/test/ticktester/Game.h
    ${MY_PROJ_ROOT}/include/test/ticktester/IListener.h
    ${MY_PROJ_ROOT}/include/test/ticktester/PlayerPawn.h
    ${MY_PROJ_ROOT}/include/test/ticktester/PrinterComponent.h
    ${MY_PROJ_ROOT}/include/test/ticktester/PrinterEntity.h
)

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


add_executable(indicus_test
    ${INDICUS_TEST_SOURCE_FILES}
    ${INDICUS_TEST_HEADER_FILES}
)

target_include_directories(indicus_test PRIVATE
    ${INDICUS_TEST_INCLUDE_DIRECTORIES}
    ${INDICUS_ENGINE_INCLUDE_DIRECTORIES}
)

#TODO: branch out based on debug or release build
#       and only look for libs in the appropriate folder
target_link_libraries(indicus_test
    ${INDICUS_SHARED_LINK_SETTINGS}
#    ${INDICUS_TEST_LINK_SETTINGS}
    -L"${MY_OUTPUT_DIR}"
    -L"${MY_OUTPUT_DIR_DEBUG}"
    -lindicus
)

target_compile_definitions(indicus_test PRIVATE
    ${INDICUS_ENGINE_COMPILE_DEFINITIONS}
)

add_dependencies(indicus_test indicus)

#--------------------------------------------------

