#ifndef EXTRUDEEDGE_H
#define EXTRUDEEDGE_H

#include <glm/glm.hpp>

struct ExtrudeEdge
{
    unsigned int edgeId;
    glm::vec2 extrudeDir;
    unsigned int pointId1;
    glm::vec2 cameraPos1;
    unsigned int pointId2;
    glm::vec2 cameraPos2;
};

#endif // EXTRUDEEDGE_H
