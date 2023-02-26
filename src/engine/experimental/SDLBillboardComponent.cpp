#include "engine/experimental/SDLBillboardComponent.h"

#include "engine/Services.h"
#include "engine/SDLControl.h"

#include "engine/experimental/DrawingControl.h"

#include "engine/Camera.h"


using namespace engine;
using namespace engine::experimental;
using namespace pt::math;

SDLBillboardComponent::
SDLBillboardComponent( const std::string& name ):
    SDLComponent( name ),
    mWidth( 1.0f ), mHeight( 1.0f ),
    mMode( Mode::FilledRGBA ), mHasFrame(false),
    mColorBase( float4(float3::white, 1.0f )),
    mColorFrame( float4(float3::white, 1.0f ))
    //type...           mProcedural;      //TODO
    //Texture           mTexture;         //TODO
{}


SDLBillboardComponent::
~SDLBillboardComponent()
{}


void SDLBillboardComponent::
Tick(float t, float dt)
{
    SDLComponent::Tick( t, dt );
}


std::vector<pt::math::float3> SDLBillboardComponent::
GetVertices()
{
    using namespace pt::math;

    //TODO: avoid per-frame memory allocation
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
SetFrameEnabled(bool enabled)
{
    mHasFrame = enabled;
}


void SDLBillboardComponent::
SetMode(Mode mode)
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
SetBaseColorF(float red, float green, float blue, float alpha)
{
    mColorBase = float4( red, green, blue, alpha );
}


void SDLBillboardComponent::
SetBaseColor(const pt::math::float3 &color, float alpha)
{
    SetBaseColorF( color.x, color.y, color.z, alpha );
}


void SDLBillboardComponent::
SetBaseColorAlpha(float alpha)
{
    mColorBase.w = alpha;
}


void SDLBillboardComponent::
SetFrameColorF(float red, float green, float blue, float alpha)
{
    mColorFrame = float4( red, green, blue, alpha );
}


void SDLBillboardComponent::
SetFrameColor(const pt::math::float3 &color, float alpha)
{
    SetFrameColorF( color.x, color.y, color.z, alpha );
}


void SDLBillboardComponent::
SetFrameColorAlpha(float alpha)
{
    mColorFrame.w = alpha;
}


bool SDLBillboardComponent::
GetFrameEnabled() const
{
    return mHasFrame;
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
GetBaseColorF3() const
{
    const float4& c = mColorBase;
    return float3( c.x, c.y, c.z );
}


pt::math::float4 SDLBillboardComponent::
GetBaseColorF4() const
{
    return mColorBase;
}


float SDLBillboardComponent::
GetBaseColorAlpha() const
{
    return mColorBase.w;
}


pt::math::float3 SDLBillboardComponent::
GetFrameColorF3() const
{
    const float4& c = mColorFrame;
    return float3( c.x, c.y, c.z );
}


pt::math::float4 SDLBillboardComponent::
GetFrameColorF4() const
{
    return mColorFrame;
}


float SDLBillboardComponent::
GetFrameColorAlpha() const
{
    return mColorFrame.w;
}


bool SDLBillboardComponent::
IsVisible() const
{
    return true; //TODO: implement
}


void SDLBillboardComponent::Draw(float t, float dt)
{
    using namespace pt::math;

    SDLControl* sdl = Services::GetSDLControl();
    SDL_Renderer* r = sdl->GetMainRenderer();
    Camera* cam = Services::GetDrawingControl2()->GetMainCamera();

    float4x4 M = this->GetWorldTransform();
    float4x4 V = cam->GetViewMtx();
    float4x4 P = cam->GetProjMtx();
    float4x4 PVM = P*V*M;

    std::vector<float3> vertices = this->GetVertices();


    //move vertices from model coords to normalized screen coords
    for(float3& v : vertices){
        float4 vf4 = PVM * float4(v, 1.0f);
        v = Vecf3FromVecf4(vf4);
    }

    //scale up from normalized screen coords to actual pixel coords
    uint32_t resW = sdl->GetMainWindowWidth();
    uint32_t resH = sdl->GetMainWindowHeight();

    for(float3& v : vertices){
        v.x = (     v.x + 1)/2;
        v.y = ((-1)*v.y + 1)/2;
        v.x = v.x * resW;
        v.y = v.y * resH;
    }

    for(float3& v : vertices){
        v.z = 0.0f;
    }


    SDL_Rect bbR;
    bbR.w = fabs( (vertices[0]-vertices[3]).length() );
    bbR.h = fabs( (vertices[0]-vertices[1]).length() );
    bbR.x = vertices[0].x;
    bbR.y = vertices[0].y;


/*
    bbR.h = 200;
    bbR.w = 200;

    bbR.x = 400;
    bbR.y = 400;
*/

    if(Mode::FilledRGBA == mMode){
        sdl->SetRenderDrawColorNormalizedF4( r, this->GetBaseColorF4() );
        sdl->RenderFillRect(r, &bbR);
    }else{
        //TODO: other drawing modes
    }

    if(mHasFrame){
        sdl->SetRenderDrawColorNormalizedF4( r, this->GetFrameColorF4() );
        sdl->RenderDrawRect(r, &bbR);
    }
}


void SDLBillboardComponent::
OnSpawned()
{
    RealComponent::OnSpawned();

    auto dc = Services::GetDrawingControl2();
    dc->AddDrawable( this );
}


void SDLBillboardComponent::
OnDespawned()
{
    auto dc = Services::GetDrawingControl2();
    dc->RemoveDrawable( this );

    RealComponent::OnDespawned();
}


bool SDLBillboardComponent::
OnCreateContext()
{

}


bool SDLBillboardComponent::
OnDestroyContext()
{

}
