#include "newmesh.h"

#include <assert.h>

using namespace pt;
using namespace engine;

GLuint NewMesh::ShaderProgram = static_cast<unsigned int>(-1);
std::map<std::string, Texture*>  pt::engine::texture_db = std::map<std::string, Texture*>();

NewMesh::
NewMesh():
        mInitialized(false),
        mVBO(0), mIBO(0){
}

NewMesh::
NewMesh(const aiMesh *aimesh){
    /*
        std::string             mMaterialName;
        std::vector<gl::Vertex> mVertices;
        std::vector<Face>       mFaces;
    */

    mVertices.clear();
    mVertices.reserve(aimesh->mNumVertices);

    mFaces.clear();
    mFaces.reserve(aimesh->mNumFaces);

    //copy vertices from aiMesh
    for(size_t i=0; i<aimesh->mNumVertices; ++i){
        aiVector3D aivec = aimesh->mVertices[i];
        gl::Vertex vert(math::float3(aivec.x, aivec.y,aivec.z));
        vert.tex.x = aimesh->mTextureCoords[0][i].x;  //first tex channel's x coordinate
        vert.tex.y = aimesh->mTextureCoords[0][i].y;  //first tex channel's y coordinate
        mVertices.push_back(vert);
    }
    //copy faces from aiMesh
    for(size_t i=0; i<aimesh->mNumFaces; ++i){
        //----------
        //todo: remove this
        unsigned int size = aimesh->mFaces[i].mNumIndices;
        if(size != 3){
            assert(false);  //can break into this line on error
        }
        //----------

        assert(3 == aimesh->mFaces[i].mNumIndices);
        Face f;
        f.vertices.reserve(3);
        f.vertices.push_back(aimesh->mFaces[i].mIndices[0]);
        f.vertices.push_back(aimesh->mFaces[i].mIndices[1]);
        f.vertices.push_back(aimesh->mFaces[i].mIndices[2]);
        mFaces.push_back(f);
    }
}

NewMesh::
~NewMesh(){

}

const std::string& NewMesh::
getName() const{
    return mName;
}

const std::string &NewMesh::
getMaterialName() const{
    return mMaterialName;
}

std::vector<gl::Vertex> NewMesh::
getVertices() const{
    return mVertices;
}

std::vector<Face> NewMesh::
getFaces() const{
    return mFaces;
}

GLuint NewMesh::
getVBO() const{
    return mVBO;
}

GLuint NewMesh::
getIBO() const{
    return mIBO;
}


void NewMesh::
createRenderContext(){
    if(mInitialized){
        destroyRenderContext();
    }
    gl::GenBuffers(1, &mVBO);
    gl::GenBuffers(1, &mIBO);

    size_t VertexBufferSize = mVertices.size()*sizeof(gl::Vertex);
    //VertexBufferSize = 3;

    gl::BindBuffer(GL_ARRAY_BUFFER, mVBO);
    gl::BufferData(GL_ARRAY_BUFFER,
                   static_cast<int>(VertexBufferSize),
                   mVertices.data(),
                   GL_STATIC_DRAW);


    //create Facebuffer
    struct FaceBufferObject{
        uint32_t a,b,c;
    };
    std::vector<FaceBufferObject> buffer_faces;
    size_t buffer_faces_size = mFaces.size();
    buffer_faces.reserve( buffer_faces_size );
    for(size_t i=0; i<buffer_faces_size; ++i){
        FaceBufferObject fbo;
        fbo.a = mFaces[i].vertices[0];
        fbo.b = mFaces[i].vertices[1];
        fbo.c = mFaces[i].vertices[2];
        buffer_faces.push_back(fbo);
    }

    size_t FaceBufferSize = buffer_faces_size*sizeof(FaceBufferObject);

    gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    gl::BufferData(GL_ELEMENT_ARRAY_BUFFER,
                   static_cast<int>(FaceBufferSize),
                   buffer_faces.data(),
                   GL_STATIC_DRAW);

    mInitialized = true;
}

void NewMesh::
destroyRenderContext(){
    gl::DeleteBuffers(1, &mVBO);
    gl::DeleteBuffers(1, &mIBO);
    mInitialized = false;
}

void NewMesh::
setMaterialName(const std::string &name){
    this->mMaterialName = name;
}

void NewMesh::
bind(){
    gl::BindBuffer(GL_ARRAY_BUFFER, mVBO);
    gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
}


