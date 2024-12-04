#pragma once

#include "engine/Def.h"
#include "pt/math.h"
#include "SDL2/SDL.h"
#include <mutex>

namespace pt{
namespace sdl{

using MutexLock = std::unique_lock< std::mutex >;
using MutexLockGuard = std::lock_guard< std::mutex >;

extern std::mutex mutex_sdl;

int             WaitEvent( SDL_Event* ev );
int             PollEvent( SDL_Event* ev );

int             Init( Uint32 flags );

SDL_Window*     CreateWindow( const char* title, int x, int y, int w, int h, Uint32 flags );
SDL_Renderer*   CreateRenderer( SDL_Window* window, int index, Uint32 flags );
void            DestroyWindow( SDL_Window* window );
void            DestroyRenderer( SDL_Renderer* renderer );

SDL_GLContext   GL_CreateContext( SDL_Window* window );
void            GL_SwapWindow( SDL_Window* window );

//note: Functions taking normalized values don't clamp them into the [0, 1] interval
//        Non-normalized values will be 1-value off of 'desired' value for every loop around [0, 1]
//        ( eg 1.5f will be 126 instead of 127 | 2.5f will be 125 instead of 127, etc. )
int             SetRenderDrawColor( SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a );
int             SetRenderDrawColorNormalized( SDL_Renderer* renderer, float r, float g, float b, float a );
int             SetRenderDrawColorNormalizedF3( SDL_Renderer* renderer, const math::float3& color, float a );
int             SetRenderDrawColorNormalizedF4( SDL_Renderer* renderer, const math::float4& color );

int             SetRenderDrawBlendMode( SDL_Renderer* renderer, SDL_BlendMode blendMode );

Uint32          RegisterEvents( int numevents );

int             RenderClear( SDL_Renderer* renderer );
void            RenderPresent( SDL_Renderer* renderer );

int             RenderDrawRect( SDL_Renderer* renderer, const SDL_Rect* rect );
int             RenderFillRect( SDL_Renderer* renderer, const SDL_Rect* rect );

Uint64          GetTicks64();

} //end of namespace 'sdl'
} //end of namespace 'pt'
