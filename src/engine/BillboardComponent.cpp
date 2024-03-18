#include "engine/BillboardComponent.h"

#include "engine/gl/Def.h"
#include "engine/gl/ShaderProgram.h"
#include "engine/Camera.h"
#include "engine/DrawingControl.h"
#include "engine/Services.h"
#include "engine/Utility.h"
#include "pt/utility.hpp"

using namespace engine;
using namespace math;

bool BillboardComponent::stInitialized = false;
gl::Buffer<unsigned int> BillboardComponent::stIndexBuffer = gl::Buffer<unsigned int>();
const pt::Name BillboardComponent::stNameM = pt::Name( "M" );

gl::Buffer<unsigned int> InitIndices()
{
    PT_LOG_DEBUG( "Initializing BillboardComponent indexbuffer" );
    //TODO: remove this
    gl::Buffer<unsigned int> indices = { 0, 1, 2, 1, 3, 2,
                                         0, 2, 1, 1, 2, 3,
                                         4, 6, 5, 5, 6, 7,
                                         4, 5, 6, 5, 7, 6 };
    //--------------------------------------------------


//    gl::Buffer<unsigned int> indices = { 0, 1, 2, 1, 3, 2 };
    //gl::Buffer<unsigned int> indices = { 0, 2, 1, 2, 3, 1 }; // inverted normal
    indices.LoadToVRAM( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
    return indices;
}



BillboardComponent::
BillboardComponent( const std::string& name ):
    RealComponent( name )
{}


bool BillboardComponent::
Initialize()
{
    if( !stInitialized ){
        stIndexBuffer = InitIndices();
        stNameM.Init();
        stInitialized = true;
    }
    return stInitialized;
}


void BillboardComponent::
OnDraw( float t, float dt )
{
    auto dc = Services::GetDrawingControl();
    auto cam = dc->GetMainCamera();
    auto shaderProgram = dc->GetDefaultShaderProgram();

    shaderProgram->Use();
    //shaderProgram->SetUniformModelMatrix( this->GetTransform() );
    //shaderProgram->SetUniformModelViewProjectionMatrix( CalcMVP( *this, *cam.get() ) );


    static vec3 posdelta = vec3( 0.0f, 0.0f, 0.0f );
    //posdelta = vec3( sinf( t * M_PI/6 ), 0.0f, 0.0f );

    static float Xangle = 0.0f;
    static float Zangle = 0.0f;
    Zangle = 1.0f * t * 5;
    Xangle = 1.0f * t * 5;
    mat4 Zrot = mat4::rotation( vec3::zUnit, Zangle );


    mat4 modelScale = CalcScaleMtx( vec3::one );
    //mat4 modelRotate = Zrot;
    //mat4 modelRotate = engine::exp::FRotator( Zangle, 0.0f, 0.0f ).GetTransform();
    //mat4 modelRotate = engine::exp::FRotator( 0.0f, Zangle, 0.0f ).GetTransform();
    mat4 modelRotate = engine::exp::FRotator( 0.0f, 0.0f, Zangle ).GetTransform();
    mat4 modelTranslate = CalcTranslationMtx( posdelta );


    vec3 camPos;
    camPos.x = 0;
    camPos.y = -5;
    camPos.z = 0;
    mat4 camTranslate = CalcTranslationMtx( camPos );
    //mat4 camRotMtx = CalcRotMtx( vec3::yUnit, vec3::zUnit );
    mat4 camRotMtx = CalcRotMtx( vec3::xUnit, vec3::zUnit );
    mat4 viewMtx = camRotMtx * camTranslate;


    //this->SetOrientation(  Zrot * vec4(vec3::yUnit, 1.0f) );
    //this->SetOrientation( FRotator( Zangle, 0.0f, 0.0f ) );

    auto pvm =
            cam->GetProjMtx()       // Projection
            //* cam->GetRotationMtx()     // (View) Rotation
            //* CalcTranslationMtx( cam->GetPosition() )    // (View) Translation
            * cam->GetViewMtx()     // (View) Rotation + Translation
            * modelTranslate        // (Model) Translation
            * modelRotate        // (Model) Rotation
            * modelScale        // (Model) Scale
            ;
    shaderProgram->SetUniformModelViewProjectionMatrix( pvm );
    static int count = 0;
    if( 0 == count%180 ){
        PT_LOG_DEBUG( "MVP:\n" << ToString( pvm ) );
    }
    ++count;

    if( mTexture ){
        auto dc = Services::GetDrawingControl();
        auto texunit = dc->GetTextureUnit( mTexture );
        gl::ActiveTexture( texunit );
        gl::BindTexture( GL_TEXTURE_2D, mTexture->GetHandle() );
    }else{
        // TODO: set FixColor display or missing texture
        assert( false );
    }

    //gl::BindTexture( GL_TEXTURE_2D, 0 );

    static int temp = 0;
    if( 0 == temp % 60 ){
        //PT_LOG_DEBUG( "Drawing(" << temp << ")" );
    }
    ++temp;

    gl::EnableVertexAttribArray( 0 );
    gl::EnableVertexAttribArray( 1 );
    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mVertexBuffer );
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, stIndexBuffer );
    gl::Vertex::SetPositionAttributePointer( 0 );
    gl::Vertex::SetTexelAttributePointer( 1 );

    //TODO: remove this
    gl::DrawElements( gl::DrawMode::TRIANGLES, 4*3, GL_UNSIGNED_INT, 0 );
    static const pt::Name tname = pt::Name("t");
    static gl::Uniform<float> uni = shaderProgram->GetUniform<float>( tname);
    shaderProgram->SetUniform( uni, t/2) ;
    gl::DrawElements( gl::DrawMode::TRIANGLES, 4*3, GL_UNSIGNED_INT, (void*)(sizeof(int)*4*3) );
    //-----
    //gl::DrawElements( gl::DrawMode::TRIANGLES, 2*3, GL_UNSIGNED_INT, 0 ); // draw 2 triangles with 3-3 vertices

    gl::DisableVertexAttribArray( 1 );
    gl::DisableVertexAttribArray( 0 );
    GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
    GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );
}


void BillboardComponent::
SetWidth( float width )
{
    mWidth = width;
}


void BillboardComponent::
SetHeight( float height )
{
    mHeight = height;
}


void BillboardComponent::
SetSize( float width, float height )
{
    mWidth  = width;
    mHeight = height;
}


void BillboardComponent::
SetTexture( gl::Texture2dPtr texture )
{
    mTexture = texture;
}


void BillboardComponent::
OnSpawned()
{
    //add this component to the drawing manager
    auto dc = Services::GetDrawingControl();
    dc->AddDrawable( this );
}


void BillboardComponent::
OnDespawned()
{
    //remove this component from the drawing manager
    auto dc = Services::GetDrawingControl();
    dc->RemoveDrawable( this );
}


void BillboardComponent::
OnTick( float t, float dt )
{}


bool BillboardComponent::
OnCreateContext()
{
    // No need for mutex here, Init is only done once.
    //  So no late-init here, rather an error msg to fix it.
    if( !stInitialized ){
        PT_LOG_ERR( "BillboardComponent is uninitialized!" );
        return false;
    }

    InitVertexData();
    mVertexBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER,
                              gl::BufferHint::STATIC_DRAW );

    if( mTexture && (not mTexture->IsLoadedInVRAM()) && mTexture->IsLoadedInRAM() ){
        mTexture->LoadToVRAM( gl::BufferTarget::TEXTURE_BUFFER,
                              gl::BufferHint::STATIC_DRAW );
    }

    return true;
}


bool BillboardComponent::
OnDestroyContext()
{
    mVertexBuffer.FreeVRAM();
    return true;
}


void BillboardComponent::
InitVertexData()
{
    //TODO: temp: remove this!
    if( 0 == mVertexBuffer.GetSize() ){
        //set up billboard mesh (6 vertexes, 2 triangles)
        std::vector<gl::Vertex> data;
        data.reserve(8);
        float w = mWidth/2;
        float h = mHeight/2;

        gl::Vertex vx_topleft( math::float3( -w, 0, h ), // position
                               math::float2( 0.0f, 1.0f ),  // texel
                               math::float3() );            // normal
        gl::Vertex vx_topright( math::float3( w, 0, h ), // position
                                math::float2( 1.0f, 1.0f ), // texel
                                math::float3() );           // normal
        gl::Vertex vx_bottomleft( math::float3( -w, 0, -h ), // position
                                  math::float2( 0.0f, 0.0f ),   // texel
                                  math::float3() );             // normal
        gl::Vertex vx_bottomright( math::float3( w, 0, -h ),// position
                                   math::float2( 1.0f, 0.0f ), // texel
                                   math::float3() );           // normal


        gl::Vertex vxA( math::float3( -w*2/3, 1.2f, h*2/3 ), // position
                        math::float2( 0.0f, 1.0f ),          // texel
                        math::float3() );                    // normal
        gl::Vertex vxB( math::float3( w*1/3, 1.2f, h*2/3 ), // position
                        math::float2( 1.0f, 1.0f ),         // texel
                        math::float3() );                   // normal
        gl::Vertex vxC( math::float3( -w*2/3, 1.2f, -h*2/3 ), // position
                        math::float2( 0.0f, 0.0f ),           // texel
                        math::float3() );                     // normal
        gl::Vertex vxD( math::float3( w*3/3, 1.2f, -h*2/3 ),// position
                        math::float2( 1.0f, 0.0f ),         // texel
                        math::float3() );                   // normal

/*
        gl::Vertex vx_topleft( math::float3( -w, h, 0 ), // position
                               math::float2( 0.0f, 1.0f ),  // texel
                               math::float3() );            // normal
        gl::Vertex vx_topright( math::float3( w, h, 0 ), // position
                                math::float2( 1.0f, 1.0f ), // texel
                                math::float3() );           // normal
        gl::Vertex vx_bottomleft( math::float3( -w, -h, 0 ), // position
                                  math::float2( 0.0f, 0.0f ),   // texel
                                  math::float3() );             // normal
        gl::Vertex vx_bottomright( math::float3( w, -h, 0 ),// position
                                   math::float2( 1.0f, 0.0f ), // texel
                                   math::float3() );           // normal

        gl::Vertex vxA( math::float3( -w*2/3, h*2/3, 1.5f ), // position
                        math::float2( 0.0f, 1.0f ),          // texel
                        math::float3() );                    // normal
        gl::Vertex vxB( math::float3( w*2/3, h*2/3, 1.5f ), // position
                        math::float2( 1.0f, 1.0f ),         // texel
                        math::float3() );                   // normal
        gl::Vertex vxC( math::float3( -w*2/3, -h*2/3, 1.5f ), // position
                        math::float2( 0.0f, 0.0f ),           // texel
                        math::float3() );                     // normal
        gl::Vertex vxD( math::float3( w*2/3, -h*2/3, 1.5f ),// position
                        math::float2( 1.0f, 0.0f ),         // texel
                        math::float3() );                   // normal
*/

        data.push_back( vx_topleft );
        data.push_back( vx_topright );
        data.push_back( vx_bottomleft );
        data.push_back( vx_bottomright );
        data.push_back( vxA );
        data.push_back( vxB );
        data.push_back( vxC );
        data.push_back( vxD );

        mVertexBuffer = std::move( data );
    }


    return;
    //--------------------------------------------------
    // ORIGINAL
/*
    if( 0 == mVertexBuffer.GetSize() ){
        //set up billboard mesh (6 vertexes, 2 triangles)
        std::vector<gl::Vertex> data;
        data.reserve(4);
        float w = mWidth/2;
        float h = mHeight/2;
        gl::Vertex vx_topleft( math::float3( -w, h, 0.0f ), // position
                               math::float2( 0.0f, 1.0f ),  // texel
                               math::float3() );            // normal
        gl::Vertex vx_topright( math::float3( w, h, 0.0f ), // position
                                math::float2( 1.0f, 1.0f ), // texel
                                math::float3() );           // normal
        gl::Vertex vx_bottomleft( math::float3( -w, -h, 0.0f ), // position
                                  math::float2( 0.0f, 0.0f ),   // texel
                                  math::float3() );             // normal
        gl::Vertex vx_bottomright( math::float3( w, -h, 0.0f ),// position
                                   math::float2( 1.0f, 0.0f ), // texel
                                   math::float3() );           // normal
        data.push_back( vx_topleft );
        data.push_back( vx_topright );
        data.push_back( vx_bottomleft );
        data.push_back( vx_bottomright );

        mVertexBuffer = std::move( data );
    }
*/
}



engine::exp::FRotator::
FRotator( float yaw, float pitch, float roll ):
    mYaw( yaw ), mPitch( pitch ), mRoll( roll )
{}


engine::exp::FRotator::
FRotator( const float3& values ):
    FRotator( values.x, values.y, values.z )
{}


float4x4 engine::exp::FRotator::
GetTransform() const
{
    math::float4x4 yawMtx = math::float4x4::identity;
    math::float4x4 pitchMtx = math::float4x4::identity;
    math::float4x4 rollMtx = math::float4x4::identity;

    float cosf_mYaw     = cosf( mYaw );
    float sinf_mYaw     = sinf( mYaw );
    float cosf_mPitch   = cosf( mPitch );
    float sinf_mPitch   = sinf( mPitch );
    float cosf_mRoll    = cosf( mRoll );
    float sinf_mRoll    = sinf( mRoll );

    yawMtx.m[0][0] = cosf_mYaw;
    yawMtx.m[0][1] = sinf_mYaw * -1;
    yawMtx.m[1][0] = sinf_mYaw;
    yawMtx.m[1][1] = cosf_mYaw;
    pitchMtx.m[0][0] = cosf_mPitch;
    pitchMtx.m[0][2] = sinf_mPitch;
    pitchMtx.m[2][0] = sinf_mPitch * -1;
    pitchMtx.m[2][2] = cosf_mPitch;
    rollMtx.m[1][1] = cosf_mRoll;
    rollMtx.m[1][2] = sinf_mRoll * -1;
    rollMtx.m[2][1] = sinf_mRoll;
    rollMtx.m[2][2] = cosf_mRoll;

    return yawMtx * pitchMtx * rollMtx;
    //return (yawMtx * pitchMtx * rollMtx).transpose();


    // this is the resulting matrix from the above matrix multiplications
    math::float4x4 transform = math::float4x4::identity;
    transform.m[0][0] = cosf_mPitch * cosf_mRoll;
    transform.m[0][1] = sinf_mYaw * sinf_mPitch * cosf_mRoll - cosf_mYaw * sinf_mRoll;
    transform.m[0][2] = cosf_mYaw * sinf_mPitch * cosf_mRoll + sinf_mYaw * sinf_mRoll;

    transform.m[1][0] = cosf_mPitch * sinf_mRoll;
    transform.m[1][1] = sinf_mYaw * sinf_mPitch * sinf_mRoll + cosf_mYaw * cosf_mRoll;
    transform.m[1][2] = cosf_mYaw * sinf_mPitch * sinf_mRoll - sinf_mYaw * cosf_mRoll;

    transform.m[2][0] = (-1) * sinf_mPitch;
    transform.m[2][1] = sinf_mYaw * cosf_mPitch;
    transform.m[2][2] = cosf_mYaw * cosf_mPitch;

    return transform;
}
