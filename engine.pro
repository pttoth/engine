TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += console c++11

TARGET = Engine

INCLUDEPATH += \
                "/usr/include/" \
                "$$PWD/include" \
                "$$PWD/lib/SDL2-2.0.8/i686-w64-mingw32/include/" \
                "$$PWD/src/modified_libs/SDL2_image-2.0.3/i686-w64-mingw32/include/" \
                "$$PWD/src/modified_libs/SDL2_net-2.0.1/i686-w64-mingw32/include" \
#                "$$PWD/lib/SDL2_image-2.0.3/i686-w64-mingw32/include/" \
#                "$$PWD/lib/SDL2_net-2.0.1/i686-w64-mingw32/include" \
#                "$$PWD/lib/SDL2-2.0.8/i686-w64-mingw32/include/SDL2/" \
#                "$$PWD/lib/SDL2-2.0.8/x86_64-w64-mingw32/include/SDL2/" \
#                "$$PWD/lib/SDL2_image-2.0.3/x86_64-w64-mingw32/include/SDL2/"

LIBS += -L"/usr/lib" \
#---------------windows x86/x64---------------
#---intel---------------------------
        -L"$$PWD/lib/SDL2-2.0.8/i686-w64-mingw32/lib/" \
        -L"$$PWD/lib/SDL2_image-2.0.3/i686-w64-mingw32/lib/" \
        -L"$$PWD/lib/SDL2_net-2.0.1/i686-w64-mingw32/lib/" \
#----amd----------------------------
#        -L"$$PWD/lib/SDL2-2.0.8/x86_64-w64-mingw32/lib" \
#        -L"$$PWD/lib/SDL2_image-2.0.3/x86_64-w64-mingw32/lib/" \
#        -L"$$PWD/lib/SDL2_net-2.0.1/x86_64-w64-mingw32/lib/" \
#---------------------------------------------
        "-lmingw32" \
        "-lSDL2main" \
        "-lSDL2" \
        "-lSDL2_image" \
        "-lSDL2_net"
        #"-lSDL2_ttf" \
        #"-lSDL2_mixer"


#HEADERS +=

SOURCES += src/main.cpp

HEADERS += \
    networking.h


