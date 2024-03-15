#include "engine/BillboardComponent.h"

#include "engine/gl/Def.h"
#include "engine/gl/ShaderProgram.h"
#include "engine/Camera.h"
#include "engine/DrawingControl.h"
#include "engine/Services.h"
#include "engine/Utility.h"
#include "pt/utility.hpp"

using namespace engine;

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
    /*
    {
    static bool firsttime = true;
    static GLuint vbuf = 0;
    static GLuint ibuf = 0;
    static unsigned int Indices[] = { 0, 2, 5,
                                      3, 6, 5,
                                      4, 2, 8
                                    };
    static math::float3 Vertices[9];

    if( firsttime ){
        firsttime = false;
        Vertices[0] = math::float3(-1.0f,  1.0f, 0.0f);
        Vertices[1] = math::float3( 0.0f,  1.0f, 0.0f);
        Vertices[2] = math::float3( 1.0f,  1.0f, 0.0f);
        Vertices[3] = math::float3(-1.0f,  0.0f, 0.0f);
        Vertices[4] = math::float3( 0.0f,  0.0f, 0.0f);
        Vertices[5] = math::float3( 1.0f,  0.0f, 0.0f);
        Vertices[6] = math::float3(-1.0f, -1.0f, 0.0f);
        Vertices[7] = math::float3( 0.0f, -1.0f, 0.0f);
        Vertices[8] = math::float3( 1.0f, -1.0f, 0.0f);
        gl::GenBuffers(1, &vbuf);
        gl::BindBuffer( GL_ARRAY_BUFFER, vbuf );
        gl::BufferData( GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW );

        gl::GenBuffers(1, &ibuf);
        gl::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibuf );
        gl::BufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW );

        gl::Clear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    }

    gl::EnableVertexAttribArray( 0 );
    gl::BindBuffer( GL_ARRAY_BUFFER, vbuf );
    gl::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibuf );
    gl::VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    //gl::DrawArrays( GL_TRIANGLES, 0, 2*3 );
    //gl::DrawArrays( GL_TRIANGLES, 0, (unsigned int)( sizeof(Vertices)/4) );
    gl::DrawElements( GL_TRIANGLES, (unsigned int) (sizeof(Indices)/4), GL_UNSIGNED_INT, 0 );

    }
    return;
*/
    //------------------------------------------------------------------
/*
    {
    static bool firsttime = true;
    static gl::Buffer<unsigned int> iBuf = { 0, 2, 5,
                                             3, 6, 5,
                                             4, 2, 8
                                                    };
    static gl::Buffer<math::float3> vBuf;
    static std::vector<math::float3> Vertices;

    if( firsttime ){
        //firsttime = false;
        Vertices.reserve(200);
        Vertices.push_back( math::float3(-1.0f,  1.0f, 0.0f) );
        Vertices.push_back( math::float3( 0.0f,  1.0f, 0.0f) );
        Vertices.push_back( math::float3( 1.0f,  1.0f, 0.0f) );
        Vertices.push_back( math::float3(-1.0f,  0.0f, 0.0f) );
        Vertices.push_back( math::float3( 0.0f,  0.0f, 0.0f) );
        Vertices.push_back( math::float3( 1.0f,  0.0f, 0.0f) );
        Vertices.push_back( math::float3(-1.0f, -1.0f, 0.0f) );
        Vertices.push_back( math::float3( 0.0f, -1.0f, 0.0f) );
        Vertices.push_back( math::float3( 1.0f, -1.0f, 0.0f) );
        vBuf = Vertices;
        vBuf.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
        iBuf.LoadToVRAM( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );

        gl::Clear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    }

    gl::EnableVertexAttribArray( 0 );
    gl::BindBuffer( GL_ARRAY_BUFFER, vBuf.GetHandle() );
    gl::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, iBuf.GetHandle() );
    gl::VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    static int counter = 0;
    if( 0 == counter % 180 ){
        PT_LOG_INFO( "vBuf.GetHandle(): " << vBuf.GetHandle() );
        PT_LOG_INFO( "vBuf.GetSize():   " << vBuf.GetSize() );
        PT_LOG_INFO( "iBuf.GetHandle(): " << iBuf.GetHandle() );
        PT_LOG_INFO( "iBuf.GetSize():   " << iBuf.GetSize() );
    }
    ++counter;
    gl::DrawElements( GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0 );


    if( firsttime ){
        firsttime = false;
    }
    }

    return;
*/
    //------------------------------------------------------------------

/*
    static bool buffered = false;
    static gl::Buffer<unsigned int> stTempIndexBuffer = {0, 2, 1,
                                                         1, 2, 3,
                                                         0, 1, 2,
                                                         1, 3, 2};
    // temporary data: screen space coordinates
    float w = 1.5f;
    float h = 1.5f;
    float zPos = sinf(t/4) * 5;
    //float zPos = 0.0f;
    static gl::Buffer<math::float3> vbuf = { math::float3( -w/2,  h/2, zPos ),
                                             math::float3(  w/2,  h/2, zPos ),
                                             math::float3( -w/2, -h/2, zPos ),
                                             math::float3(  w/2, -h/2, zPos ) };
    if( !buffered ){
        //move this out of here (need to update position per frame)
        vbuf.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );


        stTempIndexBuffer.LoadToVRAM( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
        buffered = true;
    }

    auto draw = Services::GetDrawingControl();
    auto shp = draw->GetDefaultShaderProgram();


    shp->SetUniformModelMatrix( math::float4x4::identity );
    shp->SetUniformModelViewProjectionMatrix( math::float4x4::identity );
    shp->SetUniformRotationMatrix( math::float4x4::identity );
    shp->SetUniformViewMatrix( math::float4x4::identity );
    shp->SetUniformViewProjectionMatrix( math::float4x4::identity );


    //gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mVertexBuffer );
    //gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, vbuf.GetHandle() );
    //gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, stTempIndexBuffer.GetHandle() );
    gl::BindBuffer( GL_ARRAY_BUFFER, vbuf );
    gl::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, stTempIndexBuffer );
    gl::EnableVertexAttribArray( 0 );
    gl::VertexAttribPointer( 0, 3,
                             GL_FLOAT, gl::SKIP_NORMALIZE,
                             sizeof(math::float3), PT_GL_VERTEX_OFFSET_POSITION );
    static int counter = 0;
    if( 0 == counter % 180 ){
        PT_LOG_INFO( "vBuf.GetHandle(): " << vbuf.GetHandle() );
        PT_LOG_INFO( "vBuf.GetSize():   " << vbuf.GetSize() );
        PT_LOG_INFO( "iBuf.GetHandle(): " << stTempIndexBuffer.GetHandle() );
        PT_LOG_INFO( "iBuf.GetSize():   " << stTempIndexBuffer.GetSize() );
    }
    ++counter;

    gl::DrawElements( gl::DrawMode::TRIANGLES, 4*3, GL_UNSIGNED_INT, 0 );
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, 0 );
    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, 0 );

    return;
*/
    //--------------------------------------------------

    using mat4 = math::float4x4;
    using vec4 = math::float4;
    using vec3 = math::float3;
    using f3 = math::float3;

    auto dc = Services::GetDrawingControl();
    auto cam = dc->GetMainCamera();
    auto shaderProgram = dc->GetDefaultShaderProgram();

    mat4 tf = mat4::identity;
    f3 v = f3( 0.5f, 0.3f, 0.7f );
    //PT_LOG_INFO( "pos: \n" << ToString( v ) );
    //PT_LOG_INFO( "transform: \n" << ToString(tf) );

    //SetPosition( v );

    tf = GetTransform();
    //PT_LOG_INFO( "pos: \n" << ToString( v ) );
    //PT_LOG_INFO( "transform: \n" << ToString(tf) );

    shaderProgram->Use();
    //shaderProgram->SetUniformModelMatrix( this->GetTransform() );
    //shaderProgram->SetUniformModelViewProjectionMatrix( CalcMVP( *this, *cam.get() ) );

    //SetPosition( math::Vec3() );

    static vec3 posdelta = vec3( 0.0f, 0.0f, 0.0f );

    posdelta = vec3( sinf( t * M_PI/6 ), 0.0f, 0.0f );

    //SetPosition( posdelta );
    //SetOrientation( math::Vec4( 0.2f, 0.2f, 0.5f, 0.0f ) );
    //SetScale( math::Vec3( 0.5f, 0.5f, 0.5f ) );

    const vec3 Zup = vec3( 0.0f, 0.0f, 1.0f );

    static float Xangle = 0.0f;
    static float Zangle = 0.0f;
    Zangle = 1.0f * t;
    Xangle = 1.0f * t;
    mat4 Zrot = mat4::rotation( vec3::zUnit, Zangle );


    mat4 modelScale = CalcScaleMtx( vec3::one );
    //mat4 modelRotate = CalcRotMtx( posdelta, Zup );
    //mat4 modelRotate = CalcRotMtx( vec3(0.0f, 1.0f, 0.0f), Zup );
    //mat4 modelRotate = mat4::rotation( vec3(0.0f, 0.0f, 1.0f), 0 );
    mat4 modelRotate = Zrot;
    mat4 modelTranslate = CalcTranslationMtx( posdelta );

    //mat4 modelTransform =
    //shaderProgram->SetUniformModelMatrix( modelTranslate * modelRotate * modelScale; );

    //shaderProgram->SetUniformModelMatrix( this->GetTransform() );


    vec3 camPos;
    camPos.x = 0;
    camPos.y = -5;
    camPos.z = 0;
    mat4 camTranslate = CalcTranslationMtx( camPos );
    mat4 camRotMtx = CalcRotMtx( vec3( 0.0f, 1.0f, 0.0f ), vec3(0.0f, 0.0f, 1.0f) );
    mat4 viewMtx = camRotMtx * camTranslate;


    this->SetOrientation(  Zrot * vec4(vec3::yUnit, 1.0f) );

    auto pvm =
            cam->GetProjMtx()       // Projection
            //* viewMtx

            //* cam->GetRotationMtx()     // (View) Rotation
            //* camRotMtx     // (View) Rotation


            //* cam->GetRotationMtx()
            //* CalcTranslationMtx( cam->GetPosition() )
            //* camTranslate                // (View) Translation

            * cam->GetViewMtx()     // (View) Rotation


            //* cam->GetViewMtx()     // (View) Rotation
            //* camPos                // (View) Translation
            * modelTranslate        // (Model) Translation
            * modelRotate        // (Model) Rotation
            * modelScale        // (Model) Scale
            ;
    shaderProgram->SetUniformModelViewProjectionMatrix( pvm );
    static int count = 0;
    if( 0 == count%180 ){
        PT_LOG_DEBUG( "MVP:\n" << ToString( pvm ) );
    }

    //shaderProgram->SetUniformModelViewProjectionMatrix(  )
    //shaderProgram->SetUniformModelViewProjectionMatrix( CalcMVP( *this, *cam.get() ) );

    //static int count = 0;
    if( 0 == count%180 ){
        //PT_LOG_DEBUG( "Transform:\n" << ToString( this->GetTransform() ) );
        //PT_LOG_DEBUG( "MVP:\n" << ToString( CalcMVP( *this, *cam.get() ) ) );
    }
    ++count;


    //assert(false);




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
        PT_LOG_DEBUG( "Drawing(" << temp << ")" );
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
