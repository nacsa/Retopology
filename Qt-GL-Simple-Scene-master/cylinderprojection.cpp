#include "cylinderprojection.h"

#include "topologypoint.h"


CylinderProjection::CylinderProjection()
{

    /* kocka */
    verticalGridSize = 40;
    horizontalGridSize = 4;
    height = 0.6f;
    radius = 1.0f;
    basePointPos = glm::vec3(0,-0.3,0);
    baseUpDirection = glm::vec3(0,1,0);
    baseRayDirection = glm::vec3(1,0,0.95);
    topPointPos = basePointPos + height * baseUpDirection;

    baseRayDirection = glm::normalize(baseRayDirection);
    /*lamp*//*
    verticalGridSize = 10;
    horizontalGridSize = 30;
    height = 5.2f;
    radius = 0.5f;
    basePointPos = glm::vec3(0,-1.0,0);
    baseUpDirection = glm::vec3(0,1,0);
    baseRayDirection = glm::vec3(1,0,0);
    */

}


std::list<glm::mat4> CylinderProjection::getProjectionMatrices()
{
    glm::vec3 actualRayDir = baseRayDirection;

    glm::mat4 rotationMatrix(1.0);

    float deltaAngle = 360.0f/(float)horizontalGridSize;
    float deltaHeight = height / (float) (verticalGridSize - 1);

    // Very first camera position
    centerPositions.push_back(basePointPos);
    cameraPositions.push_back(actualRayDir * radius);



    // First row of cameras
    for(int j = 1; j < horizontalGridSize; j++){

        rotationMatrix = glm::rotate(rotationMatrix, deltaAngle, baseUpDirection);
        actualRayDir = glm::vec3( rotationMatrix * glm::vec4(baseRayDirection, 1.0));

        cameraPositions.push_back(actualRayDir * radius);
    }


    // Remaining rows of cameras
    glm::vec3 lowerCameraPos;
    glm::vec3 additiveHeightVec;
    for(int i = 1; i < verticalGridSize; i++){
        additiveHeightVec = baseUpDirection * (deltaHeight * i);
        centerPositions.push_back(basePointPos + additiveHeightVec);
        for(int j = 0; j < horizontalGridSize; j++){
            lowerCameraPos = cameraPositions[get1DIndexFrom2DIndex(0, j)];
            cameraPositions.push_back(lowerCameraPos + additiveHeightVec );
        }
    }

    for(int i = 0; i < verticalGridSize; i++){
        for(int j = 0; j < horizontalGridSize; j++){
            projectMatrices.push_back(
                        glm::lookAt(
                            cameraPositions[get1DIndexFrom2DIndex(i,j)],
                            centerPositions[i],
                            baseUpDirection)
                        );
        }
    }

    cameraPositions.clear();
    centerPositions.clear();

    return projectMatrices;
}

void CylinderProjection::projectCylinderToModel(std::vector<glm::vec3> pointPositions, std::vector<glm::vec3> pointNormals, Topology& topology)
{
    auto pointIds = new unsigned int[verticalGridSize * horizontalGridSize];
    TopologyPoint point;

    // first point;
    point = TopologyPoint(pointPositions[0].x, pointPositions[0].y, pointPositions[0].z);
    point.setNormal(pointNormals[0].x, pointNormals[0].y, pointNormals[0].z);

    pointIds[index(0,0)] = topology.addPoint(point);

    // first row
    glm::vec3 pos;
    glm::vec3 norm;
    unsigned int edgeId;
    for(int i = 1; i < verticalGridSize; i++){
        pos = pointPositions[get1DIndexFrom2DIndex(i,0)];
        norm = pointNormals[get1DIndexFrom2DIndex(i,0)];

        point = TopologyPoint(pos.x, pos.y, pos.z);
        point.setNormal(norm.x, norm.y, norm.z);

        pointIds[index(i,0)] = topology.addPoint(point);

        topology.addEdge(pointIds[index(i-1, 0)], pointIds[index(i, 0)]);
    }
    // edge between last and first point
    topology.addEdge(pointIds[index(verticalGridSize - 1, 0)], pointIds[index(0, 0)]);


    // Remaining rows first points
    for(int j = 1; j < horizontalGridSize; j++){
        for(int i = 0; i < verticalGridSize; i++){

            pos = pointPositions[index(i,j)];
            norm = pointNormals[index(i,j)];

            point = TopologyPoint(pos.x, pos.y, pos.z);
            point.setNormal(norm.x, norm.y, norm.z);

            pointIds[index(i,j)] = topology.addPoint(point);

            //edge to the line under the current
            topology.addEdge(pointIds[index(i, j-1)], pointIds[index(i, j)]);

            //edge to the previous point in the line
            if(i > 0){
                edgeId = topology.addEdge(pointIds[index(i-1, j)], pointIds[index(i, j)]);
                topology.testQuadration(edgeId);
            }
        }
        edgeId = topology.addEdge(pointIds[index(verticalGridSize - 1, j)], pointIds[index(0, j)]);
        topology.testQuadration(edgeId);

    }

}

int CylinderProjection::get1DIndexFrom2DIndex(int i, int j)
{
    return i  + j * verticalGridSize;
}

int CylinderProjection::index(int i, int j)
{
    return get1DIndexFrom2DIndex(i,j);
}

//TODO: void drawCylinder(){}


/*

int CylinderProjection::get1DIndexFrom2DIndex(int i, int j)
{
    return i * horizontalGridSize + j;
}

int CylinderProjection::index(int i, int j)
{
    r

void CylinderProjection::projectCylinderToModel(std::vector<glm::vec3> pointPositions, std::vector<glm::vec3> pointNormals, Topology& topology)
{
    auto pointIds = new unsigned int[verticalGridSize * horizontalGridSize];
    TopologyPoint point;

    // first point;
    point = TopologyPoint(pointPositions[0].x, pointPositions[0].y, pointPositions[0].z);
    point.setNormal(pointNormals[0].x, pointNormals[0].y, pointNormals[0].z);

    pointIds[index(0,0)] = topology.addPoint(point);

    // first row
    glm::vec3 pos;
    glm::vec3 norm;
    unsigned int edgeId;
    for(int j = 1; j < horizontalGridSize; j++){
        pos = pointPositions[get1DIndexFrom2DIndex(0,j)];
        norm = pointNormals[get1DIndexFrom2DIndex(0,j)];

        point = TopologyPoint(pos.x, pos.y, pos.z);
        point.setNormal(norm.x, norm.y, norm.z);

        pointIds[index(0,j)] = topology.addPoint(point);

        topology.addEdge(pointIds[index(0, j-1)], pointIds[index(0, j)]);
    }
    // edge between last and first point
    topology.addEdge(pointIds[index(0, horizontalGridSize - 1)], pointIds[index(0, 0)]);


    // Remaining rows first points
    for(int i = 1; i < verticalGridSize; i++){
        for(int j = 0; j < horizontalGridSize; j++){
            pos = pointPositions[get1DIndexFrom2DIndex(i,j)];
            norm = pointNormals[get1DIndexFrom2DIndex(i,j)];

            point = TopologyPoint(pos.x, pos.y, pos.z);
            point.setNormal(norm.x, norm.y, norm.z);

            pointIds[index(i,j)] = topology.addPoint(point);

            topology.addEdge(pointIds[index(i-1, j)], pointIds[index(i, j)]);
            if(j > 0){
                edgeId = topology.addEdge(pointIds[index(i, j-1)], pointIds[index(i, j)]);
                topology.testQuadration(edgeId);
            }
        }
        edgeId = topology.addEdge(pointIds[index(i, horizontalGridSize - 1)], pointIds[index(i, 0)]);
        topology.testQuadration(edgeId);

    }

}



*/
