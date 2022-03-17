#include "test/sdl_keytester.h"

#include <string>
#include <sstream>
#include <iostream>


std::string
getMBname(uint8_t button)
{
    switch (button){
    case SDL_BUTTON_LEFT:
        return "MBleft";
    case SDL_BUTTON_RIGHT:
        return "MBright";
    case SDL_BUTTON_MIDDLE:
        return "MBmiddle";
    case SDL_BUTTON_X1:
        return "MB_X1";
    case SDL_BUTTON_X2:
        return "MB_X2";
    }
    return "n/a";
}


//TODO: rename this
std::string
dirToString(uint32_t id)
{
    switch (id){
    case SDL_MOUSEWHEEL_FLIPPED:
        return "flipped";
        break;
    case SDL_MOUSEWHEEL_NORMAL:
        return "normal";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}


std::string
mouseidToString(uint32_t id)
{
    switch (id){
    case SDL_TOUCH_MOUSEID:
        return "TOUCHPAD";
        break;
    default:
        return std::to_string( (unsigned) id);
        break;
    }
}


std::string
keycodeToString(SDL_Keycode code)
{
    if( ('A'<=code)&&(code<='Z') ){
        std::stringstream ss;
        ss << "Uppercase letters (" << code << ") in SDL_Keycode should not be possible!";
        throw std::invalid_argument(ss.str());
    }

    if( (('a'<=code)&&(code<='z'))
        ||(('0'<=code)&&(code<='9')))
    {
        std::string retval("SDLK_");
        retval.push_back(code);
        return retval;
    }

#define CaseForSDLKey(CODE) \
    case CODE: \
        return #CODE;

    switch(code){
        CaseForSDLKey(SDLK_UNKNOWN);

        CaseForSDLKey(SDLK_RETURN);
        CaseForSDLKey(SDLK_ESCAPE);
        CaseForSDLKey(SDLK_BACKSPACE);
        CaseForSDLKey(SDLK_TAB);
        CaseForSDLKey(SDLK_SPACE);
        CaseForSDLKey(SDLK_EXCLAIM);
        CaseForSDLKey(SDLK_QUOTEDBL);
        CaseForSDLKey(SDLK_HASH);
        CaseForSDLKey(SDLK_PERCENT);
        CaseForSDLKey(SDLK_DOLLAR);
        CaseForSDLKey(SDLK_AMPERSAND);
        CaseForSDLKey(SDLK_QUOTE);
        CaseForSDLKey(SDLK_LEFTPAREN);
        CaseForSDLKey(SDLK_RIGHTPAREN);
        CaseForSDLKey(SDLK_ASTERISK);
        CaseForSDLKey(SDLK_PLUS);
        CaseForSDLKey(SDLK_COMMA);
        CaseForSDLKey(SDLK_MINUS);
        CaseForSDLKey(SDLK_PERIOD);
        CaseForSDLKey(SDLK_SLASH);
        CaseForSDLKey(SDLK_COLON);
        CaseForSDLKey(SDLK_SEMICOLON);
        CaseForSDLKey(SDLK_LESS);
        CaseForSDLKey(SDLK_EQUALS);
        CaseForSDLKey(SDLK_GREATER);
        CaseForSDLKey(SDLK_QUESTION);
        CaseForSDLKey(SDLK_AT);

        CaseForSDLKey(SDLK_LEFTBRACKET);
        CaseForSDLKey(SDLK_BACKSLASH);
        CaseForSDLKey(SDLK_RIGHTBRACKET);
        CaseForSDLKey(SDLK_CARET);
        CaseForSDLKey(SDLK_UNDERSCORE);
        CaseForSDLKey(SDLK_BACKQUOTE);
        CaseForSDLKey(SDLK_CAPSLOCK);
        CaseForSDLKey(SDLK_F1);
        CaseForSDLKey(SDLK_F2);
        CaseForSDLKey(SDLK_F3);
        CaseForSDLKey(SDLK_F4);
        CaseForSDLKey(SDLK_F5);
        CaseForSDLKey(SDLK_F6);
        CaseForSDLKey(SDLK_F7);
        CaseForSDLKey(SDLK_F8);
        CaseForSDLKey(SDLK_F9);
        CaseForSDLKey(SDLK_F10);
        CaseForSDLKey(SDLK_F11);
        CaseForSDLKey(SDLK_F12);

        CaseForSDLKey(SDLK_PRINTSCREEN);
        CaseForSDLKey(SDLK_SCROLLLOCK);
        CaseForSDLKey(SDLK_PAUSE);
        CaseForSDLKey(SDLK_INSERT);
        CaseForSDLKey(SDLK_HOME);
        CaseForSDLKey(SDLK_PAGEUP);
        CaseForSDLKey(SDLK_DELETE);
        CaseForSDLKey(SDLK_END);
        CaseForSDLKey(SDLK_PAGEDOWN);
        CaseForSDLKey(SDLK_RIGHT);
        CaseForSDLKey(SDLK_LEFT);
        CaseForSDLKey(SDLK_DOWN);
        CaseForSDLKey(SDLK_UP);

        CaseForSDLKey(SDLK_NUMLOCKCLEAR);
        CaseForSDLKey(SDLK_KP_DIVIDE);
        CaseForSDLKey(SDLK_KP_MULTIPLY);
        CaseForSDLKey(SDLK_KP_MINUS);
        CaseForSDLKey(SDLK_KP_PLUS);
        CaseForSDLKey(SDLK_KP_ENTER);
        CaseForSDLKey(SDLK_KP_1);
        CaseForSDLKey(SDLK_KP_2);
        CaseForSDLKey(SDLK_KP_3);
        CaseForSDLKey(SDLK_KP_4);
        CaseForSDLKey(SDLK_KP_5);
        CaseForSDLKey(SDLK_KP_6);
        CaseForSDLKey(SDLK_KP_7);
        CaseForSDLKey(SDLK_KP_8);
        CaseForSDLKey(SDLK_KP_9);
        CaseForSDLKey(SDLK_KP_0);
        CaseForSDLKey(SDLK_KP_PERIOD);

        CaseForSDLKey(SDLK_APPLICATION);
        CaseForSDLKey(SDLK_POWER);
        CaseForSDLKey(SDLK_KP_EQUALS);
        CaseForSDLKey(SDLK_F13);
        CaseForSDLKey(SDLK_F14);
        CaseForSDLKey(SDLK_F15);
        CaseForSDLKey(SDLK_F16);
        CaseForSDLKey(SDLK_F17);
        CaseForSDLKey(SDLK_F18);
        CaseForSDLKey(SDLK_F19);
        CaseForSDLKey(SDLK_F20);
        CaseForSDLKey(SDLK_F21);
        CaseForSDLKey(SDLK_F22);
        CaseForSDLKey(SDLK_F23);
        CaseForSDLKey(SDLK_F24);
        CaseForSDLKey(SDLK_EXECUTE);
        CaseForSDLKey(SDLK_HELP);
        CaseForSDLKey(SDLK_MENU);
        CaseForSDLKey(SDLK_SELECT);
        CaseForSDLKey(SDLK_STOP);
        CaseForSDLKey(SDLK_AGAIN);
        CaseForSDLKey(SDLK_UNDO);
        CaseForSDLKey(SDLK_CUT);
        CaseForSDLKey(SDLK_COPY);
        CaseForSDLKey(SDLK_PASTE);
        CaseForSDLKey(SDLK_FIND);
        CaseForSDLKey(SDLK_MUTE);
        CaseForSDLKey(SDLK_VOLUMEUP);
        CaseForSDLKey(SDLK_VOLUMEDOWN);
        CaseForSDLKey(SDLK_KP_COMMA);
        CaseForSDLKey(SDLK_KP_EQUALSAS400);

        CaseForSDLKey(SDLK_ALTERASE);
        CaseForSDLKey(SDLK_SYSREQ);
        CaseForSDLKey(SDLK_CANCEL);
        CaseForSDLKey(SDLK_CLEAR);
        CaseForSDLKey(SDLK_PRIOR);
        CaseForSDLKey(SDLK_RETURN2);
        CaseForSDLKey(SDLK_SEPARATOR);
        CaseForSDLKey(SDLK_OUT);
        CaseForSDLKey(SDLK_OPER);
        CaseForSDLKey(SDLK_CLEARAGAIN);
        CaseForSDLKey(SDLK_CRSEL);
        CaseForSDLKey(SDLK_EXSEL);

        CaseForSDLKey(SDLK_KP_00);
        CaseForSDLKey(SDLK_KP_000);
        CaseForSDLKey(SDLK_THOUSANDSSEPARATOR);
        CaseForSDLKey(SDLK_DECIMALSEPARATOR);
        CaseForSDLKey(SDLK_CURRENCYUNIT);
        CaseForSDLKey(SDLK_CURRENCYSUBUNIT);
        CaseForSDLKey(SDLK_KP_LEFTPAREN);
        CaseForSDLKey(SDLK_KP_RIGHTPAREN);
        CaseForSDLKey(SDLK_KP_LEFTBRACE);
        CaseForSDLKey(SDLK_KP_RIGHTBRACE);
        CaseForSDLKey(SDLK_KP_TAB);
        CaseForSDLKey(SDLK_KP_BACKSPACE);
        CaseForSDLKey(SDLK_KP_A);
        CaseForSDLKey(SDLK_KP_B);
        CaseForSDLKey(SDLK_KP_C);
        CaseForSDLKey(SDLK_KP_D);
        CaseForSDLKey(SDLK_KP_E);
        CaseForSDLKey(SDLK_KP_F);
        CaseForSDLKey(SDLK_KP_XOR);
        CaseForSDLKey(SDLK_KP_POWER);
        CaseForSDLKey(SDLK_KP_PERCENT);
        CaseForSDLKey(SDLK_KP_LESS);
        CaseForSDLKey(SDLK_KP_GREATER);
        CaseForSDLKey(SDLK_KP_AMPERSAND);
        CaseForSDLKey(SDLK_KP_DBLAMPERSAND);
        CaseForSDLKey(SDLK_KP_VERTICALBAR);
        CaseForSDLKey(SDLK_KP_DBLVERTICALBAR);
        CaseForSDLKey(SDLK_KP_COLON);
        CaseForSDLKey(SDLK_KP_HASH);
        CaseForSDLKey(SDLK_KP_SPACE);
        CaseForSDLKey(SDLK_KP_AT);
        CaseForSDLKey(SDLK_KP_EXCLAM);
        CaseForSDLKey(SDLK_KP_MEMSTORE);
        CaseForSDLKey(SDLK_KP_MEMRECALL);
        CaseForSDLKey(SDLK_KP_MEMCLEAR);
        CaseForSDLKey(SDLK_KP_MEMADD);
        CaseForSDLKey(SDLK_KP_MEMSUBTRACT);
        CaseForSDLKey(SDLK_KP_MEMMULTIPLY);
        CaseForSDLKey(SDLK_KP_MEMDIVIDE);
        CaseForSDLKey(SDLK_KP_PLUSMINUS);
        CaseForSDLKey(SDLK_KP_CLEAR);
        CaseForSDLKey(SDLK_KP_CLEARENTRY);
        CaseForSDLKey(SDLK_KP_BINARY);
        CaseForSDLKey(SDLK_KP_OCTAL);
        CaseForSDLKey(SDLK_KP_DECIMAL);
        CaseForSDLKey(SDLK_KP_HEXADECIMAL);

        CaseForSDLKey(SDLK_LCTRL);
        CaseForSDLKey(SDLK_LSHIFT);
        CaseForSDLKey(SDLK_LALT);
        CaseForSDLKey(SDLK_LGUI);
        CaseForSDLKey(SDLK_RCTRL);
        CaseForSDLKey(SDLK_RSHIFT);
        CaseForSDLKey(SDLK_RALT);
        CaseForSDLKey(SDLK_RGUI);

        CaseForSDLKey(SDLK_MODE);

        CaseForSDLKey(SDLK_AUDIONEXT);
        CaseForSDLKey(SDLK_AUDIOPREV);
        CaseForSDLKey(SDLK_AUDIOSTOP);
        CaseForSDLKey(SDLK_AUDIOPLAY);
        CaseForSDLKey(SDLK_AUDIOMUTE);
        CaseForSDLKey(SDLK_MEDIASELECT);
        CaseForSDLKey(SDLK_WWW);
        CaseForSDLKey(SDLK_MAIL);
        CaseForSDLKey(SDLK_CALCULATOR);
        CaseForSDLKey(SDLK_COMPUTER);
        CaseForSDLKey(SDLK_AC_SEARCH);
        CaseForSDLKey(SDLK_AC_HOME);
        CaseForSDLKey(SDLK_AC_BACK);
        CaseForSDLKey(SDLK_AC_FORWARD);
        CaseForSDLKey(SDLK_AC_STOP);
        CaseForSDLKey(SDLK_AC_REFRESH);
        CaseForSDLKey(SDLK_AC_BOOKMARKS);

        CaseForSDLKey(SDLK_BRIGHTNESSDOWN);
        CaseForSDLKey(SDLK_BRIGHTNESSUP);
        CaseForSDLKey(SDLK_DISPLAYSWITCH);
        CaseForSDLKey(SDLK_KBDILLUMTOGGLE);

        CaseForSDLKey(SDLK_KBDILLUMDOWN);
        CaseForSDLKey(SDLK_KBDILLUMUP);
        CaseForSDLKey(SDLK_EJECT);
        CaseForSDLKey(SDLK_SLEEP);
        CaseForSDLKey(SDLK_APP1);
        CaseForSDLKey(SDLK_APP2);
        CaseForSDLKey(SDLK_AUDIOREWIND);
        CaseForSDLKey(SDLK_AUDIOFASTFORWARD);
    }
    return "n/a";
}


std::string
keymodToString(uint16_t mod)
{
    size_t count = 0;
    std::stringstream ss;

    if(mod == KMOD_NONE) return "KMOD_NONE";
    if(mod == KMOD_RESERVED) return "KMOD_RESERVED";

    #define CaseForSDLKeyMod(CODE) \
        if((mod & CODE) == CODE){ \
            if(0 < count) \
                ss << "; "; \
            ss << #CODE; \
            ++count; \
        }

    CaseForSDLKeyMod(KMOD_LCTRL);
    CaseForSDLKeyMod(KMOD_RCTRL);
    CaseForSDLKeyMod(KMOD_LSHIFT);
    CaseForSDLKeyMod(KMOD_RSHIFT);
    CaseForSDLKeyMod(KMOD_LALT);
    CaseForSDLKeyMod(KMOD_RALT);
    CaseForSDLKeyMod(KMOD_LGUI);
    CaseForSDLKeyMod(KMOD_RGUI);
    CaseForSDLKeyMod(KMOD_NUM);
    CaseForSDLKeyMod(KMOD_CAPS);
    CaseForSDLKeyMod(KMOD_MODE);

    if(0 < count){
        return ss.str();
    }
    return "n/a";
}


sdl_keytester::
sdl_keytester()
{}


void sdl_keytester::
onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{
    std::string bname = getMBname(button);
    std::string mid = mouseidToString(mouseid);
    std::cout << "MBdown ("<<x<<","<<y<<"), " << bname << ", clicks: " <<  std::to_string(clicks) << ", t:" << timestamp << ", id:" << mid << std::endl;
}


void sdl_keytester::
onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{
    std::string bname = getMBname(button);
    std::string mid = mouseidToString(mouseid);
    std::cout << "MBup   ("<<x<<","<<y<<"), " << bname << ", clicks: " <<  std::to_string(clicks) << ", t:" << timestamp << ", id:" << mid << std::endl;
}


void sdl_keytester::
onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid)
{
    std::string mid = mouseidToString(mouseid);
    //std::cout << "mousemotion(" << x << "," << y << ") rel: (" << x_rel << "," << y_rel << "), t:" << timestamp << ", id:" << mid << std::endl;
}


void sdl_keytester::
onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{
    std::string dir = dirToString(direction);
    std::string mid = mouseidToString(mouseid);
    std::cout << "mousewheel ("<<x<<","<<y<<"), t:" << timestamp << ", id:" << mid << ", dir: " << dir << std::endl;
}


void sdl_keytester::
onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{
    std::string key = keycodeToString(keycode);
    std::string mod = keymodToString(keymod);
    std::cout << "KeyDown (" << key << ", " << mod << " ), t:" << timestamp << ", repeat: " << std::to_string(repeat) << std::endl;
}


void sdl_keytester::
onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{
    std::string key = keycodeToString(keycode);
    std::string mod = keymodToString(keymod);
    std::cout << "KeyUp   (" << key << ", " << mod << " ), t:" << timestamp << ", repeat: " << std::to_string(repeat) << std::endl;
}


void sdl_keytester::
onTouchInputEvent()
{
    std::cout << "Touchpad" << std::endl;
}


void sdl_keytester::
onStart()
{
    engine::Engine::onStart();
    window = SDL_CreateWindow("keytester", 300, 300, 800, 600, NULL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


void sdl_keytester::
onExit()
{
    engine::Engine::onExit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}


void sdl_keytester::
tick(float t, float dt)
{
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );
}



