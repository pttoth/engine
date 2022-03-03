#include "engine/mesh.h"

using namespace pt;
using namespace pt::engine;

Mesh::Mesh():Asset()
{

}

Mesh::
Mesh(const MeshDescriptor &descriptor)
        :Asset (descriptor){

}

Mesh::~Mesh()
{

}
