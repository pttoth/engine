/** -----------------------------------------------------------------------------
  * FILE:    main.cpp
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#include "sdlapplication.h"
#include "engine.h"

#include "test/sdl_keytester.h"
#include "test/ticktester/game.h"

#include "pt/config.h"

#include <iostream>
#include <exception>


int
main(int argc, char *argv[])
{
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

            switch(c){
            case '0':
                done = true;
                break;
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
    }
    return 0;
}
