#ifndef CONSTRAINTPOINT_H
#define CONSTRAINTPOINT_H

#include <glm/glm.hpp>

class ConstraintPoint
{
public:
    static unsigned int current_id;
    unsigned int id;
    glm::vec3 pos;
    ConstraintPoint();
    ConstraintPoint(glm::vec3 pos);
};

#endif // CONSTRAINTPOINT_H
