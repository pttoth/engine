#pragma once

#include "pt/math.h"

#include "engine/texture.h"
#include "engine/glhelpers.h"

#include "assimp/mesh.h"

#include <map>

namespace pt{
namespace engine{

struct Vertex{
    math::float3            pos;
    math::int2              texpos;
    math::float3            normal;
};

struct Face{
    std::vector<uint32_t>   vertices;
    math::float3            normal;     //needed?
};

struct Material{
    std::string             name;
    std::string             texture_name;

    Texture*                texture;
};

class NewMesh
{
public:
    std::string             mName;
    std::string             mMaterialName;
    std::vector<gl::Vertex> mVertices;

    std::vector<Face>       mFaces;
    bool                    mInitialized;
    GLuint                  mVBO, mIBO;


    static GLuint   ShaderProgram;  //will be acquired through a ServiceLocator

    struct Dictionary{
        struct elem{
            std::string key;
            std::string val;
        };

        std::vector<elem> entries;

        std::string translate(const std::string& key) const{
            for(size_t i=0; i<entries.size(); ++i){
                if(entries[i].key == key){
                    return entries[i].val;
                }
            }
            return std::string();
        }
    };


    NewMesh();
    NewMesh(const aiMesh* aimesh);

    virtual ~NewMesh();

    //copy ctor + op
    //move ctor + op
    //comparator op


    const std::string&      getName()           const;
    const std::string&      getMaterialName()   const;
    std::vector<gl::Vertex> getVertices()       const;
    std::vector<Face>       getFaces()          const;
    GLuint                  getVBO()            const;
    GLuint                  getIBO()            const;


    void                    createRenderContext();
    void                    destroyRenderContext();
    void                    setMaterialName(const std::string& name);
    void                    bind();

};


extern std::map<std::string, Texture*>  texture_db;

inline void
RegisterTexture(const std::string& texname, Texture* tex){
    texture_db.emplace(texname, tex);
}

inline pt::engine::Texture*
fetchTexture(const std::string& name){
    Texture* retval;
    try{
        retval = texture_db.at(name);
    }catch(const std::out_of_range&){
        std::cout << "TEXTURE ERROR: could not access Texture '" << name << "'\n";
        retval = nullptr;
    }
    return retval;
}

inline pt::engine::Texture*
fetchMaterial(const std::string& name){
    return nullptr;
}

inline void drawMesh(NewMesh* mesh){
    /*
    Material*   mat         = fetchMaterial(mesh->getMaterialName());
    Texture*    texture     = fetchTexture(mat->texture_name);
    */
    Texture*    texture     = fetchTexture( mesh->getMaterialName() ); //todo: delete this

    //set model transform
    /*
    math::float4x4 M = math::float4x4::identity;
    M.m[3][0] = 0.5f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = 0.0f;

    GLint uniM = gl::GetUniformLocation(mesh->ShaderProgram, "M");
    gl::UniformFloat4x4(uniM, gl::DO_TRANSPOSE, M);
    */

    gl::EnableVertexAttribArray(0);
    gl::EnableVertexAttribArray(1);

    texture->bind(GL_TEXTURE0);
    mesh->bind();

    gl::VertexAttribPointer(0, 3,
                            GL_FLOAT, gl::SKIP_NORMALIZE,
                            sizeof(gl::Vertex), VERTEX_OFFSET_POSITION);

    gl::VertexAttribPointer(1, 2,
                            GL_FLOAT, gl::SKIP_NORMALIZE,
                            sizeof(gl::Vertex), VERTEX_OFFSET_TEXTURE);

    //gl::DrawArrays(GL_TRIANGLES, 0, 2*3);

    gl::DrawElements(GL_TRIANGLES,
                     static_cast<int>(mesh->getFaces().size()*3),
                     GL_UNSIGNED_INT,
                     nullptr);

}


}

} //end of namespace PT

