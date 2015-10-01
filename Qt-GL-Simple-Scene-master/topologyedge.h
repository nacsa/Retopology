#ifndef TOPOLOGYEDGE_H
#define TOPOLOGYEDGE_H

#include <list>

class TopologyEdge
{
private:
    unsigned int id;

public:
    static unsigned int current_id;
    int pointId1;
    int pointId2;


    std::list<unsigned int> neighborTriangles;
    std::list<unsigned int> neighborQuads;

    TopologyEdge(unsigned int id, int i1, int i2);
    TopologyEdge(int i1, int i2);
    unsigned int getId();


    void addNeighborTriangle(unsigned int neighbor);
    void removeNeighborTriangle(unsigned int neighbor);
    void addNeighborQuad(unsigned int neighbor);
    void removeNeighborQuad(unsigned int neighbor);

};

#endif // TOPOLOGYEDGE_H
