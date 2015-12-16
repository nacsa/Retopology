#include "vertexisland.h"

#include "topologypoint.h"


VertexIsland::VertexIsland(Topology *topology)
{
    this->topology = topology;
}

void VertexIsland::addVertexIds(std::list<unsigned int> vertexIds)
{
    for(auto vertexId : vertexIds){
        this->vertexIds.push_back(vertexId);
    }
    calculateCenter();
}

void VertexIsland::setVertexId(unsigned int vertexId)
{
    clear();
    vertexIds.push_back(vertexId);
    calculateCenter();
}

void VertexIsland::clear()
{
    vertexIds.clear();
}

void VertexIsland::resetVertexStartPositions()
{
    startVertexPositionMap.clear();
    for(unsigned int vertexId : vertexIds){
        TopologyPoint point = topology->newPoints[topology->pointIdToIndex(vertexId)];
        startVertexPositionMap[vertexId] = point.getPosition();
    }
}

void VertexIsland::resetVertexStartNormals()
{
    startVertexNormalMap.clear();
    for(unsigned int vertexId : vertexIds){
        TopologyPoint point = topology->newPoints[topology->pointIdToIndex(vertexId)];
        startVertexNormalMap[vertexId] = point.getNormal();
    }
}

void VertexIsland::updateModelMatrix()
{
    modelMatrix = glm::translate(glm::mat4(1), center);
}

void VertexIsland::updateIslandPosition()
{
    glm::mat4 translateMatrix = glm::translate(modelMatrix, -center);
    glm::vec3 newCenter = glm::vec3(translateMatrix * glm::vec4(center, 1));
    glm::vec3 diff = newCenter - center;

    for(unsigned int vertexId : vertexIds){
        TopologyPoint point = topology->newPoints[topology->pointIdToIndex(vertexId)];
        glm::vec3 newPos = point.getPosition() + diff;
        topology->movePoint(vertexId, newPos.x, newPos.y, newPos.z);
    }

    center = newCenter;
}

void VertexIsland::updateIslandRotation()
{
    glm::mat4 translateMatrix = glm::translate(modelMatrix, -center);
    //glm::vec3 newCenter = glm::vec3(translateMatrix * glm::vec4(center, 1));
    //glm::vec3 diff = newCenter - center;

    for(unsigned int vertexId : vertexIds){
        //TopologyPoint point = topology->newPoints[topology->pointIdToIndex(vertexId)];
        glm::vec3 newPos = glm::vec3(translateMatrix * glm::vec4(startVertexPositionMap[vertexId], 1));
        glm::vec3 newNorm = glm::vec3(translateMatrix * glm::vec4(startVertexNormalMap[vertexId], 1));
        topology->movePoint(vertexId, newPos.x, newPos.y, newPos.z, newNorm.x, newNorm.y, newNorm.z);
    }

}

void VertexIsland::updateIslandScaling()
{
   glm::mat4 translateMatrix = glm::translate(modelMatrix, -center);

    for(unsigned int vertexId : vertexIds){
        //glm::vec3 newPos = glm::vec3(translateMatrix * glm::vec4(point.getPosition()-center, 1));
        glm::vec3 newPos =  glm::vec3(translateMatrix * glm::vec4(startVertexPositionMap[vertexId], 1));
        topology->movePoint(vertexId, newPos.x, newPos.y, newPos.z);
    }
}

void VertexIsland::calculateCenter()
{
    glm::vec3 sumCoord;
    for(unsigned int vertexId : vertexIds){
        TopologyPoint point = topology->newPoints[topology->pointIdToIndex(vertexId)];
        sumCoord = sumCoord + point.getPosition();
    }
    sumCoord = sumCoord / (float)vertexIds.size();
    center = sumCoord;
}
