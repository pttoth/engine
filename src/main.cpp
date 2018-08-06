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

int main(int argc, char *argv[]){

    try{
        //pttoth::engine::Game app(argc, argv);
        //app.execute();
        sdl_keytester tester;
        tester.execute();
    }
    catch(std::exception& e){
        std::cout << "ERROR: " << e.what() << ", program terminated!" << std::endl;
    }
    catch(...){
        std::cout << "ERROR: unknown exception caught, program terminated!" << std::endl;
    }

    return 0;
}
