#include "sdlbillboardcomponent.h"

#include <assert.h>

#include "services.h"
#include "sdlcontrol.h"
#include "enginecontrol.h"

using namespace engine;


SDLBillboardComponent::
SDLBillboardComponent():
    mWidth( 0.0f ), mHeight( 0.0f ),
    mMode( Mode::FilledRGBA ),
    mColorBase( pt::math::float3::white ), mColorBaseAlpha( 1.0f ),
    mColorFrame( pt::math::float3::white ), mColorFrameAlpha( 1.0f )
    //type...           mProcedural;      //TODO
    //Texture           mTexture;         //TODO
{}


SDLBillboardComponent::
~SDLBillboardComponent()
{}


bool SDLBillboardComponent::
operator==( const SDLBillboardComponent &other )
{
    assert(false); //TODO: implement
    return false;
}


void SDLBillboardComponent::
tick( float t, float dt )
{}


void SDLBillboardComponent::
OnRegistered()
{}


void SDLBillboardComponent::
OnUnregistered()
{}

pt::array<pt::math::float3>
SDLBillboardComponent::GetVertices()
{
    using namespace pt::math;

    pt::array<pt::math::float3> retval;
    retval.reserve(4);
    float whalf = mWidth/2;
    float hhalf = mHeight/2;
    retval.push_back( float3(-whalf,  hhalf, 0.0f) );
    retval.push_back( float3(-whalf, -hhalf, 0.0f) );
    retval.push_back( float3( whalf, -hhalf, 0.0f) );
    retval.push_back( float3( whalf,  hhalf, 0.0f) );

    return retval;
}


void SDLBillboardComponent::
OnCreateContext()
{}


void SDLBillboardComponent::
OnDestroyContext()
{}


void SDLBillboardComponent::
Draw()
{
    using namespace pt::math;

    SDLControl* sdl = Services::getSDLControl();
    if(nullptr == sdl){
        assert(sdl);
        pt::log::warn << "SDLBillboardComponent::Draw(): Couldn't fetch SDLControl ptr\n";
        return;
    }

    SDL_Renderer* r = sdl->GetMainRenderer();
    if(nullptr == r){
        assert(r);
        pt::log::warn << "SDLBillboardComponent::Draw(): Couldn't fetch renderer ptr!";
        return;
    }


    float4x4 M = this->getTransform();
    float4x4 VP = cam->GetViewMtx() * cam->GetProjMtx();
    float4x4 MVP = M*VP;

    pt::array<float3> vertices = this->GetVertices();

    //move vertices from model coords to normalized screen coords
    for(float3& v : vertices){
        float4 vf4 = float4(v, 1.0f) * MVP;
        v = Vecf3FromVecf4(vf4);
    }

    //scale up from normalized screen coords to actual pixel coords
    uint32_t resW = sdl->GetMainWindowWidth();
    uint32_t resH = sdl->GetMainWindowHeight();

    for(float3& v : vertices){
        //TODO: note: may need a -1 multiplier somewhere for the Y, we'll see
        //              (it may already be handled in GetViewMtx() )
        v.x = (v.x + 1)/2;
        v.y = (v.y + 1)/2;
        v.x = v.x * resW;
        v.y = v.y * resH;
    }

    SDL_Rect bbR;
    bbR.h = fabs(v[0]-v[3]);
    bbR.w = fabs(v[1]-v[2]);
    bbr.x = v[0].x;
    bbr.y = v[0].y;

    sdl->RenderDrawRect(r, bbRect);
}


bool SDLBillboardComponent::
IsVisible() const
{
    return true; //TODO: implement
}


void SDLBillboardComponent::
SetMode(SDLBillboardComponent::Mode mode)
{
    mMode = mode;
}


void SDLBillboardComponent::
SetHeight(float height)
{
    mHeight = height;
}


void SDLBillboardComponent::
SetWidth(float width)
{
    mWidth = width;
}


void SDLBillboardComponent::
SetBaseColor(const pt::math::float3& color)
{
    mColorBase = color;
}


void SDLBillboardComponent::
SetBaseColorAlpha(float alpha)
{
    mColorBaseAlpha = alpha;
}


void SDLBillboardComponent::
SetFrameColor(const pt::math::float3& color)
{
    mColorFrame = color;
}


void SDLBillboardComponent::
SetFrameColorAlpha(float alpha)
{
    mColorFrameAlpha = alpha;
}


SDLBillboardComponent::Mode SDLBillboardComponent::
GetMode() const
{
    return mMode;
}


float SDLBillboardComponent::
GetHeight() const
{
    return mHeight;
}


float SDLBillboardComponent::
GetWidth() const
{
    return mWidth;
}


pt::math::float3 SDLBillboardComponent::
GetBaseColor() const
{
    return mColorBase;
}


float SDLBillboardComponent::
GetBaseColorAlpha() const
{
    return mColorBaseAlpha;
}


pt::math::float3 SDLBillboardComponent::
GetFrameColor() const
{
    return mColorFrame;
}


float SDLBillboardComponent::
GetFrameColorAlpha() const
{
    return mColorFrameAlpha;
}


void SDLBillboardComponent::
OnSpawn()
{}


void SDLBillboardComponent::
OnDespawn()
{}
