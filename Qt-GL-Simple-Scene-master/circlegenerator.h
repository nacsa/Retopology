#ifndef CIRCLEGENERATOR_H
#define CIRCLEGENERATOR_H

#include "shader.hpp"
#include <glm/core/func_trigonometric.hpp>
#include <glm/core/func_matrix.hpp>
#include <GL/gl3w.h>

class CircleGenerator
{
    bool shouldDraw;

    static const int numberOfVertieces = 100;
    float vertieces[numberOfVertieces*3];

    Shader* shader;
    GLuint vaoHandle;

    GLuint vertexBuffer;

    void bindBufferData();
    void bindCoordinates();

public:
    CircleGenerator();

    void setShouldDraw(bool shouldDraw);
    bool isShouldDraw();

    void init(GLuint vaoHandle);

    void draw(glm::mat4 projection, glm::mat4 modelview, glm::vec3 color);

    void allowedDraw();
};

#endif // CIRCLEGENERATOR_H
