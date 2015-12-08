#include "constraintpoint.h"

unsigned int ConstraintPoint::current_id = 0;

ConstraintPoint::ConstraintPoint()
{
}

ConstraintPoint::ConstraintPoint(glm::vec3 pos)
{
    this->id = current_id++;
    this->pos = pos;
}
