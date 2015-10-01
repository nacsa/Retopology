#ifndef MODELIMPORTER_H
#define MODELIMPORTER_H

//#include <Assimp/include/assimp/scene.h>
#include <vector>
#include <string>

class ModelImporter
{
public:
    ModelImporter();
//    bool Import3DFromFile( const std::string& pFile);
    bool loadOBJ(const std::string &path,
        std::vector<unsigned int> &outIndices,
        std::vector<float> &outVertices,
        std::vector<float> &outNormals
        ) ;
private:
//   aiScene* scene;
   /* void get_bounding_box (aiVector3D* min, aiVector3D* max);
    void get_bounding_box_for_node (const aiNode* nd, aiVector3D* min, aiVector3D* max);*/
};

#endif // MODELIMPORTER_H
