#ifndef TOPOLOGYHELPERTRIANGLE_H
#define TOPOLOGYHELPERTRIANGLE_H

#include <list>

class TopologyHelperTriangle
{
private:
    unsigned int id;

public:
    static unsigned int current_id;

    unsigned int pointId1;
    unsigned int pointId2;
    unsigned int pointId3;

    unsigned int edgeId1;
    unsigned int edgeId2;
    unsigned int edgeId3;

    TopologyHelperTriangle(unsigned int id,unsigned int i1,unsigned int i2,unsigned int i3,unsigned int ei1,unsigned int ei2,unsigned int ei3);
    TopologyHelperTriangle(unsigned int i1,unsigned int i2,unsigned int i3);
    TopologyHelperTriangle();
    unsigned int getId();
};

#endif // TOPOLOGYHELPERTRIANGLE_H
