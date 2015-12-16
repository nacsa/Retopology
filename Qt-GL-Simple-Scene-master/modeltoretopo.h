#ifndef MODELTORETOPO_H
#define MODELTORETOPO_H

#include <map>
#include "modelimporter.h"
#include "topology.h"

class ModelToRetopo
{

public:
    ModelToRetopo();
    void importModelAsRetopo(const char* fileName, Topology* topology);
    void exportRetopoAsModel(const char* fileName, Topology* topology);
private:
    std::map<unsigned int, int> pointIdToIndexMap;
    bool isPointSameAsVertieces(TopologyPoint point, float x, float y, float z);
    bool isPointSameAsVertiecesSort(TopologyPoint point, float x, float y, float z);
};

#endif // MODELTORETOPO_H
