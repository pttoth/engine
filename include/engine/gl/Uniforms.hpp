#pragma once

#include "engine/gl/Def.h"
#include "engine/gl/GlWrapper.h"
#include "pt/macros.h"
#include "pt/logging.h"
#include "pt/name.h"

#include <functional>

namespace engine{
namespace gl{
namespace experimental{
void LoadUniform( GLint location, GLfloat v ){
    gl::Uniform1f( location, v );
}
void LoadUniform( GLint location, math::float1 v ){
    gl::Uniform1f( location, v.x );
}
void LoadUniform( GLint location, math::float2 v ){
    gl::Uniform2f( location, v.x, v.y );
}
void LoadUniform( GLint location, math::float3 v ){
    gl::Uniform3f( location, v.x, v.y, v.z );
}
void LoadUniform( GLint location, math::float4 v ){
    gl::Uniform4f( location, v.x, v.y, v.z, v.w );
}
void LoadUniform( GLint location, GLint v ){
    gl::Uniform1i( location, v );
}
void LoadUniform( GLint location, math::int1 v ){
    gl::Uniform1i( location, v.x );
}
void LoadUniform( GLint location, math::int2 v ){
    gl::Uniform2i( location, v.x, v.y );
}
void LoadUniform( GLint location, math::int3 v ){
    gl::Uniform3i( location, v.x, v.y, v.z );
}
void LoadUniform( GLint location, math::int4 v ){
    gl::Uniform4i( location, v.x, v.y, v.z, v.w );
}
void LoadUniform( GLint location, GLuint v ){
    gl::Uniform1ui( location, v );
}
void LoadUniformV( GLint location, GLsizei count, const math::float1* value ){
    gl::Uniform1fv( location, count, value->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::float2* value ){
    gl::Uniform2fv( location, count, value->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::float3* value ){
    gl::Uniform3fv( location, count, value->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::float4* value ){
    gl::Uniform4fv( location, count, value->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::int1* value ){
    gl::Uniform1iv( location, count, value->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::int2* value ){
    gl::Uniform2iv( location, count, value->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::int3* value ){
    gl::Uniform3iv( location, count, value->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::int4* value ){
    gl::Uniform4iv( location, count, value->v );
}
void LoadUniformV1( GLint location, GLsizei count, const GLuint* value ){
    gl::Uniform1uiv( location, count, value );
}
void LoadUniformV2( GLint location, GLsizei count, const GLuint* value ){
    gl::Uniform2uiv( location, count, value );
}
void LoadUniformV3( GLint location, GLsizei count, const GLuint* value ){
    gl::Uniform3uiv( location, count, value );
}
void LoadUniformV4( GLint location, GLsizei count, const GLuint* value ){
    gl::Uniform4uiv( location, count, value );
}

void LoadUniformMatrix4x4( GLint location, GLboolean transpose, const math::float4x4 value ){
    UniformMatrix4fv( location, 1, transpose, &(value.m[0][0]) );
}


void LoadUniformMatrix2x2V( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value ){
    UniformMatrix2fv( location, count, transpose, value );
}
void LoadUniformMatrix3x3V( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value ){
    UniformMatrix3fv( location, count, transpose, value );
}
void LoadUniformMatrix4x4V( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value ){
    UniformMatrix4fv( location, count, transpose, value );
}
void LoadUniformMatrix4x4V( GLint location, GLsizei count, GLboolean transpose, const math::float4x4* value ){
    UniformMatrix4fv( location, count, transpose, &(value->m[0][0]) );
}

template<class T>
class Uni
{
public:
    Uni( const pt::Name& name ):
        mName( name )
    {}
    Uni( const Uni& other ) = default;
    Uni( Uni&& source );
    virtual ~Uni();
    Uni& operator=( const Uni& other );
    Uni& operator=( Uni&& source );

    bool operator==( const Uni& other ) const;

    void operator=( const T& val ){
        mData = val;
        LoadToVRAM();
    }

    void LoadToVRAM()
    {
        if( 0 == mHandle ){
            PT_LOG_ERR( "Tried to load uniform '" << mName.GetStdString() << "' with handle (0) to VRAM" );
            return;
        }
        LoadUniform( mHandle, mData );
    }
    T ReadFromVRAM()
    {
        PT_UNIMPLEMENTED_FUNCTION
    }

protected:
private:
    pt::Name mName;
    GLint mHandle = 0;
    T mData;
};

void DeleteMe()
{
    Uni<float>  Fasd( "float" );
    Uni<int>    Iasd( "int" );
    Uni<math::float4x4> Masd( "matrix" );
    //Uni<math::float2[2]> asdasd( "asdasd" );
    //asdasd.LoadToVRAM();
    //Uni<pt::Name> bork( "bork" );

    Fasd.LoadToVRAM();
    Iasd.LoadToVRAM();
    //bork.LoadToVRAM();
}

} // end of namespace 'experimental'





//poor man's 'Concept' :)
//  things in this namespace define, which parameter types are allowed for this helper function call
//  see:
//    ShaderProgram::SetUniform<T>( GLsizei count, const Uniform<T>& uniform )
namespace concept{

void LoadUniformV( GLint location, GLsizei count, const GLfloat* values ){
    gl::Uniform1fv( location, count, values );
}
void LoadUniformV( GLint location, GLsizei count, const math::float1* values ){
    gl::Uniform1fv( location, count, values->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::float2* values ){
    gl::Uniform2fv( location, count, values->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::float3* values ){
    gl::Uniform3fv( location, count, values->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::float4* values ){
    gl::Uniform4fv( location, count, values->v );
}
void LoadUniformV( GLint location, GLsizei count, const GLint* values ){
    gl::Uniform1iv( location, count, values );
}
void LoadUniformV( GLint location, GLsizei count, const math::int1* values ){
    gl::Uniform1iv( location, count, values->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::int2* values ){
    gl::Uniform2iv( location, count, values->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::int3* values ){
    gl::Uniform3iv( location, count, values->v );
}
void LoadUniformV( GLint location, GLsizei count, const math::int4* values ){
    gl::Uniform4iv( location, count, values->v );
}
void LoadUniformV( GLint location, GLsizei count, const GLuint* values ){
    gl::Uniform1uiv( location, count, values );
}
void LoadUniformV( GLint location, GLsizei count, const math::float4x4* values ){
    gl::UniformMatrix4fv( location, count, gl::Transpose::DO_TRANSPOSE, &(values->m[0][0]) );
}

} // end of namespace 'concept'


template<class T> class Uniform;
template<class T> using UniformPtr       = std::shared_ptr< Uniform<T> >;
template<class T> using ConstUniformPtr  = std::shared_ptr< const Uniform<T> >;
template<class T> using UniformWPtr      = std::weak_ptr< Uniform<T> >;
template<class T> using ConstUniformWPtr = std::weak_ptr< const Uniform<T> >;
template<class T> using UniformUPtr      = std::unique_ptr< Uniform<T> >;
template<class T> using ConstUniformUPtr = std::unique_ptr< const Uniform<T> >;

template<class T>
class Uniform {
public:
    Uniform( const pt::Name& varName, const pt::Name& progName, GLint varHandle, GLint progHandle ):
        mNameVar( varName ), mNameProg( progName ), mHandleVar( varHandle ), mHandleProg( progHandle )
    {}
    virtual ~Uniform()
    {}
    Uniform( const Uniform& other ) = default;
    Uniform( Uniform&& source ) = default;
    Uniform& operator=( const Uniform& other ) = default;
    Uniform& operator=( Uniform&& source ) = default;

    bool operator==( const Uniform& other ) const = delete;

    GLint GetHandle() const{
        return mHandleVar;
    }

    pt::Name GetName() const{
        return mNameVar;
    }

    GLuint GetProgramHandle() const{
        return mHandleProg;
    }

    pt::Name GetProgramName() const{
        return mNameProg;
    }

    bool IsValid() const{
        return ( 0 != mHandleProg ) && ( 0 != mHandleVar );
    }

    void Data( const T& val ) const{
        mData = val;
    }

    T& Data() const{
        return mData;
    }

protected:
private:
    pt::Name    mNameVar;
    pt::Name    mNameProg;
    GLint       mHandleVar = 0;
    GLuint      mHandleProg = 0;
    T           mData;
};


} // end of namespace 'gl'
} // end of namespace 'engine'
