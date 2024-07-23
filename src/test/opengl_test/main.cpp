#include "test/opengl_test/BillboardActor.h"
#include "test/opengl_test/Game.h"
#include "test/opengl_test/Pawn.h"

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
#ifdef PT_PLATFORM_LINUX
    size_t  size;
    const size_t  maxsize = 256;
    void*   array[maxsize];

    // get void*'s for all entries on the stack
    size = backtrace( array, maxsize );

    // print out all the frames to stderr
    fprintf(stderr, "---------- Stack trace ----------\n" );
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    fprintf(stderr, "\n");
#else
    PT_UNIMPLEMENTED_FUNCTION
#endif
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
