#include "constraintprojection.h"
#include "MeshMorph.h"

ConstraintProjection::ConstraintProjection()
{
    activePointId = 0;
}

void ConstraintProjection::bindBufferData()
{
    std::vector<float> vertexPos;
    vertexPos.resize(pointConstraintPointMap.size()*3);
    int i = 0;
    for(auto pointConstraintPoint : pointConstraintPointMap){
        glm::vec3 pos = (pointConstraintPoint.second).pos;
        vertexPos[i * 3 + 0] = pos.x;
        vertexPos[i * 3 + 1] = pos.y;
        vertexPos[i * 3 + 2] = pos.z;
        i++;
    }

    GLuint vertexSize = 3 * sizeof(GLfloat);
    glBindVertexArray(vaoHandle);

    if(vertexBuffer == 0)
        glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexPos.size() * sizeof(float), &vertexPos[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, verticalGridSize * 2 * 3 * sizeof(float), vertieces, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    glBindVertexArray(0);
}

void ConstraintProjection::init(Topology *topology, GLuint vaoHandle)
{
    this->enabled = false;
    this->topology = topology;
    this->vaoHandle = vaoHandle;
    shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\constraintproj_vertex.glsl",
                        "D:\\msconlab\\Qt-GL-Simple-Scene-master\\constraintproj_fragment.glsl",
                        "D:\\msconlab\\Qt-GL-Simple-Scene-master\\constraintproj_geometry.glsl");
}

void ConstraintProjection::allowedDraw(glm::mat4 projection, glm::mat4 modelview)
{
    if(enabled && pointConstraintPointMap.size() != 0)
        draw(projection, modelview);
}

void ConstraintProjection::draw(glm::mat4 projection, glm::mat4 modelview)
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_POINT);
    glPolygonOffset( -1.0, -10.0 );
    shader->enable();
    shader->bindUniformMatrixMat4("projection",projection);
    shader->bindUniformMatrixMat4("modelview", modelview);
    //topology_shader->bindUniformFloat("snappingId", (float)snappingPointId);
    glBindVertexArray(vaoHandle);

    glPointSize(6);

    //glDrawElements(GL_POINTS, topology.newPoints.size(), GL_UNSIGNED_INT, NULL);
    glDrawArrays(GL_POINTS, 0, pointConstraintPointMap.size());
    glBindVertexArray(0);

    shader->disable();
    glDisable(GL_POLYGON_OFFSET_POINT);
    glEnable(GL_CULL_FACE);
}

void ConstraintProjection::startConstraintProj(std::list<unsigned int> pointIds)
{
    activePointId = 0;
    this->pointIds.resize(pointIds.size());
    int i = 0;
    for(auto pointId : pointIds){
        this->pointIds[i] = pointId;
        i++;
    }
    topology->saveState();
}

void ConstraintProjection::setActivePoint(unsigned int id)
{
    activePointId = id;
}

void ConstraintProjection::deleteConstraintPoint()
{
    pointConstraintPointMap.erase(activePointId);
    topology->removeConstraintProjPoint(activePointId);
    bindBufferData();
}

void ConstraintProjection::addConstraintPoint(glm::vec3 pos)
{
    if(activePointId == 0)
        return;
    pointConstraintPointMap.erase(activePointId);
    pointConstraintPointMap[activePointId] = ConstraintPoint(pos);
    topology->addConstraintProjPoint(activePointId);
    bindBufferData();
}

void ConstraintProjection::apply()
{
    int pointNum = pointIds.size();
    float* vertices = new float[pointNum * 3];
    float* normals = new float[pointNum * 3];
    float* verticesOut = new float[pointNum * 3];
    int featureNum = pointConstraintPointMap.size();
    unsigned short* featureIndices = new unsigned short[featureNum];
    float* newFeaturePos = new float[featureNum * 3];

    int featureIndex = 0;

    for(int i = 0; i < pointNum; i++){
        unsigned int pointId = pointIds[i];
        TopologyPoint point = topology->newPoints[topology->pointIdToIndex(pointId)];
        glm::vec3 pos = point.getPosition();
        glm::vec3 normal = point.getNormal();
        vertices[i * 3 + 0] = pos.x;
        vertices[i * 3 + 1] = pos.y;
        vertices[i * 3 + 2] = pos.z;

        normals[i * 3 + 0] = normal.x;
        normals[i * 3 + 1] = normal.y;
        normals[i * 3 + 2] = normal.z;

        if(pointConstraintPointMap.count(pointId)){
            featureIndices[featureIndex] = i;
            ConstraintPoint featurePoint = pointConstraintPointMap.at(pointId);
            newFeaturePos[featureIndex * 3 + 0] = featurePoint.pos.x;
            newFeaturePos[featureIndex * 3 + 1] = featurePoint.pos.y;
            newFeaturePos[featureIndex * 3 + 2] = featurePoint.pos.z;

            featureIndex++;
        }
    }

    MeshMorph meshMorph;
    meshMorph.init(vertices, pointNum, featureIndices, featureNum);
    meshMorph.getMorphedData(verticesOut, newFeaturePos);


    for(int i = 0; i < pointNum; i++){
        unsigned int pointId = pointIds[i];
        topology->movePoint(pointId, verticesOut[i * 3 + 0], verticesOut[i * 3 + 1], verticesOut[i * 3 + 2]);
    }
}

void ConstraintProjection::revert()
{
    topology->loadState();
}

void ConstraintProjection::finish()
{
    activePointId = 0;

    for(auto pointId : pointIds){
        topology->removeConstraintProjPoint(pointId);
    }
}

bool ConstraintProjection::isEnabled()
{
    return enabled;
}

void ConstraintProjection::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

unsigned int ConstraintProjection::getActivePointId()
{
    return activePointId;
}
