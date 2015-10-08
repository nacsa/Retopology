#include "topologyhelpertriangle.h"

unsigned int TopologyHelperTriangle::current_id = 0;

TopologyHelperTriangle::TopologyHelperTriangle(unsigned int id, unsigned int i1, unsigned int i2, unsigned int i3, unsigned int ei1, unsigned int ei2, unsigned int ei3)
{
    this->id = id;
    this->pointId1 = i1;
    this->pointId2 = i2;
    this->pointId3 = i3;
    this->edgeId1 = ei1;
    this->edgeId2 = ei2;
    this->edgeId3 = ei3;
}

TopologyHelperTriangle::TopologyHelperTriangle(unsigned int i1, unsigned int i2, unsigned int i3): id(++current_id)
{
    this->pointId1 = i1;
    this->pointId2 = i2;
    this->pointId3 = i3;
    this->edgeId1 = 0;
    this->edgeId2 = 0;
    this->edgeId3 = 0;
}

TopologyHelperTriangle::TopologyHelperTriangle()
{

}

unsigned int TopologyHelperTriangle::getId()
{
    return id;
}
