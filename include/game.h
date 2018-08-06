/** -----------------------------------------------------------------------------
  * FILE:
  * AUTHOR:
  * EMAIL:
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "application.h"
//#include "world.h"

#include "SDL2/SDL.h"

#include <cstdint>

namespace pttoth{ namespace engine{

    class Game: public Application{

    public:
        Game();
        Game(int const argc, char* argv[]);
        ~Game();
    protected:
        SDL_Window* window;
        SDL_Renderer* renderer;

        virtual void onStart() override;
        virtual void onExit() override;
        virtual void onEvent(SDL_Event* event) override;

        virtual void updateGameState(float t, float dt);

        virtual void quit();

        /**

         */
        /**
         * @brief onMouseButtonDown
         * @param x
         * @param y
         * @param button:       "SDL_BUTTON_LEFT" "_MIDDLE" "_RIGHT" "_X1" or "_X2"
                    X1: Mouse4 ???  CHECK
                    X2: Mouse5 ???  CHECK
         * @param clicks:
         * @param mouseid:      id of mouse, or SDL_TOUCH_MOUSEID if touchpad
         */
        virtual void onMouseButtonDown(int32_t x, int32_t y,
                                       uint8_t button, uint8_t clicks,
                                       uint32_t timestamp, uint32_t mouseid);
        virtual void onMouseButtonUp(int32_t x, int32_t y,
                                     uint8_t button, uint8_t clicks,
                                     uint32_t timestamp, uint32_t mouseid);
        /**
         * @param x:         horizontal mouse position in window
         * @param y:         vertical   mouse position in window
         * @param x_rel:     horizontal mouse position relative to last position
         * @param y_rel:     vertical   mouse position relative to last position
         * @param mouseid:      id of mouse, or SDL_TOUCH_MOUSEID if touchpad
         */
        virtual void onMouseMotion(int32_t x, int32_t y,
                                   int32_t x_rel, int32_t y_rel,
                                   uint32_t timestamp, uint32_t mouseid);
        /**
         * @param x:         horizontal movement ( -left + right )
         * @param y:         vertical   movement ( -down + up    )
         * @param direction: platform dependent formats
         *                     SDL_MOUSEWHEEL_NORMAL or SDL_MOUSEWHEEL_FLIPPED
         *                     FLIPPED inverts x and y values
         * @param mouseid:      id of mouse, or SDL_TOUCH_MOUSEID if touchpad
         */
        virtual void onMouseWheel(int32_t x, int32_t y,
                                  uint32_t timestamp, uint32_t mouseid,
                                  uint32_t direction);
        /**
         * @param keycode:   key id
         * @param keymod:    active modifier keys
         * @param repeat:    non-zero if this is a key repeat
         */
        virtual void onKeyDown(SDL_Keycode keycode, uint16_t keymod,
                               uint32_t timestamp, uint8_t repeat);

        /**
         * @param keycode:   key id
         * @param keymod:    active modifier keys
         * @param repeat:    non-zero if this is a key repeat
         */
        virtual void onKeyUp(SDL_Keycode keycode, uint16_t keymod,
                             uint32_t timestamp, uint8_t repeat);

        /**
         * @brief onTouchEvent: touchpad events
         * @todo:   create signature
         */
        virtual void onTouchEvent();

        inline Uint32 getUpTime(){ return _uptime ; }

    private:
        Uint32 _tickrate = 50;
        Uint32 _uptime;
        SDL_TimerID _gametimer_id;
//        World* _world;


        void processGametimerTick();

    };
} }

