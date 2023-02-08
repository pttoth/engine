#include "engine/SDLApplication.h"
#include "engine/Engine.h"

#include "engine/test/SDLKeytester.h"
#include "engine/test/ticktester/Game.h"
#include "engine/test/ogltester01/Game.h"
#include "engine/test/ActorTester/Game.h"

#include "pt/config.h"

#include <iostream>
#include <exception>


#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
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
main(int argc, char *argv[])
{
    signal(SIGSEGV, handler);   // install our handler
    signal(SIGABRT, handler);   // install our handler

    bool done = false;

    enum eSettings{
        strDefaultProject,
    };

    pt::Config cfg;
    CfgAddKey(cfg, strDefaultProject);
    std::string defproj;

    try{
        cfg.readF("../../cfg/main.cfg");
        defproj = cfg.getS(strDefaultProject);
        if( !pt::IsStringNumeric(defproj) || ('-' == defproj[0]) ){
            defproj.clear(); //set back to default if config val is not a number
        }
    }catch(std::invalid_argument& e){
        //file doesn't exist, run with defaults
        std::cout << "file doesn't exist\n"; // TODO logger
        cfg.writeF("../../cfg/main.cfg");
    }

    while(!done){
        try{
            SDL_Delay(20);
            std::cout << "\n-----\nwhat should I start?\n";
            SDL_Delay(20);
            std::cout << "1: SDL_KeyTester\n";
            SDL_Delay(20);
            std::cout << "2: Tick testing game\n";
            SDL_Delay(20);
            std::cout << "3: OpenGL test 01\n";
            SDL_Delay(20);
            std::cout << "4: Actor test\n";

            SDL_Delay(20);
            std::cout << "\n0: Exit program\n";
            char c;
            SDL_Delay(20);

            if(0 < defproj.length()){
                c = defproj[0];
                defproj = defproj.substr(1, defproj.length()-1);
            }else{
                std::cin >> c;
            }

            sdl_keytester tester;
            test::ticktester::Game tg;
            test::ogltester01::Game ogltester;
            test::actortester::Game actorTester;

            switch(c){
            case '0':
                done = true;
                break;
            case '1':
                tester.Execute();
                break;
            case '2':
                tg.Execute();
                break;
            case '3':
                ogltester.Execute();
                break;
            case '4':
                actorTester.Execute();
                break;


            default:
                std::cout << "invalid entry!\n";
                SDL_Delay(1000);
                break;
            }
        }
        catch(std::exception& e){
            std::cout << "ERROR: " << e.what() << ", program terminated!" << std::endl;
        }
        catch(...){
            std::cout << "ERROR: unknown exception caught, program terminated!" << std::endl;
        }
    }
    return 0;
}
