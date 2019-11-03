TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += console c++11

TARGET = Engine

Release:DESTDIR     = $$PWD/bin/release
Release:OBJECTS_DIR = $$PWD/bin/release/.obj
Release:MOC_DIR     = $$PWD/bin/release/.moc
Release:RCC_DIR     = $$PWD/bin/release/.rcc
Release:UI_DIR      = $$PWD/bin/release/.ui

Debug:DESTDIR       = $$PWD/bin/debug
Debug:OBJECTS_DIR   = $$PWD/bin/debug/.obj
Debug:MOC_DIR       = $$PWD/bin/debug/.moc
Debug:RCC_DIR       = $$PWD/bin/debug/.rcc
Debug:UI_DIR        = $$PWD/bin/debug/.ui

INCLUDEPATH += \
                "/usr/include/" \
                "$$PWD/include/" \
#                "$$PWD/lib/pttoth_v1.0.0/include/" \
                "$$PWD/lib/pttoth_v1.1.0alpha/include/" \
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
        #-L"$$PWD/lib/pttoth_v1.0.0/lib/" \
        -L"$$PWD/lib/libpttoth_v1.0.1alpha/lib/" \
        #-L"$$PWD/lib/pttoth_v1.1.0alpha/lib/" \
        -L"$$PWD/lib/SDL2-2.0.8/i686-w64-mingw32/lib/" \
        -L"$$PWD/lib/SDL2_image-2.0.3/i686-w64-mingw32/lib/" \
        -L"$$PWD/lib/SDL2_net-2.0.1/i686-w64-mingw32/lib/" \
#----amd----------------------------
#        -L"$$PWD/lib/SDL2-2.0.8/x86_64-w64-mingw32/lib" \
#        -L"$$PWD/lib/SDL2_image-2.0.3/x86_64-w64-mingw32/lib/" \
#        -L"$$PWD/lib/SDL2_net-2.0.1/x86_64-w64-mingw32/lib/" \
#---------------------------------------------
        "-lpttoth" \
        "-lmingw32" \
        "-lSDL2main" \
        "-lSDL2" \
        "-lSDL2_image" \
        "-lSDL2_net"
        #"-lSDL2_ttf" \
        #"-lSDL2_mixer"

HEADERS += \
    include/application.h \
    include/game.h \
#    include/sdlcontrol.h \
    include/services.h \
#    include/transform2d.h \
    include/world.h \
    include/worldcomponent.h \
    include/entity.h \
    include/component.h \
    test/sdl_keytester.h \
    test/ticktester_game.h \
    test/ticktester_printerentity.h \
    test/ticktester_printercomponent.h \
    include/realcomponent.h \
    include/sdlhelpers.h \
    include/engine_events.h \
    include/enginecontrol.h


SOURCES += \
    src/application.cpp \
    src/game.cpp \
    src/main.cpp \
#    src/sdlcontrol.cpp \
    src/services.cpp \
#    src/transform2d.cpp \
    src/world.cpp \
    src/worldcomponent.cpp \
    src/entity.cpp \
    src/component.cpp \
    test/sdl_keytester.cpp \
    test/ticktester_game.cpp \
    test/ticktester_printercomponent.cpp \
    test/ticktester_printerentity.cpp \
    src/realcomponent.cpp \





