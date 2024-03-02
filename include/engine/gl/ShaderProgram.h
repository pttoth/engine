#pragma once

#include "engine/Def.h"
#include "engine/gl/Shader.h"
#include "engine/gl/Uniforms.hpp"
#include "pt/macros.h"
#include <assert.h>
#include <vector>

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( ShaderProgram )

class ShaderProgram
{
public:
    //ShaderProgram();
    ShaderProgram( const pt::Name& name );
    virtual ~ShaderProgram();
    ShaderProgram( ShaderProgram&& source )            = default;
    ShaderProgram& operator=( ShaderProgram&& source ) = default;

    ShaderProgram( const ShaderProgram& other )             = delete;
    ShaderProgram& operator=( const ShaderProgram& other )  = delete;
    bool operator==( const ShaderProgram& other ) const     = delete;

    void        AddShader( ShaderPtr shader );
    void        ClearShaders();
    void        FreeVRAM();
    GLuint      GetHandle() const;
    pt::Name    GetName() const;

    // GetUniform() does not retrieve uniform data, just handles:
    //   with good design, reading uniform values from GPU should never be needed
    //   especially for its severe performance impact of the CPU-GPU sync
    template<class T>
    Uniform<T> GetUniform( const pt::Name& name ){
        assert( mLinked );
        if( !mLinked ){
            PT_LOG_ERR( "Tried to get uniform '" << name.GetStdString()
                        << "' from shader '" << mName.GetStdString()
                        << "' that is not linked!" );
            static const pt::Name emptyname("");
            return Uniform<T>( name, emptyname, 0, 0 );
        }
        GLint varHandle = gl::GetUniformLocation( mHandle, name.GetStdString().c_str() );

        return Uniform<T>( name, mName, varHandle, mHandle );
    }

    bool IsLinked() const;
    bool Link();

    template<class T>
    void SetUniform( const Uniform<T>& uniform ){
        SetUniformV<T>( 1, uniform );
    }

    template<class T>
    void SetUniformV( GLsizei count, const Uniform<T>& uniform ){
        GLuint varProgHandle = uniform.GetProgramHandle();
        GLint  varHandle     = uniform.GetHandle();

        assert( uniform.IsInitialized() );
        if( !uniform.IsInitialized() ){
            PT_LOG_ERR( "Tried to set uninitialized Uniform '" << uniform.GetName().GetStdString()
                << "' in program '" << mName.GetStdString() << "'. Skipping." );
            return;
        }

        assert( 0 != varHandle );
        assert( 0 != varProgHandle );
        if( (0 == varHandle) || (0 == varProgHandle) ){
            PT_LOG_ERR( "Tried to set Uniform '" << uniform.GetName().GetStdString()
                << "' in program '" << mName.GetStdString()
                << "', while having 0-s as handles. Skipping." );
            return;
        }

        assert( varProgHandle == mHandle );
        if( varProgHandle != mHandle ){
            PT_LOG_ERR( "Tried to set Uniform '" << uniform.GetName().GetStdString()
                << "' in program '" << mName.GetStdString()
                << "', it does not belong to. Skipping." );
            return;
        }

        assert( mLinked );
        if( !mLinked ){
            PT_LOG_ERR( "Tried to set Uniform '" << uniform.GetName().GetStdString()
                << "' in program '" << mName.GetStdString()
                << "', that is not linked. Skipping." );
            return;
        }

        // ensure, that the program is in use
        this->Use();

        concept::LoadUniformV( varHandle, count, &(uniform.Data()) );
        //'varHandle' cannot be invalid, if the Uniform<T> was generated by this ShaderProgram object
        //one error case might be, if the program handle gets reused after deletion
        //  and its old Uniforms are still hanging around, which get passed into the new program with the same handle
        //  in those rare cases, 'varHandle' might be invalid
        //  in debug mode, it'll assert-fail after generating an error
    }

    void Use();

protected:
private:
    bool                    mLinked = false;
    pt::Name                mName;
    std::vector<ShaderPtr>  mShaders;
    GLuint                  mHandle = 0;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
