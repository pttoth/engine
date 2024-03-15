#pragma once

#include "engine/gl/Def.h"
#include "engine/gl/GlWrapper.h"
#include "pt/macros.h"
#include "pt/logging.h"
#include "pt/name.h"

#include <functional>

namespace engine{
namespace gl{

// TODO: move this to the end of the file

//poor man's 'Concept' :)
//  things in this namespace define, which parameter types are allowed for this helper function call
//  see:
//    ShaderProgram::SetUniform<T>( GLsizei count, const Uniform<T>& uniform )
namespace concept{

inline void  LoadUniformV( GLint location, GLsizei count, const GLfloat* values ){
    gl::Uniform1fv( location, count, values );
}
inline void  LoadUniformV( GLint location, GLsizei count, const math::float1* values ){
    gl::Uniform1fv( location, count, values->v );
}
inline void  LoadUniformV( GLint location, GLsizei count, const math::float2* values ){
    gl::Uniform2fv( location, count, values->v );
}
inline void  LoadUniformV( GLint location, GLsizei count, const math::float3* values ){
    gl::Uniform3fv( location, count, values->v );
}
inline void  LoadUniformV( GLint location, GLsizei count, const math::float4* values ){
    gl::Uniform4fv( location, count, values->v );
}
inline void  LoadUniformV( GLint location, GLsizei count, const GLint* values ){
    gl::Uniform1iv( location, count, values );
}
inline void  LoadUniformV( GLint location, GLsizei count, const math::int1* values ){
    gl::Uniform1iv( location, count, values->v );
}
inline void  LoadUniformV( GLint location, GLsizei count, const math::int2* values ){
    gl::Uniform2iv( location, count, values->v );
}
inline void  LoadUniformV( GLint location, GLsizei count, const math::int3* values ){
    gl::Uniform3iv( location, count, values->v );
}
inline void  LoadUniformV( GLint location, GLsizei count, const math::int4* values ){
    gl::Uniform4iv( location, count, values->v );
}
inline void  LoadUniformV( GLint location, GLsizei count, const GLuint* values ){
    gl::Uniform1uiv( location, count, values );
}
inline void  LoadUniformV( GLint location, GLsizei count, const math::float4x4* values ){
    gl::UniformMatrix4fv( location, count, gl::Transpose::DO_TRANSPOSE, &(values->m[0][0]) );
}

template<class ArrayDataType>
inline void  LoadUniformV( GLint location, GLsizei count, const std::vector<ArrayDataType>& values ){
    LoadUniformV( location, count, values.data() );
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
    Uniform()
    {}
    Uniform( const pt::Name& varName, const pt::Name& progName, GLint varHandle, GLint progHandle ):
        mNameVar( varName ), mNameProg( progName ), mHandleVar( varHandle ), mHandleProg( progHandle )
    {}
    virtual ~Uniform()
    {}
    Uniform( const Uniform& other ) = delete;
    Uniform& operator=( const Uniform& other ) = delete;

    Uniform( Uniform&& source ){
        mInitialized = source.mInitialized;
        mNameVar     = std::move( source.mNameVar );
        mNameProg    = std::move( source.mNameProg );
        mHandleVar   = source.mHandleVar;
        mHandleProg  = source.mHandleProg;
        mData        = std::move( source.mData );
    }

    Uniform& operator=( Uniform&& source ){
        mInitialized = source.mInitialized;
        mNameVar     = std::move( source.mNameVar );
        mNameProg    = std::move( source.mNameProg );
        mHandleVar   = source.mHandleVar;
        mHandleProg  = source.mHandleProg;
        mData        = std::move( source.mData );
        return *this;
    }

    Uniform& operator=( const T& val ){
        mInitialized = true;
        mData = val;
        return *this;
    }

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

    bool IsInitialized() const{
        return mInitialized;
    }

    bool IsValid() const{
        return ( 0 != mHandleProg ) && ( -1 < mHandleVar );
    }

    const T& Data() const{
        return mData;
    }

protected:
private:
    bool        mInitialized = false;
    pt::Name    mNameVar;
    pt::Name    mNameProg;
    GLint       mHandleVar = -2; // -2 : default
                                 // -1 : query result missing
                                 // 0+ : valid
    GLuint      mHandleProg = 0;
    T           mData;
};


} // end of namespace 'gl'
} // end of namespace 'engine'
