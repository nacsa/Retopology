#include "topologyhelperquad.h"
unsigned int TopologyHelperQuad::current_id = 0;

TopologyHelperQuad::TopologyHelperQuad(unsigned int id, unsigned i1, unsigned int i2, unsigned int i3, unsigned int i4)
{
    this->id = id;
    this->pointId1 = i1;
    this->pointId2 = i2;
    this->pointId3 = i3;
    this->pointId4 = i4;
}

TopologyHelperQuad::TopologyHelperQuad(unsigned int i1, unsigned int i2, unsigned int i3, unsigned int i4): id(++current_id)
{
    this->pointId1 = i1;
    this->pointId2 = i2;
    this->pointId3 = i3;
    this->pointId4 = i4;
}

unsigned int TopologyHelperQuad::getId()
{
    return id;
}
