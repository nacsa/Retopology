#include "topologyedge.h"


unsigned int TopologyEdge::current_id = 0;


TopologyEdge::TopologyEdge(unsigned int id, int i1, int i2)
{
    this->id = id;
    pointId1 = i1;
    pointId2 = i2;
}

TopologyEdge::TopologyEdge(int i1, int i2): id(++current_id)
{
    pointId1 = i1;
    pointId2 = i2;
}

unsigned int TopologyEdge::getId()
{
    return id;
}

void TopologyEdge::addNeighborTriangle(unsigned int neighbor)
{
    neighborTriangles.push_back(neighbor);
}

void TopologyEdge::removeNeighborTriangle(unsigned int neighbor)
{
    neighborTriangles.remove(neighbor);
}

void TopologyEdge::addNeighborQuad(unsigned int neighbor)
{

    neighborQuads.push_back(neighbor);
}

void TopologyEdge::removeNeighborQuad(unsigned int neighbor)
{
    neighborQuads.remove(neighbor);
}


