#include "topologypoint.h"

unsigned int TopologyPoint::current_id = 0;

TopologyPoint::TopologyPoint()
{

}

TopologyPoint::TopologyPoint(unsigned int id)
{
    this->id = id;
}

TopologyPoint::TopologyPoint(float x=0.0, float y=0.0, float z=0.0): id(++current_id)
{
    //current_id++;
    //this->id = current_id;
    position = glm::vec3(x,y,z);
}

TopologyPoint &TopologyPoint::operator=(const TopologyPoint &other)
{
    this->neighborEdges = other.neighborEdges;
    this->neighborPoints = other.neighborPoints;
    this->id = other.id;
    this->position = other.position;
    this->normal = other.normal;
    this->active = other.active;
    return *this;
}

void TopologyPoint::addNeighborPoint(unsigned int neighbor)
{
    this->neighborPoints.push_back(neighbor);
}

void TopologyPoint::removeNeighborPoint(unsigned int neighbor)
{
    this->neighborPoints.remove(neighbor);
}

void TopologyPoint::addNeighborEdge(unsigned int neighbor)
{
    this->neighborEdges.push_back(neighbor);
}

void TopologyPoint::removeNeighborEdge(unsigned int neighbor)
{
    this->neighborEdges.remove(neighbor);
}

bool TopologyPoint::isPointIdNeighbor(unsigned int neighborId)
{
    return find(neighborPoints.begin(), neighborPoints.end(), neighborId) != neighborPoints.end();
}


void TopologyPoint::setActive(bool active){
    this->active = active;
}

bool TopologyPoint::isActive(){
    return active;
}

void TopologyPoint::setPosition(float x, float y, float z){
    position = glm::vec3(x,y,z);
}

glm::vec3 TopologyPoint::getPosition(){
   return this->position;
}

void TopologyPoint::setNormal(float x, float y, float z){
    normal = glm::vec3(x,y,z);
}

glm::vec3 TopologyPoint::getNormal(){
   return this->normal;
}

unsigned int TopologyPoint::getId(){
    return this->id;
}

bool TopologyPoint::operator==(const TopologyPoint &other) const {
    return (other.id == this->id);
}
