#ifndef CYLINDERGENERATOR_H
#define CYLINDERGENERATOR_H

#include <glm/glm.hpp>
#include "circlegenerator.h"

class CylinderGenerator
{
    bool shouldDraw;
    bool updateVerticalLines;
    glm::vec3 defaultCircleUpDirection;
    CircleGenerator* circle;

    float* vertieces;

    Shader* shader;
    GLuint vaoHandle;

    GLuint vertexBuffer;

    void bindBufferData();
    void bindCoordinates();

public:

    float height;
    float radius;


    glm::vec3 basePointPos;
    glm::vec3 topPointPos;
    glm::vec3 baseUpDirection; //normalized
    glm::vec3 baseRayDirection; //normalized

    int verticalGridSize;
    int horizontalGridSize;

    CylinderGenerator();

    void init(CircleGenerator* CircleGenerator, GLuint vaoHandle);

    void setShouldDraw(bool shouldDraw);
    bool isShouldDraw();

    void draw(glm::mat4 projection, glm::mat4 modelview);
    void allowedDraw(glm::mat4 projection, glm::mat4 modelview);
};

#endif // CYLINDERGENERATOR_H
