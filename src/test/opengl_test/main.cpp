#include "test/opengl_test/BillboardActor.h"
#include "test/opengl_test/Game.h"
#include "test/opengl_test/Pawn.h"

#include <exception>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// linux
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>


// callback to call when encountering specific process signal
void
Handler(int sig )
{
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
    exit(1);
}

void
AssertHander( int sig ){
    std::cerr << "Assertion failed!\n";
    Handler( sig );
};

void
SegFaultHandler(int sig ){
    std::cerr << "Segmentation fault!\n";
    Handler( sig );
};

#include <thread>

int
main( int argc, char *argv[] )
{
    signal( SIGSEGV, SegFaultHandler );   // install our handler
    signal( SIGABRT, AssertHander );   // install our handler

    // shutdown after running for 3 seconds
    /*
    std::thread t1( []{
        PT_LOG_OUT( "Scheduling program exit after running for 3 seconds" );
        pt::Sleep( 3000 );
        PT_LOG_OUT( "3 seconds: Exiting program" );
        exit(1);
    } );
    */

    Game game( argc, argv );
    game.Initialize();
    game.Execute();
}
