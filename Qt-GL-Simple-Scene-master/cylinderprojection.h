#ifndef CYLINDERPROJECTION_H
#define CYLINDERPROJECTION_H

#include <glm/glm.hpp>
#include <list>
#include <vector>

#include "topology.h"

/*
    Reference: two way texture mapping
    ábrák is jöhetnek mi egymás.

*/

class CylinderProjection
{

    std::list<glm::mat4> projectMatrices;
    std::vector<glm::vec3> cameraPositions;
    std::vector<glm::vec3> centerPositions;

    int get1DIndexFrom2DIndex(int i, int j);

    int index(int i, int j);

public:

    float height;
    float radius;


    glm::vec3 basePointPos;
    glm::vec3 topPointPos;
    glm::vec3 baseUpDirection; //normalized
    glm::vec3 baseRayDirection; //normalized


    int verticalGridSize;
    int horizontalGridSize;

    CylinderProjection();

    std::list<glm::mat4> getProjectionMatrices();

    void projectCylinderToModel(std::vector<glm::vec3> pointPositions, std::vector<glm::vec3> pointNormals, Topology& topology);

};

#endif // CYLINDERPROJECTION_H
