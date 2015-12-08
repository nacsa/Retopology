#ifndef MODELTORETOPO_H
#define MODELTORETOPO_H

#include "modelimporter.h"
#include "topology.h"

class ModelToRetopo
{

public:
    ModelToRetopo();
    void importModelAsRetopo(const char* fileName, Topology* topology);
private:
    bool isPointSameAsVertieces(TopologyPoint point, float x, float y, float z);
    bool isPointSameAsVertiecesSort(TopologyPoint point, float x, float y, float z);
};

#endif // MODELTORETOPO_H
