#include "sdl_keytester.h"

#include <string>
#include <iostream>
#include <sstream>

std::string getMBname(uint8_t button){
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
}

std::string dirToString(uint32_t id){
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

std::string mouseidToString(uint32_t id){
    switch (id){
    case SDL_TOUCH_MOUSEID:
        return "TOUCHPAD";
        break;
    default:
        return std::to_string( (unsigned) id);
        break;
    }
}

std::string translateFKey(SDL_Keycode code){
    std::string retval;
    retval.reserve(3);
    retval.push_back('F');
    if((SDLK_F1 <= code)&&(code<=SDLK_F9)){
        retval.push_back(code-SDLK_F1+'1');
        return retval;
    }else{
        retval.push_back('1');
        if( code == SDLK_F10) {
            retval.push_back('0');
        }else if( code == SDLK_F11) {
            retval.push_back('1');
        }else if( code == SDLK_F12) {
            retval.push_back('2');
        }else {
            return "n/a";
        }
    }
    return retval;
}

std::string keycodeToString(SDL_Keycode code){
    std::string retval;
    if(('a' <= code)&&(code<='z')){
        retval.push_back(code);
        return retval;
    }
    if(('0' <= code)&&(code<='9')){
        retval.push_back(code);
        return retval;
    }
    if((SDLK_F1 <= code)&&(code<=SDLK_F12)){
        return translateFKey(code);
    }

    switch (code){
        //case SDLK_NONE:     return "none";
        case SDLK_LSHIFT:       return "LSHIFT";
        case SDLK_RSHIFT:       return "RSHIFT";
        case SDLK_LCTRL:        return "LCTRL";
        case SDLK_RCTRL:        return "RCTRL";
        case SDLK_LALT:         return "LALT";
        case SDLK_RALT:         return "RALT";
        case SDLK_LGUI:         return "LGUI";
        case SDLK_RGUI:         return "RGUI";
        case SDLK_NUMLOCKCLEAR: return "NUM";
        case SDLK_CAPSLOCK:     return "CAPS";
        case SDLK_ESCAPE:       return "ESC";
        case SDLK_SPACE:        return "SPACE";
        case SDLK_BACKSPACE:    return "BACKSPACE";
        case SDLK_TAB:          return "TAB";
        case SDLK_RETURN:       return "RETURN";


/*
        case SDLK_CTRL:     return "CTRL";      //currently these never pass
        case SDLK_SHIFT:    return "SHIFT";     //currently these never pass
        case SDLK_ALT:      return "ALT";       //currently these never pass
        case SDLK_GUI:      return "GUI";       //currently these never pass
       case SDLK_RESERVED: return "reserved";*/
    }
/*
    retval = SDL_GetKeyName(code);
    return retval;
*/
    return "n/a";
}

std::stringstream& AddKeyToString(std::stringstream& ss, const char* str){
    std::string s = ss.str();
    if( 0 != s.length() ){ ss << '+'; }
    ss << str;
    return ss;
}

std::string keymodToString(uint16_t mod){
    std::stringstream ss;
    std::string str;
    if( mod & KMOD_LSHIFT ){
        AddKeyToString(ss, "LShift");
    }
    if( mod & KMOD_RSHIFT ){
        AddKeyToString(ss, "RShift");
    }
    if( mod & KMOD_LCTRL ){
        AddKeyToString(ss, "LCtrl");
    }
    if( mod & KMOD_RCTRL ){
        AddKeyToString(ss, "RCtrl");
    }
    if( mod & KMOD_LALT ){
        AddKeyToString(ss, "LAlt");
    }
    if( mod & KMOD_RALT ){
        AddKeyToString(ss, "RAlt");
    }
    if( mod & KMOD_LGUI ){
        AddKeyToString(ss, "LGUI");
    }
    if( mod & KMOD_RGUI ){
        AddKeyToString(ss, "RGUI");
    }
    if( mod & KMOD_NUM ){
        AddKeyToString(ss, "NUM");
    }
    if( mod & KMOD_CAPS ){
        AddKeyToString(ss, "CAPS");
    }
    if( mod & KMOD_MODE ){
        AddKeyToString(ss, "MODE");
    }

    switch (mod){
        case KMOD_NONE:     return "none";
        /*case KMOD_LSHIFT:   return "LSHIFT";
        case KMOD_RSHIFT:   return "RSHIFT";
        case KMOD_LCTRL:    return "LCTRL";
        case KMOD_RCTRL:    return "RCTRL";
        case KMOD_LALT:     return "LALT";
        case KMOD_RALT:     return "RALT";
        case KMOD_LGUI:     return "LGUI";
        case KMOD_RGUI:     return "RGUI";
        case KMOD_NUM:      return "NUM";
        case KMOD_CAPS:     return "CAPS";
        case KMOD_MODE:     return "MODE";
        case KMOD_CTRL:     return "CTRL";      //currently these never pass
        case KMOD_SHIFT:    return "SHIFT";     //currently these never pass
        case KMOD_ALT:      return "ALT";       //currently these never pass
        case KMOD_GUI:      return "GUI";       //currently these never pass
        */
        case KMOD_RESERVED: return "reserved";
    }
    str = ss.str();
    if(0 == str.length()){
        return "n/a";
    }
    return str;
}

sdl_keytester::sdl_keytester()
{

}

void sdl_keytester::
        onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
    std::string bname = getMBname(button);
    std::string mid = mouseidToString(mouseid);
    std::cout << "MBdown ("<<x<<","<<y<<"), " << bname << ", clicks: " <<  std::to_string(clicks) << ", t:" << timestamp << ", id:" << mid << std::endl;
}

void sdl_keytester::
        onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
    std::string bname = getMBname(button);
    std::string mid = mouseidToString(mouseid);
    std::cout << "MBup   ("<<x<<","<<y<<"), " << bname << ", clicks: " <<  std::to_string(clicks) << ", t:" << timestamp << ", id:" << mid << std::endl;
}

void sdl_keytester::
        onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid){
    std::string mid = mouseidToString(mouseid);
    //std::cout << "mousemotion(" << x << "," << y << ") rel: (" << x_rel << "," << y_rel << "), t:" << timestamp << ", id:" << mid << std::endl;
}

void sdl_keytester::
        onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction){
    std::string dir = dirToString(direction);
    std::string mid = mouseidToString(mouseid);
    std::cout << "mousewheel ("<<x<<","<<y<<"), t:" << timestamp << ", id:" << mid << ", dir: " << dir << std::endl;
}

void sdl_keytester::
        onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){
    std::string key = keycodeToString(keycode);
    std::string mod = keymodToString(keymod);
    std::cout << "KeyDown (" << key << ", " << mod << " ), t:" << timestamp << ", repeat: " << std::to_string(repeat) << std::endl;
}

void sdl_keytester::
        onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){
    std::string key = keycodeToString(keycode);
    std::string mod = keymodToString(keymod);
    std::cout << "KeyUp   (" << key << ", " << mod << " ), t:" << timestamp << ", repeat: " << std::to_string(repeat) << std::endl;
}

void sdl_keytester::
        onTouchInputEvent(){
    std::cout << "Touchpad" << std::endl;
}

void sdl_keytester::onStart()
{
    pttoth::engine::Game::onStart();
    window = SDL_CreateWindow("keytester", 300, 300, 800, 600, NULL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void sdl_keytester::onExit()
{
    pttoth::engine::Game::onExit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void sdl_keytester::
        tick(float t, float dt){
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );
}



