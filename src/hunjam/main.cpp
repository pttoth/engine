#include "test/opengl_test/BillboardActor.h"
#include "test/opengl_test/Game.h"
#include "test/opengl_test/Pawn.h"

#include "engine/Utility.h"
#include "pt/def.h"

#include <exception>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <csignal>

// linux
#ifdef PT_PLATFORM_LINUX
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#endif

// callback to call when encountering specific process signal
void
Handler(int sig )
{
    PT_PRINT_STACKTRACE();
    exit(1);
}


void
AssertHander( int sig ){
    std::cerr << "Assertion failed!\n";
    Handler( sig );
}


void
SegFaultHandler(int sig ){
    std::cerr << "Segmentation fault!\n";
    Handler( sig );
}

int
main( int argc, char *argv[] )
{
    std::signal( SIGSEGV, SegFaultHandler );
    std::signal( SIGABRT, AssertHander );

    try{
        Game game( argc, argv );

        if( ! game.Initialize() ){
            return 1;
        }
        game.Execute();
    }catch( const std::exception& e ){
        PT_LOG_ERR( "An exception was thrown: "
                    << e.what() );
    }
    catch(...){
        PT_LOG_ERR( "An unknown exception was thrown." );
    }

    return 0;
}
