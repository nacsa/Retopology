#include "topologyhelperquad.h"
unsigned int TopologyHelperQuad::current_id = 0;

TopologyHelperQuad::TopologyHelperQuad(unsigned int id, unsigned i1, unsigned int i2, unsigned int i3, unsigned int i4, unsigned ei1, unsigned int ei2, unsigned int ei3, unsigned int ei4)
{
    this->id = id;
    this->pointId1 = i1;
    this->pointId2 = i2;
    this->pointId3 = i3;
    this->pointId4 = i4;
    this->edgeId1 = ei1;
    this->edgeId2 = ei2;
    this->edgeId3 = ei3;
    this->edgeId4 = ei4;
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
