#ifndef CONSTRAINTPROJECTION_H
#define CONSTRAINTPROJECTION_H

#include <map>
#include "constraintpoint.h"
#include "shader.hpp"
#include "topology.h"
class ConstraintProjection
{
    std::map<unsigned int, ConstraintPoint> pointConstraintPointMap;
    std::vector<unsigned int> pointIds;
    Topology* topology;
    unsigned int activePointId;

    bool enabled;

    Shader* shader;
    GLuint vaoHandle;
    GLuint vertexBuffer;

    void bindBufferData();

public:
    ConstraintProjection();

    void init(Topology* topology, GLuint vaoHandle);
    void allowedDraw(glm::mat4 projection, glm::mat4 modelview);
    void draw(glm::mat4 projection, glm::mat4 modelview);

    void startConstraintProj(std::list<unsigned int> pointIds);
    void setActivePoint(unsigned int id);
    void deleteConstraintPoint();
    void addConstraintPoint(glm::vec3 pos);
    void apply();
    void revert();
    void finish();
    bool isEnabled();
    void setEnabled(bool enabled);
    unsigned int getActivePointId();
};

#endif // CONSTRAINTPROJECTION_H
