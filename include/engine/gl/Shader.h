/** -----------------------------------------------------------------------------
  * FILE:    Shader.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Represents and handles a shader's functionality and lifecycle.
  *     - A Shader with type 'gl::ShaderType::NO_SHADER_TYPE' should not be passed to GL functions!
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/gl/Def.h"
#include "pt/alias.h"
#include "pt/macros.h"
#include "pt/name.h"

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( Shader )

class Shader
{
public:
    virtual ~Shader();

    static bool         Initialize();

    // On error, returns nullptr if 'name' is empty
    //  ...or a stub in other cases
    static ShaderPtr    CreateFromFile( const std::string& name, gl::ShaderType type, const std::string& path );

    bool                Compile();
    void                FreeVRAM();
    GLuint              GetHandle() const;
    std::string         GetName() const;
    gl::ShaderType      GetShaderType() const;
    const std::string&  GetSourceCode() const;
    bool                IsCompiled() const;
    bool                IsStub() const;
    static void         ReloadCodeFromFile( ShaderPtr shader );

protected:
    Shader();
    Shader( const std::string& name, gl::ShaderType type, const std::string& code );
    Shader( const Shader& other ) = delete;
    Shader( Shader&& source ) = delete;
    Shader& operator=( const Shader& other ) = delete;
    Shader& operator=( Shader&& source ) = delete;
    bool operator==( const Shader& other ) = delete;

    bool                CompileParameterized( bool ignore_stub );
    static void         LoadCodeFromFile( ShaderPtr shader, const std::string& path );
    static std::string  GetShortDetailsAsString( Shader& shader );
    static std::string  GetDetailsAsString( Shader& shader );

private:
    static ShaderPtr    stStubVertexShader;
    static ShaderPtr    stStubGeometryShader;
    static ShaderPtr    stStubFragmentShader;

    bool                mFailedCompilation  = false;
    bool                mIsStub             = false;
    std::string          mName;
    gl::ShaderType       mType = gl::ShaderType::NO_SHADER_TYPE;
    std::string          mPath;
    GLuint               mHandle = 0;
    std::string         mSourceCode;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
