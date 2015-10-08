#include "projectmanager.h"

#include "topologypoint.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

enum InputState{PATH_, POINTS_, EDGES_, TRIANGLES_, QUADS_};

ProjectManager::ProjectManager()
{
}

void ProjectManager::saveProject(const std::string &path, const Topology &topology)
{
    ofstream outputFile;
    outputFile.open(path);

    outputFile << modelPath << endl;
    outputFile << "p " << topology.newPoints.size() << " " << TopologyPoint::current_id << endl;
    for(auto point : topology.newPoints){
        outputFile << point.getId() << " " << point.getPosition().x << " " << point.getPosition().y << " " << point.getPosition().z
                   << " " << point.getNormal().x << " " << point.getNormal().y << " " << point.getNormal().z << endl;
    }
    outputFile << "e " << topology.edges.size() << " " << TopologyEdge::current_id << endl;
    for(auto edge : topology.edges){
        outputFile << edge.getId() << " " << edge.pointId1 << " " << edge.pointId2 << endl;
    }
    outputFile << "t " << topology.triangles.size() << " " << TopologyHelperTriangle::current_id << endl;
    for(auto triangle : topology.triangles){
        outputFile << triangle.getId() << " " << triangle.pointId1 << " " << triangle.pointId2 << " " << triangle.pointId3
                   << " " << triangle.edgeId1 << " " << triangle.edgeId2 << " " << triangle.edgeId3 << endl;
    }
    outputFile << "q " << topology.quads.size() << " " << TopologyHelperQuad::current_id << endl;
    for(auto quad : topology.quads){
        outputFile << quad.getId() << " " << quad.pointId1 << " " << quad.pointId2 << " " << quad.pointId3 << " " << quad.pointId4
                   << " " << quad.edgeId1 << " " << quad.edgeId2 << " " << quad.edgeId3 << " " << quad.edgeId4 << endl;
    }

    outputFile.close();
}

void ProjectManager::openProject(const std::string &path, Topology& topology)
{
    ifstream inputFile;
    inputFile.open(path);
    //TODO: ellenorizni
    if(inputFile.fail()){
        //ERROR
        return;
    }
    InputState state = PATH_;
    string line;
    std::istringstream iss;
    char c;
    int n, i;
    unsigned int id, current_id, pointId1, pointId2, pointId3, pointId4, edgeId1, edgeId2, edgeId3, edgeId4;
    float posX, posY, posZ, normX, normY, normZ;
    //Read model path
    getline(inputFile, line);
    string modelPath = line;

    //Read points
    getline(inputFile, line);
    iss = istringstream(line);
    iss >> c >> n >> current_id;
    printf("read points: %d \n", n);
    TopologyPoint::current_id = current_id;
    for(i = 0; i < n; i++){
        getline(inputFile, line);
        iss = istringstream(line);
        iss >> id >> posX >> posY >> posZ >> normX >> normY >> normZ;
        TopologyPoint point(id);
        point.setPosition(posX, posY, posZ);
        point.setNormal(normX, normY, normZ);
        topology.addPoint(point);
    }

    //Read edges
    getline(inputFile, line);
    iss = istringstream(line);
    iss >> c >> n >> current_id;
    printf("read edges: %d \n", n);
    TopologyEdge::current_id = current_id;
    for(i = 0; i < n; i++){
        getline(inputFile, line);
        iss = istringstream(line);
        iss >> id >> pointId1 >> pointId2;
        TopologyEdge edge(id, pointId1, pointId2);
        topology.addEdge(edge);
    }

    //Read triangles
    getline(inputFile, line);
    iss = istringstream(line);
    iss >> c >> n >> current_id;
    printf("read triangles: %d \n", n);
    TopologyHelperTriangle::current_id = current_id;
    for(i = 0; i < n; i++){
        getline(inputFile, line);
        iss = istringstream(line);
        iss >> id >> pointId1 >> pointId2 >> pointId3 >> edgeId1 >> edgeId2 >> edgeId3;
        TopologyHelperTriangle triangle(id, pointId1, pointId2, pointId3, edgeId1, edgeId2, edgeId3);
        topology.addTriangle(triangle);
    }

    //Read quads
    getline(inputFile, line);
    iss = istringstream(line);
    iss >> c >> n >> current_id;
    printf("read quads: %d \n", n);
    TopologyHelperQuad::current_id = current_id;
    for(i = 0; i < n; i++){
        getline(inputFile, line);
        iss = istringstream(line);
        iss >> id >> pointId1 >> pointId2 >> pointId3 >> pointId4 >> edgeId1 >> edgeId2 >> edgeId3 >> edgeId4;
        TopologyHelperQuad quad(id, pointId1, pointId2, pointId3, pointId4, edgeId1, edgeId2, edgeId3, edgeId4);
        topology.addQuad(quad);
    }

}

void ProjectManager::setModelPath(string path)
{
    this->modelPath = path;
}

