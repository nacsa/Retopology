#include "modeltoretopo.h"
#include "topologypoint.h"

ModelToRetopo::ModelToRetopo()
{
}

void ModelToRetopo::importModelAsRetopo(const char *fileName, Topology *topology)
{
    ModelImporter importer;
    std::vector<unsigned int> indices;
    std::vector<float> vertices;
    std::vector<float> normals;
    bool successfulImport = importer.loadOBJ(fileName, indices, vertices, normals);
    if(successfulImport){
        int numberOfPoints = vertices.size() / 3;
        int numberOfTriangles = indices.size() / 3;
        std::vector<unsigned int> pointVertexMap;
        std::vector<unsigned int> pointIdIndexMap;
        pointVertexMap.resize(numberOfPoints);
        std::vector<TopologyPoint> points;
        bool pointAlreadyIn = false;

        for(int i = 0; i < numberOfPoints; i++){
            pointAlreadyIn = false;
            for(int j = 0; j < points.size(); j++){
                TopologyPoint point = points[j];
                if(isPointSameAsVertiecesSort(point, vertices[i*3], vertices[i*3 + 1], vertices[i*3 + 2])){
                   glm::vec3 norm = point.getNormal();
                   point.setNormal(norm.x + normals[i*3], norm.y + normals[i*3 + 1], norm.z + normals[i*3 + 2]);
                   pointAlreadyIn = true;
                   pointVertexMap[i] = j;
                   points[j] = point;
                   break;
                }
            }
            if(!pointAlreadyIn){
                TopologyPoint tmpPoint(vertices[i*3], vertices[i*3 + 1], vertices[i*3 + 2]);
                tmpPoint.setNormal(normals[i*3], normals[i*3 + 1], normals[i*3 + 2]);
                points.push_back(tmpPoint);
                pointVertexMap[i] = points.size()-1;
                //unsigned int pointId = topology->addPoint(tmpPoint);
                //pointIds[points.size()] = pointId;
            }
        }
        int size = points.size();
        pointIdIndexMap.resize(size);
        for(int i = 0; i < points.size(); i++){
            TopologyPoint point = points[i];
            glm::vec3 norm = glm::normalize(point.getNormal());
            point.setNormal(norm.x, norm.y, norm.z);
            glm::vec3 pos = point.getPosition();
            unsigned int pointId = topology->addPoint(point);
            pointIdIndexMap[i] = pointId;
        }

        unsigned int edgeId;
        for(int i = 0; i < numberOfTriangles; i++){
            unsigned int pointIdx1 = pointIdIndexMap[pointVertexMap[indices[i * 3 + 0]]];
            unsigned int pointIdx2 = pointIdIndexMap[pointVertexMap[indices[i * 3 + 1]]];
            unsigned int pointIdx3 = pointIdIndexMap[pointVertexMap[indices[i * 3 + 2]]];

            edgeId = topology->addEdge(pointIdx1, pointIdx2);
            topology->addTriangle(edgeId, pointIdx1, pointIdx2, pointIdx3);
            edgeId = topology->addEdge(pointIdx1, pointIdx3);
            topology->addTriangle(edgeId, pointIdx1, pointIdx2, pointIdx3);
            edgeId = topology->addEdge(pointIdx2, pointIdx3);
            topology->addTriangle(edgeId, pointIdx1, pointIdx2, pointIdx3);
        }
    }
}


float epsilon = 0.0001f;

bool ModelToRetopo::isPointSameAsVertieces(TopologyPoint point, float x, float y, float z){
    return isPointSameAsVertiecesSort(point, x, y, z) ||
            isPointSameAsVertiecesSort(point, x, z, y) ||
            isPointSameAsVertiecesSort(point, y, x, z) ||
            isPointSameAsVertiecesSort(point, y, z, x) ||
            isPointSameAsVertiecesSort(point, z, x, y) ||
            isPointSameAsVertiecesSort(point, z, y, x);
}

bool ModelToRetopo::isPointSameAsVertiecesSort(TopologyPoint point, float x, float y, float z){
   glm::vec3 pos = point.getPosition();
   return (pos.x < x + epsilon && pos.x > x - epsilon &&
      pos.y < y + epsilon && pos.y > y - epsilon &&
      pos.z < z + epsilon && pos.z > z - epsilon);
}
