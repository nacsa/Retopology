#ifndef TOPOLOGYIMPORTER_H
#define TOPOLOGYIMPORTER_H

#include<vector>

class TopologyImporter
{
public:
    TopologyImporter();
    bool loadOBJ(const std::string &path,
        std::vector<unsigned int> &outIndices,
        std::vector<float> &outVertices,
        std::vector<float> &outNormals
        ) ;
};

#endif // TOPOLOGYIMPORTER_H
