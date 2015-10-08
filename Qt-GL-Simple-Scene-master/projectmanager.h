#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <string>
#include "topology.h"


/*
 * FileFormat:
 *
 * model path
 * p
 * posX(float), posY(float), posZ(float), norX(float), normY(float), normZ(float),
 * ...
 * e
 * pointId1(int), pointId2(int)
 * ...
 * t
 * pointId1(int), pointId2(int), pointId3(int), edgeId1(int), edgeId2(int), edgeId3(int)
 * ...
 * q
 * pointId1(int), pointId2(int), pointId3(int), pointId4(int), edgeId1(int), edgeId2(int), edgeId3(int), edgeId4(int)
 *
 */
class ProjectManager
{

    std::string modelPath;

public:
    ProjectManager();
    void saveProject(const std::string &path, const Topology &topology);
    void openProject(const std::string &path, Topology &topology);
    void setModelPath(std::string path);
};

#endif // PROJECTMANAGER_H
