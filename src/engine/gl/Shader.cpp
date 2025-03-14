#include "engine/gl/Shader.h"

#include "engine/gl/GlWrapper.h"
#include "engine/Utility.h"
#include "pt/guard.hpp"
#include "pt/logging.h"

#include "assert.h"

engine::gl::ShaderPtr engine::gl::Shader::stStubVertexShader    = nullptr;
engine::gl::ShaderPtr engine::gl::Shader::stStubGeometryShader  = nullptr;
engine::gl::ShaderPtr engine::gl::Shader::stStubFragmentShader  = nullptr;


const char* StubVertexShaderCode=R"(
    #version 330

    void main() {
        gl_Position = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
    }
)";

const char* StubGeometryShaderCode=R"(
    #version 330 core

    layout (triangles) in;
    layout (triangle_strip, max_vertices = 3) out;

    void main() {
    }
)";

const char* StubFragmentShaderCode=R"(
    #version 330

    out vec4 fragout_color;

    void main() {
        fragout_color = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
    }
)";


const char*
GetStubCodeForShaderType( engine::gl::ShaderType type )
{
    switch( type ){
    case engine::gl::ShaderType::VERTEX_SHADER:     return StubVertexShaderCode;
    case engine::gl::ShaderType::GEOMETRY_SHADER:   return StubGeometryShaderCode;
    case engine::gl::ShaderType::FRAGMENT_SHADER:   return StubFragmentShaderCode;
    default:                                        return "";
    }
    return nullptr;
}


void
ReadSourceFile( std::ifstream& ifs, std::stringstream& ss )
{
    const size_t bufsize = 16 * 1024 + 1;
    char buf[bufsize];
    buf[bufsize-1] = 0;
    while( ifs.good() ){
        ifs.read( buf, bufsize-1 );
        size_t chars_read = ifs.gcount();
        buf[chars_read] = 0;
        ss << buf;
    }
}

//---------------------------------------------------------------------------

using namespace engine::gl;

Shader::
~Shader()
{
    FreeVRAM();
}


bool Shader::
Initialize()
{
    stStubVertexShader              = ShaderPtr( new Shader() );
    stStubVertexShader->mName       = "Stub_VertexShader";
    stStubVertexShader->mType       = gl::ShaderType::VERTEX_SHADER;
    stStubVertexShader->mPath       = "";
    stStubVertexShader->mSourceCode = StubVertexShaderCode;

    stStubGeometryShader            = ShaderPtr( new Shader() );
    stStubGeometryShader->mName     = "Stub_GeometryShader";
    stStubGeometryShader->mType     = gl::ShaderType::GEOMETRY_SHADER;
    stStubGeometryShader->mPath     = "";
    stStubGeometryShader->mSourceCode = StubGeometryShaderCode;

    stStubFragmentShader            = ShaderPtr( new Shader() );
    stStubFragmentShader->mName     = "Stub_FragmentShader";
    stStubFragmentShader->mType     = gl::ShaderType::FRAGMENT_SHADER;
    stStubFragmentShader->mPath     = "";
    stStubFragmentShader->mSourceCode = StubFragmentShaderCode;

    bool suc_vtx  = stStubVertexShader->CompileParameterized( true );
    bool suc_geom = stStubGeometryShader->CompileParameterized( true );
    bool suc_frag = stStubFragmentShader->CompileParameterized( true );

    return suc_vtx && suc_geom && suc_frag;
}


ShaderPtr Shader::
CreateFromFile( const std::string& name, gl::ShaderType type, const std::string& path )
{
    if( 0 == name.length() ){
        PT_LOG_ERR( "Tried to create shader with empty name!" );
        return nullptr;
    }

    ShaderPtr instance  = ShaderPtr( new Shader() );
    instance->mName     = name;
    instance->mType     = type;

    LoadCodeFromFile( instance, path );

    return instance;
}


bool Shader::
Compile()
{
    return CompileParameterized( false );
}


void Shader::
FreeVRAM()
{
    if( 0 != mHandle ){
        PT_LOG_DEBUG( "Deleting shader(" << mHandle << ")'" << mName << "'" );

        gl::DeleteShader( mHandle );
#ifdef ENGINE_DEBUG_ENABLED
        GLint result = GL_FALSE;
        glGetShaderiv( mHandle, GL_DELETE_STATUS, &result );
        assert( GL_INVALID_OPERATION != glGetError() );
        if( GL_TRUE == result ){
            PT_LOG_WARN( "Deleted shader '" << mHandle << "' is still bound to a shaderprogram and was only flagged for deletion!" );
        }
#endif
        mHandle = 0;
    }
}


GLuint Shader::
GetHandle() const
{
    return mHandle;
}


std::string Shader::
GetName() const
{
    return mName;
}


ShaderType Shader::
GetShaderType() const
{
    return mType;
}


const std::string& Shader::
GetSourceCode() const
{
    return mSourceCode;
}


bool Shader::
IsCompiled() const
{
    return ( 0 != mHandle );
}


bool Shader::
IsStub() const
{
    return mIsStub;
}


void Shader::
ReloadCodeFromFile( ShaderPtr shader )
{
    LoadCodeFromFile( shader, shader->mPath );
}


Shader::
Shader()
{}


bool Shader::
CompileParameterized( bool ignore_stub )
{
    // if failed compilation once, don't even try again
    if( mFailedCompilation ){
        PT_LOG_ERR( "Tried to compile shader '" << mName << "', that already failed compilation once!" );
        return false;
    }

    if( !ignore_stub && IsStub() ){
        PT_LOG_ERR( "Compiling stub shader! (" << GetDetailsAsString( *this ) << ")" );
    }

    if( 0 == mHandle ){
        PT_LOG_DEBUG( "Creating shader object (" << GetDetailsAsString( *this ) << ")" );
        GLenum  errorcode = 0;
        GLint   success   = GL_FALSE;

        // leaving the case of mType='ShaderType::NO_SHADER_TYPE' to be error-checked as OpenGL error
        mHandle = gl::CreateShader( mType );
        auto handleguard = pt::CreateGuard( [this]{
            gl::DeleteShader( mHandle );
            mHandle = 0;
        } );

        errorcode = gl::GetError();
        if( 0 == mHandle || GL_NO_ERROR != errorcode ){
            PT_LOG_ERR( "Could not create shader '" << mName
                        << "' (type: " << std::hex << mType << std::dec
                        << ")\n  (" << gl::GetErrorString( errorcode )
                        << "):\n  " << gl::GetErrorDescription( errorcode ) );
            return false;
        }

        PT_LOG_OUT( "Compiling shader  - (" << mHandle << ", " << GetShortDetailsAsString( *this ) << ")" );

        const GLchar* sourcecode = mSourceCode.c_str();
        const GLint   length     = mSourceCode.length();

        // upload source code to VRAM
        gl::ShaderSource( mHandle, 1, &sourcecode, &length );
        errorcode = gl::GetError();
        if( GL_NO_ERROR != errorcode ){
            PT_LOG_ERR( "OpenGL error: " << gl::GetErrorString( errorcode )
                        << "\n Description:  " << gl::GetErrorDescription( errorcode ) );
            return false;
        }

        gl::CompileShader( mHandle );
        gl::GetShaderiv( mHandle, GL_COMPILE_STATUS, &success);
        if( GL_FALSE == success ){
            mFailedCompilation = true;
            PT_LOG_ERR( "Failed to compile shader '" << mName << "'" );
            gl::PrintShaderInfoLog( mHandle );
            return false;
        }

        handleguard.Disable();
    }

    return true;
}


Shader::
Shader( const std::string& name, gl::ShaderType type, const std::string& code ):
    mName( name ), mType( type ), mSourceCode( code )
{}


void Shader::
LoadCodeFromFile( ShaderPtr shader, const std::string& path )
{
    shader->FreeVRAM();
    shader->mIsStub             = false;
    shader->mFailedCompilation  = false;
    shader->mPath               = path;

    std::ifstream ifs( path );
    if( !ifs.is_open() ){
        PT_LOG_ERR( "Could not load shader code from file '" << path << "'!" );
    }else{
        std::stringstream ss;
        ReadSourceFile( ifs, ss );
        if( 0 == ss.str().length() ){
            PT_LOG_ERR( "Failed to load shader code (" << GetDetailsAsString( *shader )  << ")" );
        }else{
            shader->mSourceCode = ss.str();
            PT_LOG_INFO( "Loaded shader code (" << GetDetailsAsString( *shader )  << ")" );
            return;
        }
    }

    PT_LOG_ERR( "Falling back to stub when loading shader code (" << GetDetailsAsString( *shader )  << ")" );
    shader->mSourceCode = GetStubCodeForShaderType( shader->mType );
    shader->mIsStub = true;
}


std::string Shader::
GetShortDetailsAsString( Shader& shader )
{
    std::stringstream ss;
    ss << "name: '" << shader.mName << "', type: " << gl::GetShaderTypeAsString(shader.mType) << "'";
    return ss.str();
}


std::string Shader::
GetDetailsAsString( Shader& shader )
{
    std::stringstream ss;
    ss << "name: '" << shader.mName << "', type: " << gl::GetShaderTypeAsString(shader.mType) << ", path: '" << shader.mPath << "'";
    return ss.str();
}
