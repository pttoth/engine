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
#include "progkonfshowcasegame.h"

int main(int argc, char *argv[]){
    SDL_Delay(200); //delay to avoid qt debugger thread desync
    try{
        sdl_keytester tester;
        TickTesterGame tg;
        pttoth::engine::ProgKonfShowcaseGame pksg;

        std::cout << "what should I start?\n";
        std::cout << "1: " << tester.getGameName() << "\n";
        std::cout << "2: " << tg.getGameName() <<"\n";
        std::cout << "3: " << pksg.getGameName() << "\n";
        char c;
        SDL_Delay(50);
        std::cin >> c;
        //c = '3';
        switch(c){
        case '1':
            tester.execute();
            break;
        case '2':
            tg.execute();
            break;
        case '3':
            pksg.execute();
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
