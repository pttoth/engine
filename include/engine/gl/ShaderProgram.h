#pragma once

#include "engine/Def.h"
#include "engine/gl/Shader.h"
#include "engine/gl/Uniform.hpp"
#include "pt/macros.h"
#include <assert.h>
#include <vector>

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( ShaderProgram )

//TODO: class should cache the results of 'GetUniform()' calls and return the cached value (saves a string comparison per call)

// @TODO: shader program linking falls through for some reason, resulting in full white screen
//          compilation should catch the error and stop execution!
//  strong guess: Shader compilation stops startup, but failed shaderprogram linking does not

class ShaderProgram
{
public:
    //ShaderProgram();
    ShaderProgram( const pt::Name& name );
    virtual ~ShaderProgram();
    ShaderProgram( ShaderProgram&& source );
    ShaderProgram& operator=( ShaderProgram&& source );

    ShaderProgram( const ShaderProgram& other )             = delete;
    ShaderProgram& operator=( const ShaderProgram& other )  = delete;
    bool operator==( const ShaderProgram& other ) const     = delete;

    void        AddShader( ShaderPtr shader );
    void        AddUniformName( const pt::Name& name ); // Helps detect missing / optimized-away / mistyped variables
                                                        //  (Not very neat, might be removed later.)
    void        ClearShaders();
    void        FreeVRAM();
    GLuint      GetHandle() const;
    pt::Name    GetName() const;

    const std::vector<pt::Name>& GetUniformNames() const;

    // GetUniform() does not retrieve uniform data, just handles.
    //   With good design, reading uniform values from GPU should never be needed,
    //   especially for the performance impact of the GPU pipeline flush
    template<class T>
    Uniform<T> GetUniform( const char* name ){
        assert( mLinked );
        if( !mLinked ){
            PT_LOG_LIMITED_ERR( 20, "Tried to get uniform '" << name << "' from shader '" << mName << "' that is not linked!" );
            static const pt::Name emptyname("");
            return Uniform<T>( name, emptyname, 0, 0 );
        }
        GLint varHandle = gl::GetUniformLocation( mHandle, name );
#ifdef PT_DEBUG_ENABLED
        if( -1 == varHandle ){
            PT_LOG_LIMITED_WARN( 20, "Uniform '" << name << "' returned from shader program '" << GetName() << "' does not exist. Variable unused and optimized away?" );
        }
#endif
        return Uniform<T>( name, mName, varHandle, mHandle );
    }

    template<class T>
    Uniform<T> GetUniform( const std::string& name ){
        return GetUniform<T>( name.c_str() );
    }

    template<class T>
    Uniform<T> GetUniform( const pt::Name& name ){
        return GetUniform<T>( name.GetStdString().c_str() );
    }

    bool IsLinked() const;
    bool Link();

    template<class T>
    void SetUniform( const Uniform<T>& uniform ){
        SetUniformV<T>( 1, uniform );
    }

    template<class T>
    void SetUniform( Uniform<T>& uniform, const T& data ){
        uniform = data;
        SetUniform( uniform );
    }

    void Use();

protected:
    virtual void OnLinked();

    template<class T>
    void SetUniformV( GLsizei count, const Uniform<T>& uniform ){
        GLuint varProgHandle = uniform.GetProgramHandle();
        GLint  varHandle     = uniform.GetHandle();

        assert( uniform.IsInitialized() );
        assert( 0 != varProgHandle );
        if( !uniform.IsInitialized() || (0 == varProgHandle) ){
            PT_LOG_LIMITED_ERR( 20, "Tried to set uninitialized Uniform '" << uniform.GetName()
                << "' in program '" << mName << "'. Skipping." );
            return;
        }
#ifdef PT_DEBUG_ENABLED
        if( !uniform.IsValid() ){
            PT_LOG_LIMITED_WARN( 20, "Tried to set invalid Uniform '" << uniform.GetName()
                << "' in shader program '" << mName << "'. Skipping." );
            return;
        }
#endif
        assert( varProgHandle == mHandle );
        if( varProgHandle != mHandle ){
            PT_LOG_LIMITED_ERR( 20, "Tried to set Uniform '" << uniform.GetName()
                << "' in program '" << mName << "', it does not belong to. Skipping." );
            return;
        }
        if( !mLinked ){
            PT_LOG_LIMITED_ERR( 20, "Tried to set Uniform '" << uniform.GetName()
                << "' in program '" << mName << "', that is not linked. Skipping." );
            assert( mLinked );
            return;
        }

        //TODO: find a way to track active ShaderProgram on client-side without stalling GPU with queries
        //      then remove this overhead
        // ensure, that the program is in use
        this->Use();

        concept::LoadUniformV( varHandle, count, &(uniform.Data()) );
        //'varHandle' cannot be invalid, if the Uniform<T> was generated by this ShaderProgram object
        //An error case might be, if the shaderprogram handle gets reused after deletion
        //  and its old Uniform<T>-s are still hanging around, which get passed into the new program with the same handle
        //  in those rare cases, 'varHandle' might be invalid.
        //  In debug mode, GlWrapper will take care of the error handling after OpenGL generates and error.
        //  ...but if it maps correctly to a variable with different name in the new shader, the error will pass all checking.
        //Another similar case may be, if the shader gets relinked and its Uniforms are still hanging around.
        //TODO: ShaderPrograms in RAM should have a unique 'uint64_t' global ID, that never gets reused. They too should be passed to Uniform<T>-s.
        //  Get a new client-side shader ID on every Link().
    }

    std::vector<pt::Name>   mUniformNames;

private:
    void SetDefaultMemberValues(){
        mDirty   = false;
        mLinked  = false;
        mName    = pt::Name();
        mShaders = std::vector<ShaderPtr>();
        mHandle  = 0;
    }

    bool                    mDirty = false;  // Shader pipeline modified, but not linked yet
    bool                    mLinked = false;
    pt::Name                mName;
    std::vector<ShaderPtr>  mShaders;
    GLuint                  mHandle = 0;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
