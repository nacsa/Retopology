#include "cylindergenerator.h"
#include <glm/gtc/matrix_transform.hpp>
#include <qfile.h>
#include <qresource.h>

CylinderGenerator::CylinderGenerator()
{
    shouldDraw = false;
    shouldDrawLine = false;
    updateVerticalLines = true;
    verticalGridSize = 40;
    horizontalGridSize = 4;
    height = 0.6f;
    radius = 1.0f;
    basePointPos = glm::vec3(0,-0.3,0);
    baseUpDirection = glm::vec3(0,1,0);
    baseRayDirection = glm::vec3(1,0,0.95);
    defaultCircleUpDirection = glm::vec3(0,1,0);
    topPointPos = basePointPos + height * baseUpDirection;

    baseRayDirection = glm::normalize(baseRayDirection);

    vertexBuffer =0;
}

void CylinderGenerator::init(CircleGenerator *CircleGenerator, GLuint vaoHandle)
{
    this->circle = CircleGenerator;
    this->vaoHandle = vaoHandle;
    //glGenVertexArrays(1, vaoHandle);

    shader = new Shader();
    shader->load(":/circle_vertex",
                 ":/circle_fragment");

    //lineShader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\selectrect_vertex.glsl",
    //                            "D:\\msconlab\\Qt-GL-Simple-Scene-master\\selectrect_fragment.glsl");
    //lineShader = new Shader(QResource(":/selectrect_vertex").absoluteFilePath().toLatin1().data(),
    //                        QResource("qrc:///shaders/selectrect_fragment.glsl").absoluteFilePath().toLatin1().data());
    lineShader = new Shader();
    lineShader->load(":/selectrect_vertex",
                           ":/selectrect_fragment");

    bindCoordinates();
    bindBufferData();
}

void CylinderGenerator::draw(glm::mat4 projection, glm::mat4 modelview)
{
    float pi = 3.1415926;
    float rotAngle;
    glm::vec3 rotationVector = glm::cross(defaultCircleUpDirection, baseUpDirection);
    if(rotationVector.x == 0 && rotationVector.y == 0 && rotationVector.z == 0)
        rotAngle = 0;
    else{
        rotAngle = glm::acos(glm::dot(defaultCircleUpDirection, baseUpDirection)/glm::length(baseUpDirection));
        rotAngle *= 180.0f/pi;
    }
    glm::mat4 translatedModelview = glm::translate(modelview, basePointPos);

    glm::mat4 rotatedModelview = translatedModelview;
    if(rotAngle != 0)
        rotatedModelview = glm::rotate(translatedModelview, rotAngle, rotationVector);

    glm::mat4 scaledCircleModelview = glm::scale(rotatedModelview, glm::vec3(radius, 1, radius));

    glm::mat4 circleModelview;
    float portion = height / ((float)horizontalGridSize - 1.0f);
    for(int i = 0; i < horizontalGridSize; i++){
        circleModelview = glm::translate(scaledCircleModelview, glm::vec3(0, portion * ((float)i), 0));
        circle->draw(projection, circleModelview, glm::vec3(1, 0, 0));
    }

    bindCoordinates();
    bindBufferData();

    shader->enable();

    shader->bindUniformMatrixMat4("projection",projection);
    shader->bindUniformMatrixMat4("modelview", rotatedModelview);
    shader->bindUniformFloat3("outColor", 1, 0, 0);

    glBindVertexArray(vaoHandle);
    glLineWidth(3);

    glDrawArrays(GL_LINES, 0, verticalGridSize * 2 * 3);


    glBindVertexArray(0);
    shader->disable();
}

void CylinderGenerator::allowedDraw(glm::mat4 projection, glm::mat4 modelview)
{
    if(shouldDraw){
        draw(projection, modelview);
    }
}

void CylinderGenerator::setShouldDrawLine(bool shouldDrawLine)
{
    this->shouldDrawLine = shouldDrawLine;
}

bool CylinderGenerator::isShouldDrawLine()
{
    return shouldDrawLine;
}

void CylinderGenerator::setShouldDraw(bool shouldDraw)
{
    this->shouldDraw = shouldDraw;
}

bool CylinderGenerator::isShouldDraw()
{
    return shouldDraw;
}

void CylinderGenerator::setLineStart(float x, float y, float z)
{
    lineVertices[0] = x;
    lineVertices[1] = y;
    lineVertices[2] = z;
    lineVertices[3] = x;
    lineVertices[4] = y;
    lineVertices[5] = z;
    bindLineBufferData();
}

void CylinderGenerator::setLineEnd(float x, float y, float z)
{
    lineVertices[3] = x;
    lineVertices[4] = y;
    lineVertices[5] = z;
    bindLineBufferData();
}

void CylinderGenerator::drawLine()
{
    lineShader->enable();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(vaoHandle);
    glLineWidth(3);
    glDrawArrays(GL_LINES, 0, 2 * 3);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    lineShader->disable();
}

void CylinderGenerator::allowedDrawLine()
{
    if(shouldDrawLine){
        drawLine();
    }
}

void CylinderGenerator::bindLineBufferData()
{
    GLuint vertexSize = 3 * sizeof(GLfloat);
    glBindVertexArray(vaoHandle);

    if(vertexBuffer == 0)
        glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(float), lineVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    glBindVertexArray(0);
}

void CylinderGenerator::bindBufferData()
{
    GLuint vertexSize = 3 * sizeof(GLfloat);
    glBindVertexArray(vaoHandle);

    if(vertexBuffer == 0)
        glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, verticalGridSize * 2 * 3 * sizeof(float), vertieces, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    glBindVertexArray(0);
}


void CylinderGenerator::bindCoordinates()
{
    float anglePorition = 360.0f / (float)verticalGridSize;
    vertieces = new float[verticalGridSize * 2 * 3];

    glm::mat4 rotationMatrix = glm::mat4(1.0);

    vertieces[0] = baseRayDirection.x * radius;
    vertieces[1] = 0;
    vertieces[2] = baseRayDirection.z * radius;
    vertieces[3] = baseRayDirection.x * radius;
    vertieces[4] = height;
    vertieces[5] = baseRayDirection.z * radius;

    for(int i = 1; i < verticalGridSize; i++){
        rotationMatrix = glm::mat4(1.0);
        rotationMatrix = glm::rotate(rotationMatrix, anglePorition * (float)i, glm::vec3(0,1,0));
        glm::vec3 rotatedRay = glm::vec3( rotationMatrix * glm::vec4(baseRayDirection, 1.0) );
        vertieces[i*6] = rotatedRay.x * radius;
        vertieces[i*6 + 1] = 0;
        vertieces[i*6 + 2] = rotatedRay.z * radius;
        vertieces[i*6 + 3] = rotatedRay.x * radius;
        vertieces[i*6 + 4] = height;
        vertieces[i*6 + 5] = rotatedRay.z * radius;
    }
}
