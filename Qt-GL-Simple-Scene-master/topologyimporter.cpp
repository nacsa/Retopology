#include "topologyimporter.h"
#include <fstream>

#include <Assimp/include/assimp/ai_assert.h>
#include <Assimp/include/assimp/scene.h>
#include <Assimp/include/assimp/Importer.hpp>
#include <Assimp/include/assimp/postprocess.h>
#include <Assimp/include/assimp/IOSystem.hpp>



TopologyImporter::TopologyImporter()
{
}

bool TopologyImporter::loadOBJ(const std::string &path, std::vector<unsigned int> &outIndices, std::vector<float> &outVertices, std::vector<float> &outNormals)
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
