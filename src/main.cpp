/** -----------------------------------------------------------------------------
  * FILE:    main.cpp
  * AUTHOR:  pttoth - 2018.07.23.
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#include <iostream>
#include <exception>

#include "application.h"
#include "game.h"

#include "sdl_keytester.h"
#include "test/ticktester_game.h"


int main(int argc, char *argv[]){

    try{
        SDL_Delay(20);
        std::cout << "what should I start?\n";
        SDL_Delay(20);
        std::cout << "1: SDL_KeyTester\n";
        SDL_Delay(20);
        std::cout << "2: Tick testing game\n";
        char c;
        SDL_Delay(20);
        std::cin >> c;

        sdl_keytester tester;
        TickTesterGame tg;

        switch(c){
        case '1':
            tester.execute();
            break;
        case '2':
            tg.execute();
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

    return 0;
}
