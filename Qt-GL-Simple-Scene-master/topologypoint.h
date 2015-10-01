#ifndef TOPOLOGYPOINT_H
#define TOPOLOGYPOINT_H

#include <list>
#include <glm/glm.hpp>

class TopologyPoint
{
private:

    unsigned int id;
    glm::vec3 position;
    glm::vec3 normal;
    bool active;

public:
    static unsigned int current_id;

    std::list<unsigned int> neighborPoints;
    std::list<unsigned int> neighborEdges;

    TopologyPoint();
    TopologyPoint(unsigned int id);
    TopologyPoint(float x, float y, float z);
    TopologyPoint& operator=(const TopologyPoint& other);


    void addNeighborPoint(unsigned int neighbor);
    void removeNeighborPoint(unsigned int neighbor);
    void addNeighborEdge(unsigned int neighbor);
    void removeNeighborEdge(unsigned int neighbor);
    bool isPointIdNeighbor(unsigned int neighborId);

    void setActive(bool active);
    bool isActive();
    void setPosition(float x, float y, float z);
    glm::vec3 getPosition();
    void setNormal(float x, float y, float z);
    glm::vec3 getNormal();
    bool operator==(const TopologyPoint &other) const;
    unsigned int getId();

};

#endif // TOPOLOGYPOINT_H
