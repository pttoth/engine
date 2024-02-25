#include "test/opengl_test/BillboardActor.h"
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
void handler(int sig)
{
    size_t  size;
    const size_t  maxsize = 256;
    void*   array[maxsize];

    // get void*'s for all entries on the stack
    size = backtrace( array, maxsize );

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}


int
main( int argc, char *argv[] )
{
    signal(SIGSEGV, handler);   // install our handler
    signal(SIGABRT, handler);   // install our handler



}
