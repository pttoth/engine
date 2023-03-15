#include "SDLWrapper.h"

std::mutex pt::sdl::mutex_sdl;

int pt::sdl::
WaitEvent( SDL_Event* ev )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_WaitEvent( ev );
}


int pt::sdl::
PollEvent( SDL_Event* ev )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_PollEvent( ev );
}


int pt::sdl::
Init( Uint32 flags )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_Init( flags );
}


SDL_Window* pt::sdl::
CreateWindow( const char* title, int x, int y, int w, int h, Uint32 flags )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_CreateWindow( title, x, y, w, h, flags );
}


SDL_Renderer* pt::sdl::
CreateRenderer( SDL_Window* window, int index, Uint32 flags )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_CreateRenderer( window, index, flags );
}


void pt::sdl::
DestroyWindow( SDL_Window* window )
{
    MutexLockGuard lock( mutex_sdl );
    SDL_DestroyWindow( window );
}


void pt::sdl::
DestroyRenderer( SDL_Renderer* renderer )
{
    MutexLockGuard lock( mutex_sdl );
    SDL_DestroyRenderer( renderer );
}


void pt::sdl::
GL_SwapWindow( SDL_Window* window )
{
    MutexLockGuard lock( mutex_sdl );
    SDL_GL_SwapWindow( window );
}


int pt::sdl::
SetRenderDrawColor( SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_SetRenderDrawColor( renderer, r, g, b, a );
}


int pt::sdl::
SetRenderDrawColorNormalized( SDL_Renderer* renderer, float r, float g, float b, float a )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_SetRenderDrawColor( renderer, r*255, g*255, b*255, a*255 );
}


int pt::sdl::
SetRenderDrawColorNormalizedF3( SDL_Renderer* renderer, const pt::math::float3& color, float a )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_SetRenderDrawColor( renderer, color.x*255, color.y*255, color.z*255, a*255 );
}


int pt::sdl::
SetRenderDrawColorNormalizedF4( SDL_Renderer* renderer, const pt::math::float4& color )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_SetRenderDrawColor( renderer, color.x*255, color.y*255, color.z*255, color.w*255 );
}


int pt::sdl::
SetRenderDrawBlendMode( SDL_Renderer* renderer, SDL_BlendMode blendMode )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_SetRenderDrawBlendMode( renderer, blendMode );
}


int pt::sdl::
RenderClear( SDL_Renderer* renderer )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_RenderClear( renderer );
}


void pt::sdl::
RenderPresent( SDL_Renderer* renderer )
{
    MutexLockGuard lock( mutex_sdl );
    SDL_RenderPresent( renderer );
}


int pt::sdl::
RenderDrawRect( SDL_Renderer* renderer, const SDL_Rect* rect )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_RenderDrawRect( renderer, rect );
}


int pt::sdl::
RenderFillRect( SDL_Renderer* renderer, const SDL_Rect* rect )
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_RenderFillRect( renderer, rect );
}


Uint64 pt::sdl::
GetTicks64()
{
    MutexLockGuard lock( mutex_sdl );
    return SDL_GetTicks64();
}
