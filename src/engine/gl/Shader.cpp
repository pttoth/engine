#include "engine/gl/Shader.h"

#include "engine/gl/GlWrapper.h"
#include "engine/Utility.h"
#include "pt/guard.hpp"
#include "pt/logging.h"

#include "assert.h"

const std::string gEmptyString;

const std::string gStubVertexShaderCode=R"(
    #version 330

    void main() {
        gl_Position = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
    }
)";

const std::string gStubGeometryShaderCode=R"(
    #version 330 core

    layout (triangles) in;
    layout (triangle_strip, max_vertices = 3) out;

    void main() {
    }
)";

const std::string gStubFragmentShaderCode=R"(
    #version 330

    out vec4 fragout_color;

    void main() {
        fragout_color = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
    }
)";


const std::string&
GetStubCodeForShaderType( engine::gl::ShaderType type )
{
    switch( type ){
    case engine::gl::ShaderType::VERTEX_SHADER:     return gStubVertexShaderCode;
    case engine::gl::ShaderType::GEOMETRY_SHADER:   return gStubGeometryShaderCode;
    case engine::gl::ShaderType::FRAGMENT_SHADER:   return gStubFragmentShaderCode;
    default:                                        return gEmptyString;
    }
    return gEmptyString;
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


ShaderPtr Shader::
CreateFromFile( const std::string& name, gl::ShaderType type, const std::string& path )
{
    if( 0 == name.length() ){
        PT_LOG_ERR( "Created shader with empty name!" );
        assert( 0 < name.length() );
    }

    ShaderPtr instance  = ShaderPtr( new Shader() );
    instance->mName     = name;
    instance->mType     = type;

    LoadCodeFromFile( instance.get(), path );
    return instance;
}


ShaderPtr Shader::
CreateStubShader( ShaderType type )
{
    assert( gl::ShaderType::NO_SHADER_TYPE != type );
    PT_LOG_ERR( "Creating stub shader of type '" << GetShaderTypeAsString( type ) << "'" );

    ShaderPtr instance  = ShaderPtr( new Shader() );
    instance->mName     = "";
    instance->mType     = type;
    instance->mIsStub   = true;
    instance->mSourceCode = GetStubCodeForShaderType( type );

    return instance;
}


bool Shader::
Compile()
{
    return CompileOrCompileStub();
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


const std::string& Shader::
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
ReloadCodeFromFile()
{
    PT_LOG_INFO( "Reloading code for shader '" << this->GetName() << "'" );
    LoadCodeFromFile( this, mPath );
}


Shader::
Shader()
{}


bool Shader::
CompileOrCompileStub()
{
    bool primary_success    = true;    // first compilation attempt succeeded
    bool stub_success       = false;   // failsafe compilation (stub) succeeded

    if( !mIsStub ){
        primary_success = CompileParameterized( false );
        if( primary_success ){
            return true;
        }else{
            mIsStub = true;
        }
    }

    stub_success = CompileParameterized( true );

    // if the fallback (stub) compiled well, the result can still be false for the first attempt
    return primary_success && stub_success;
}


bool Shader::
CompileParameterized( bool ignore_stub_logging )
{
    if( !ignore_stub_logging && mIsStub ){
        PT_LOG_ERR( "Compiling stub shader! (" << GetPropertiesAsString( *this ) << ")" );
    }

    if( 0 == mHandle ){
        PT_LOG_DEBUG( "Creating shader object (" << GetPropertiesAsString( *this ) << ")" );
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

        PT_LOG_OUT( "Compiling shader  - (" << mHandle << ", " << GetPropertiesAsString_Short( *this ) << ")" );

        const std::string&  sourcecodestr   = GetCodeOrStubCode();
        const GLchar*       sourcecode      = sourcecodestr.c_str();
        const GLint         length          = sourcecodestr.length();

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
LoadCodeFromFile( Shader* shader, const std::string& path )
{
    shader->FreeVRAM();
    shader->mIsStub             = false;
    shader->mPath               = path;

    std::ifstream ifs( path );
    if( !ifs.is_open() ){
        PT_LOG_ERR( "Failed to load shader code (" << GetPropertiesAsString( *shader )  << "). Could not open file for reading." );
    }else{
        std::stringstream ss;
        ReadSourceFile( ifs, ss );
        if( 0 == ss.str().length() ){
            PT_LOG_ERR( "Failed to load shader code (" << GetPropertiesAsString( *shader )  << "). Empty file." );
        }else{
            shader->mSourceCode = ss.str();
            PT_LOG_INFO( "Loaded shader code (" << GetPropertiesAsString( *shader )  << ")" );
            return;
        }
    }

    PT_LOG_ERR( "Loaded stub code for shader (" << GetPropertiesAsString( *shader )  << ")" );
    shader->mIsStub = true;
}


std::string Shader::
GetPropertiesAsString_Short( Shader& shader )
{
    std::stringstream ss;
    ss << "name: '" << shader.mName << "', type: " << gl::GetShaderTypeAsString(shader.mType) << "'";
    return ss.str();
}


std::string Shader::
GetPropertiesAsString( Shader& shader )
{
    std::stringstream ss;
    ss << "name: '" << shader.mName << "', type: " << gl::GetShaderTypeAsString(shader.mType) << ", path: '" << shader.mPath << "'";
    return ss.str();
}


const std::string& Shader::
GetCodeOrStubCode() const
{
    if( mIsStub ){
        return GetStubCodeForShaderType( mType );
    }
    return mSourceCode;
}
