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

HEADERS += \
    include/math/bool1.h \
    include/math/bool2.h \
    include/math/bool3.h \
    include/math/bool4.h \
    include/math/boolswizzle.h \
    include/math/float1.h \
    include/math/float2.h \
    include/math/float2swizzle.h \
    include/math/float3.h \
    include/math/float3swizzle.h \
    include/math/float4.h \
    include/math/float4swizzle.h \
    include/math/float4x4.h \
    include/math/floatswizzle.h \
    include/math/int1.h \
    include/math/int2.h \
    include/math/int3.h \
    include/math/int4.h \
    include/math/intswizzle.h \
    include/math/math.h \
    include/application.h \
    include/game.h \
    include/game_events.h \
    include/networking.h \
#    include/sdlcontrol.h \
    include/services.h \
#    include/transform2d.h \
    include/world.h \
    include/worldcomponent.h \
    sdl_keytester.h \
    include/entity.h \
    include/component.h \
    include/gamecontrol.h \
    include/physicalcomponent.h \
    test/ticktester_game.h \
    test/ticktester_printerentity.h \
    test/ticktester_printercomponent.h


SOURCES += \
    include/math/constants.cpp \
    src/application.cpp \
    src/game.cpp \
    src/main.cpp \
#    src/sdlcontrol.cpp \
    src/services.cpp \
#    src/transform2d.cpp \
    src/world.cpp \
    src/worldcomponent.cpp \
    sdl_keytester.cpp \
    src/entity.cpp \
    src/component.cpp \
    src/physicalcomponent.cpp \
    test/ticktester_game.cpp \
    test/ticktester_printercomponent.cpp \
    test/ticktester_printerentity.cpp





