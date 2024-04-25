#include "engine/gl/AssimpConfig.h"

#include "pt/logging.h"
#include "pt/utility.hpp"

#include <assimp/postprocess.h>

#define InitializePostProcessAttribute( key ) \
    CfgAddKey( (*this), key ); \
    mPostProcessSteps[key] = false;



using namespace engine;

AssimpConfig::
AssimpConfig()
{
    InitializeFields();
}


void AssimpConfig::
InitializeFields()
{
    InitializePostProcessAttribute( aiProcess_CalcTangentSpace );
    InitializePostProcessAttribute( aiProcess_JoinIdenticalVertices );
    InitializePostProcessAttribute( aiProcess_MakeLeftHanded );
    InitializePostProcessAttribute( aiProcess_Triangulate );
    InitializePostProcessAttribute( aiProcess_RemoveComponent );
    InitializePostProcessAttribute( aiProcess_GenNormals );
    InitializePostProcessAttribute( aiProcess_GenSmoothNormals );
    InitializePostProcessAttribute( aiProcess_SplitLargeMeshes );
    InitializePostProcessAttribute( aiProcess_PreTransformVertices );
    InitializePostProcessAttribute( aiProcess_LimitBoneWeights );
    InitializePostProcessAttribute( aiProcess_ValidateDataStructure );
    InitializePostProcessAttribute( aiProcess_ImproveCacheLocality );
    InitializePostProcessAttribute( aiProcess_RemoveRedundantMaterials );
    InitializePostProcessAttribute( aiProcess_FixInfacingNormals );
    InitializePostProcessAttribute( aiProcess_PopulateArmatureData );
    InitializePostProcessAttribute( aiProcess_SortByPType );
    InitializePostProcessAttribute( aiProcess_FindDegenerates );
    InitializePostProcessAttribute( aiProcess_FindInvalidData );
    InitializePostProcessAttribute( aiProcess_GenUVCoords );
    InitializePostProcessAttribute( aiProcess_TransformUVCoords );
    InitializePostProcessAttribute( aiProcess_FindInstances );
    InitializePostProcessAttribute( aiProcess_OptimizeMeshes );
    InitializePostProcessAttribute( aiProcess_OptimizeGraph  );
    InitializePostProcessAttribute( aiProcess_FlipUVs );
    InitializePostProcessAttribute( aiProcess_FlipWindingOrder  );
    InitializePostProcessAttribute( aiProcess_SplitByBoneCount  );
    InitializePostProcessAttribute( aiProcess_Debone );
    InitializePostProcessAttribute( aiProcess_GlobalScale );
    InitializePostProcessAttribute( aiProcess_EmbedTextures  );
    InitializePostProcessAttribute( aiProcess_ForceGenNormals );
    InitializePostProcessAttribute( aiProcess_DropNormals );
    InitializePostProcessAttribute( aiProcess_GenBoundingBoxes );


    //initialize all config vars as 'false'
    for( auto& e : GetEntriesRef() ){
        e.val_str = "false";
    }
}


void AssimpConfig::
ReadFile()
{
    Config::read();
}


void AssimpConfig::
ReadFile( const char* path )
{
    Config::readF( path );
}


void AssimpConfig::
ReadFile( const std::string& path )
{
    Config::readF( path );
}


void AssimpConfig::
SetDefaults()
{
    this->clear();
    this->InitializeFields();
    setB( aiProcess_JoinIdenticalVertices, true );
    setB( aiProcess_Triangulate, true );
    setB( aiProcess_RemoveComponent, true );
    setB( aiProcess_ValidateDataStructure, true );
    setB( aiProcess_SortByPType, true );
    setB( aiProcess_TransformUVCoords, true );
}


unsigned int AssimpConfig::
GetParameterMask() const
{
    unsigned int retval = 0;
    for( auto& e : GetEntriesRef() ){
        bool val = ( "true" == pt::StringToLower( e.val_str ) );
        retval += e.key_id * val;
    }
    return retval;
}


bool AssimpConfig::
GetAttrib( aiPostProcessSteps key ) const
{
    if( 0 < mPostProcessSteps.count( key ) ){
        return mPostProcessSteps.at( key );
    }
    PT_LOG_ERR( "Invalid argument'" << key << "' passed as key to AssimpConfig::GetAttrib()" );
    #ifdef PT_DEBUG_ENABLED
        pt::PrintStackTrace();
    #endif
    return false;
}


AssimpConfig::
~AssimpConfig()
{}

