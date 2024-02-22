#include "ShaderProgram.h"

#include "engine/gl/GlWrapper.h"

#include "pt/logging.h"
#include "pt/utility.hpp"

#include <thread>
#include <vector>
#include <sstream>
#include <assert.h>

using namespace engine;
using namespace pt;


std::vector<char>
GetObjectInfoLog( GLuint object )
{
    bool isshader = (GL_TRUE == gl::IsProgram( object ) );
    bool isprogram = (GL_TRUE == gl::IsShader( object ) );
    assert( isshader || isprogram );

    //get info log string size
    GLsizei infolog_length = 0;
    GLsizei returned_length = 0;
    std::vector<char> buffer;

    if( isshader )
    {
        gl::GetShaderiv( object, GL_INFO_LOG_LENGTH, &infolog_length );
        assert( GL_NO_ERROR == gl::GetError() );

        //TODO: check how big a buffer we need here!
        buffer.resize( infolog_length+1 );
        gl::GetShaderInfoLog( object, infolog_length, &returned_length, buffer.data() );
    }else if ( isprogram ){
        gl::GetProgramiv( object, GL_INFO_LOG_LENGTH, &infolog_length );
        assert( GL_NO_ERROR == gl::GetError() );

        //TODO: check how big a buffer we need here!
        buffer.resize( infolog_length+1 );
        gl::GetProgramInfoLog( object, infolog_length, &returned_length, buffer.data() );
    }else{
        const char* errorstr = "Unable to fetch info log";
        buffer.resize( strlen( errorstr ) + 1);
        strcpy( buffer.data(), errorstr );
        buffer[ buffer.size()-1 ] = '\0';
        return buffer;
    }

    buffer.resize( returned_length +1 ); //shrink buffer to correct size (no realloc happens)
    buffer[ buffer.size()-1 ] = '\0'; //TODO: need this?

    return buffer;
}


ShaderProgram::
ShaderProgram(const std::string& vertex_source,
              const std::string& geometry_source,
              const std::string& fragment_source):
    //initialize vectors with one-element initializer lists
    mVertSources( {vertex_source} ),
    mGeomSources( {geometry_source} ),
    mFragSources( {fragment_source} )
{}


ShaderProgram::
ShaderProgram(const std::vector<std::string>& vertex_sources,
              const std::vector<std::string>& geometry_sources,
              const std::vector<std::string>& fragment_sources):
    mVertSources( vertex_sources ),
    mGeomSources( geometry_sources ),
    mFragSources( fragment_sources )
{
    if( 0 == vertex_sources.size() ){
        throw std::invalid_argument("Tried to initialize ShaderProgram without Vertex Shader");
    }
}


ShaderProgram::
~ShaderProgram()
{
    if( 0 < mHandleProgram ){
        gl::DeleteProgram( mHandleProgram );
        if( GL_NO_ERROR != gl::GetError() ){
            pt::log::err << "Possible GPU memory leak! 'glDeleteProgram' resulted in error for handle: " << mHandleProgram << "\n";
        }
    }

    for( GLuint s : mVertHandles ){
        gl::DeleteShader( s );
        if( GL_NO_ERROR != gl::GetError() ){
            pt::log::err << "Possible GPU memory leak! 'glDeleteShader' resulted in error for vertex shader handle: " << s << "\n";
        }
    }

    for( GLuint s : mGeomHandles ){
        gl::DeleteShader( s );
        if( GL_NO_ERROR != gl::GetError() ){
            pt::log::err << "Possible GPU memory leak! 'glDeleteShader' resulted in error for geometry shader handle: " << s << "\n";
        }
    }

    for( GLuint s : mFragHandles ){
        gl::DeleteShader( s );
        if( GL_NO_ERROR != gl::GetError() ){
            pt::log::err << "Possible GPU memory leak! 'glDeleteShader' resulted in error for fragment shader handle: " << s << "\n";
        }
    }
}


void ShaderProgram::
CreateContext()
{
    {
        std::lock_guard<std::mutex> guard( mMutex );
        if( mInitialized ){
            if( mInitializeFailed ){
                throw std::logic_error("Called .Create() on a ShaderProgram that already failed creation");
            }
            throw std::logic_error("Called .Create() on a ShaderProgram that has already been initialized");
        }
    }

    /*
    std::thread t( []()
    {
        //move here the code below
    } );

    t.detach();
    */

    try{
        std::lock_guard<std::mutex> guard( mMutex );
        mInitialized = true;
        mInitializeFailed = true;
        Compile();
        Link();
        mInitializeFailed = false;
    }catch(const std::exception& e){
        std::stringstream ss;
        ss << "ShaderProgram compiler thread has encountered an exception!\n";
        ss << "  e.what(): " << e.what();
        ss << "\n";
        pt::log::err << ss.str();
    }catch(...){
        pt::log::err << "ShaderProgram compiler thread threw an unknown object!\n";
    }
}


void ShaderProgram::
Use()
{
    std::lock_guard<std::mutex> guard( mMutex );

    if( !mInitialized ){
        throw std::logic_error( "Tried to Use() an uninitialized shader" );
    }
    if( mInitializeFailed ){
        throw std::runtime_error( "Tried to Use() a shader that failed initialization" );
    }

    gl::UseProgram( mHandleProgram );
    GLenum error = gl::GetError();
    assert( GL_NO_ERROR == error );

    //Not all cases are handled here.
    //The rest must never appear because we supply controlled, clean data as parameters.
    if( GL_INVALID_OPERATION == error ){
        throw std::logic_error("Could not make program object part of the current state. ('transform feedback' mode active?)");
    }
}


bool ShaderProgram::
IsContextCreated() const
{
    std::lock_guard<std::mutex> guard( mMutex );
    return (mInitialized && !mInitializeFailed);
}


bool ShaderProgram::
IsInUse() const
{
    std::lock_guard<std::mutex> guard( mMutex );

    if( !mInitialized || mInitializeFailed || (0 == mHandleProgram) ){
        return false;
    }

    GLint current_prog = 0;
    gl::GetIntegerv( GL_CURRENT_PROGRAM, &current_prog );
    GLenum error = gl::GetError();
    assert( GL_NO_ERROR == error );


    return (current_prog == mHandleProgram);
}


void ShaderProgram::
GetProgramIV(GLuint program, GLenum pname, GLint *params)
{
    std::lock_guard<std::mutex> guard( mMutex );
    return gl::GetProgramiv(program, pname, params);
}

const std::vector<std::string>&
ShaderProgram::GetShaders(GLenum shadertype)
{
    switch(shadertype){
    case GL_VERTEX_SHADER:
        return mVertSources;
    case GL_GEOMETRY_SHADER:
        return mGeomSources;
    case GL_FRAGMENT_SHADER:
        return mFragSources;
    }
    assert(false);
    throw std::invalid_argument("Invalid shader type");
}


void ShaderProgram::
CompileShadersOfType(GLenum shadertype, const char *shadertype_as_cstring)
{
    const std::vector<std::string>& shaders = GetShaders( shadertype );

    //for each shader of given type
    for( size_t i=0; i<shaders.size(); ++i )
    {
        const std::string& s = shaders[i];

        int         shader_length   = static_cast<int>( s.length() );
        const char* shader_source   = s.c_str();
        GLuint      shader_obj      = gl::CreateShader( shadertype );

        assert( GL_NO_ERROR == gl::GetError() );

        if( 0 == shader_obj ){
            std::stringstream ss;
            ss << "Could not create " << shadertype_as_cstring <<  " [" << i << "]";

            std::vector<char> buffer = GetObjectInfoLog( shader_obj );

            pt::log::err << ss.str() << "\n";
            pt::log::err << "  log:\n";
            pt::log::err << buffer.data() << "\n";

            throw std::runtime_error( ss.str() );
        }

        mVertHandles.push_back( shader_obj );

        //set source code for shader object
        gl::ShaderSource( shader_obj, 1, &(shader_source), &shader_length );
        assert( GL_NO_ERROR == gl::GetError() );

        //compile shader
        GLint success;
        gl::CompileShader( shader_obj );
        gl::GetShaderiv( shader_obj, GL_COMPILE_STATUS, &success );
        assert( GL_NO_ERROR == gl::GetError() );

        if ( GL_FALSE == success ) {
            std::vector<char> buffer = GetObjectInfoLog( shader_obj );

            std::stringstream ss;
            ss << EnumToString(shadertype) << "[" << i << "] compilation failed";

            pt::log::err << ss.str() << "\n";
            pt::log::err << "  log:\n";
            pt::log::err << buffer.data() << "\n";

            throw std::runtime_error( ss.str() );
        }
    }
}


void ShaderProgram::
Compile()
{
    CompileShadersOfType( GL_VERTEX_SHADER,   EnumToString(GL_VERTEX_SHADER ) );
    CompileShadersOfType( GL_FRAGMENT_SHADER, EnumToString(GL_FRAGMENT_SHADER) );
    CompileShadersOfType( GL_GEOMETRY_SHADER, EnumToString(GL_GEOMETRY_SHADER) );
}


void ShaderProgram::
Link()
{
    //create shader program object
    mHandleProgram = gl::CreateProgram();
    if( 0 == mHandleProgram ){
        throw std::runtime_error("Could not create shader program");
    }

    //add shaders to program object
    for(auto s : mVertHandles){
        gl::AttachShader( mHandleProgram, s );
        assert( GL_NO_ERROR == gl::GetError() );
    }

    for(auto s : mGeomHandles){
        gl::AttachShader( mHandleProgram, s );
        assert( GL_NO_ERROR == gl::GetError() );
    }

    for(auto s : mFragHandles){
        gl::AttachShader( mHandleProgram, s );
        assert( GL_NO_ERROR == gl::GetError() );
    }

    //link program
    gl::LinkProgram( mHandleProgram );
    assert( GL_NO_ERROR == gl::GetError() );

    int success = GL_FALSE;
    gl::GetProgramiv( mHandleProgram, GL_LINK_STATUS, &success );
    assert( GL_NO_ERROR == gl::GetError() );
    if ( GL_FALSE == success ) {
        std::vector<char> buffer = GetObjectInfoLog( mHandleProgram );

        std::stringstream ss;
        ss << "Failed to link shader program";

        pt::log::err << ss.str() << "!\n";
        pt::log::err << "  log:\n";
        pt::log::err << buffer.data() << "\n";

        throw std::runtime_error( ss.str() );
    }

    gl::ValidateProgram( mHandleProgram );
    assert( GL_NO_ERROR == gl::GetError() );

    success = GL_FALSE;
    gl::GetProgramiv( mHandleProgram,  GL_VALIDATE_STATUS, &success );
    assert( GL_NO_ERROR == gl::GetError() );

    if ( GL_FALSE == success ) {
        std::vector<char> buffer = GetObjectInfoLog( mHandleProgram );

        std::stringstream ss;
        ss << "Failed to validate shader program";

        pt::log::err << ss.str() << "!\n";
        pt::log::err << "  log:\n";
        pt::log::err << buffer.data() << "\n";

        throw std::runtime_error( ss.str() );
    }
}
