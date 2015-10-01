#ifndef TOPOLOGYHELPERQUAD_H
#define TOPOLOGYHELPERQUAD_H

class TopologyHelperQuad
{
private:
    unsigned int id;

public:
    static unsigned int current_id;

    unsigned  int pointId1;
    unsigned int pointId2;
    unsigned int pointId3;
    unsigned int pointId4;


    unsigned int edgeId1;
    unsigned int edgeId2;
    unsigned int edgeId3;
    unsigned int edgeId4;

    TopologyHelperQuad(unsigned int id, unsigned i1,unsigned int i2,unsigned int i3,unsigned int i4);
    TopologyHelperQuad(unsigned int i1,unsigned int i2,unsigned int i3,unsigned int i4);

    unsigned int getId();

};

#endif // TOPOLOGYHELPERQUAD_H
