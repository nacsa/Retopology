#ifndef VERTEXISLAND_H
#define VERTEXISLAND_H

#include <list>
#include "glm/glm.hpp"
#include "topology.h"

class VertexIsland
{
    Topology* topology;
    std::list<unsigned int> vertexIds;
    std::map<unsigned int, glm::vec3> startVertexPositionMap;
    std::map<unsigned int, glm::vec3> startVertexNormalMap;
    glm::vec3 center;
    void calculateCenter();
public:
    glm::mat4 modelMatrix;
    VertexIsland(){}
    VertexIsland(Topology* topology);
    void addVertexIds(std::list<unsigned int> vertexIds);
    void setVertexId(unsigned int vertexId);
    void clear();
    void resetVertexStartPositions();
    void resetVertexStartNormals();
    void updateModelMatrix();
    void updateIslandPosition();
    void updateIslandRotation();
    void updateIslandScaling();
};

#endif // VERTEXISLAND_H
