#include "sdlbillboardcomponent.h"

#include <assert.h>

#include "services.h"
#include "sdlcontrol.h"
#include "enginecontrol.h"
#include "DrawingManager.h"

#include "pt/logging.h"

using namespace engine;


SDLBillboardComponent::
SDLBillboardComponent(const std::string &name):
    SDLDrawableComponent(name),
    mWidth( 0.0f ), mHeight( 0.0f ),
    mMode( Mode::FilledRGBA ),
    mColorBase( pt::math::float3::white ), mColorBaseAlpha( 1.0f ),
    mColorFrame( pt::math::float3::white ), mColorFrameAlpha( 1.0f )
    //type...           mProcedural;      //TODO
    //Texture           mTexture;         //TODO
{}


SDLBillboardComponent::
SDLBillboardComponent(const SDLBillboardComponent &other):
    SDLDrawableComponent(other)
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

std::vector<pt::math::float3> SDLBillboardComponent::GetVertices()
{
    using namespace pt::math;

    std::vector<pt::math::float3> retval;
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
Draw(float t, float dt)
{
    using namespace pt::math;

    SDLControl* sdl = Services::getSDLControl();
    SDL_Renderer* r = sdl->GetMainRenderer();
    Camera* cam = Services::getDrawingControl()->GetMainCamera();

    float4x4 M = this->getTransform();
    float4x4 VP = cam->GetViewMtx() * cam->GetProjMtx();
    float4x4 MVP = M*VP;

    std::vector<float3> vertices = this->GetVertices();

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

    for(float3& v : vertices){
        v.z = 0.0f;
    }

    SDL_Rect bbR;
    bbR.h = fabs( (vertices[0]-vertices[3]).length() );
    bbR.w = fabs( (vertices[1]-vertices[2]).length() );
    bbR.x = vertices[0].x;
    bbR.y = vertices[0].y;

    sdl->RenderDrawRect(r, &bbR);
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
