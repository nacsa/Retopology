#ifndef SIMILARMODELPROJECTION_H
#define SIMILARMODELPROJECTION_H

#include "topologypoint.h"
#include "topology.h"

#include <glm/glm.hpp>
#include <list>
#include <vector>

class SimilarModelProjection
{ 
    float projectionDistance;
    float nearPlane;
    float farPlane;
    glm::mat4 projectionMatrix;
public:
    SimilarModelProjection();
    void projectToModel(std::vector<glm::vec3> pointPositions, std::vector<glm::vec3> pointNormals, Topology& topology);
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix(TopologyPoint point);
};

#endif // SIMILARMODELPROJECTION_H
