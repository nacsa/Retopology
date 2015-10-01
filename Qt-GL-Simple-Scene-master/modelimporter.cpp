#include "modelimporter.h"
#include <fstream>

#include <Assimp/include/assimp/ai_assert.h>
#include <Assimp/include/assimp/scene.h>
#include <Assimp/include/assimp/Importer.hpp>
#include <Assimp/include/assimp/postprocess.h>
#include <Assimp/include/assimp/IOSystem.hpp>


//#define aisgl_min(x,y) (x<y?x:y)
//#define aisgl_max(x,y) (y>x?y:x)



ModelImporter::ModelImporter()
{

}
/*
bool ModelImporter::Import3DFromFile( const std::string& pFile)
{
    Assimp::Importer importer;
    // the global Assimp scene object

    //check if file exists
    std::ifstream fin(pFile.c_str());
    if(!fin.fail()) {
        fin.close();
    }
    else{
        printf("Couldn't open file: %s\n", pFile.c_str());
        printf("%s\n", importer.GetErrorString());
        return false;
    }

    scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);

    // If the import failed, report it
    if( !scene)
    {
        printf("%s\n", importer.GetErrorString());
        return false;
    }

    // Now we can access the file's contents.
    printf("Import of scene %s succeeded.",pFile.c_str());

    aiVector3D scene_min, scene_max, scene_center;
    get_bounding_box(&scene_min, &scene_max);
    float tmp;
    tmp = scene_max.x-scene_min.x;
    tmp = scene_max.y - scene_min.y > tmp?scene_max.y - scene_min.y:tmp;
    tmp = scene_max.z - scene_min.z > tmp?scene_max.z - scene_min.z:tmp;
    //scaleFactor = 1.f / tmp;

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}
/*
void ModelImporter::get_bounding_box (aiVector3D* min, aiVector3D* max)
{

    min->x = min->y = min->z =  1e10f;
    max->x = max->y = max->z = -1e10f;
    get_bounding_box_for_node(scene->mRootNode,min,max);
}

void ModelImporter::get_bounding_box_for_node (const aiNode* nd,
    aiVector3D* min,
    aiVector3D* max)

{
    unsigned int n = 0, t;

    for (; n < nd->mNumMeshes; ++n) {
        const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
        for (t = 0; t < mesh->mNumVertices; ++t) {

            aiVector3D tmp = mesh->mVertices[t];

            min->x = aisgl_min(min->x,tmp.x);
            min->y = aisgl_min(min->y,tmp.y);
            min->z = aisgl_min(min->z,tmp.z);

            max->x = aisgl_max(max->x,tmp.x);
            max->y = aisgl_max(max->y,tmp.y);
            max->z = aisgl_max(max->z,tmp.z);
        }
    }

    for (n = 0; n < nd->mNumChildren; ++n) {
        get_bounding_box_for_node(nd->mChildren[n],min,max);
    }
}

*/


bool ModelImporter::loadOBJ
    (
    const std::string& path,
    std::vector<unsigned int> &outIndices,
    std::vector<float> &outVertices,
    std::vector<float> &outNormals
    )
{
    Assimp::Importer importer;

    //check if file exists
    std::ifstream fin(path.c_str());
    if(!fin.fail()) {
        fin.close();
    }
    else{
        printf("Couldn't open file: %s\n", path.c_str());
        printf("%s\n", importer.GetErrorString());
        return false;
    }

    importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality);//aiProcessPreset_TargetRealtime_Quality);
    const aiScene* scenee =  importer.ApplyPostProcessing(aiProcess_Triangulate);
    aiMesh* mesh = scenee->mMeshes[0];


    int numOfFaces = mesh->mNumFaces;
    int numOfIndices = numOfFaces * 3;
    outIndices.resize(numOfIndices);

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace &face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        outIndices[i * 3 + 0] = face.mIndices[0];
        outIndices[i * 3 + 1] = face.mIndices[1];
        outIndices[i * 3 + 2] = face.mIndices[2];
    }

    int numOfVertices = mesh->mNumVertices;
    outVertices.resize(numOfVertices * 3);
    outNormals.resize(numOfVertices * 3);
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        if (mesh->HasPositions()) {
            outVertices[i * 3 + 0] = mesh->mVertices[i].x;
            outVertices[i * 3 + 1] = mesh->mVertices[i].y;
            outVertices[i * 3 + 2] = mesh->mVertices[i].z;
        }

        if (mesh->HasNormals()) {
            outNormals[i * 3 + 0] = mesh->mNormals[i].x;
            outNormals[i * 3 + 1] = mesh->mNormals[i].y;
            outNormals[i * 3 + 2] = mesh->mNormals[i].z;
        }


    }
    return true;
}
