#include "circlegenerator.h"


CircleGenerator::CircleGenerator()
{


}

void CircleGenerator::bindBufferData()
{
    GLuint vertexSize = 3 * sizeof(GLfloat);
    glBindVertexArray(vaoHandle);

    if(vertexBuffer == 0)
        glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfVertieces * 3 * sizeof(float), vertieces, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    glBindVertexArray(0);
}


void CircleGenerator::bindCoordinates()
{
    float pi = 3.1415926;
    float theta = 2 * 3.1415926 / (float)numberOfVertieces;
    float c = glm::cos(theta);
    float s = glm::sin(theta);
    float t;
    float r = 1.0f;
    float x = 1;
    float y = 0;
    float z = 0;

    for(int i = 0; i < numberOfVertieces; i++){
        theta = 2 * pi * (float)i / (float)numberOfVertieces;
        vertieces[i * 3] = r * glm::cos(theta);
        vertieces[i * 3 + 1] = 0;
        vertieces[i * 3 + 2] = r * glm::sin(theta);
        /*
        vertieces[i * 3] = x;
        vertieces[i * 3 + 1] = y;
        vertieces[i * 3 + 2] = 0;
        */

        t = x;
        x = c * x - s * y;
        y = s * x + c * y;
    }
}


void CircleGenerator::setShouldDraw(bool shouldDraw)
{
    this->shouldDraw = shouldDraw;
}

bool CircleGenerator::isShouldDraw()
{
    return shouldDraw;
}


void CircleGenerator::init(GLuint vaoHandle)
{
    this->vaoHandle = vaoHandle;

    shader = new Shader();
    shader->load(":/circle_vertex", ":/circle_fragment");

    bindCoordinates();
    bindBufferData();
}

void CircleGenerator::draw(glm::mat4 projection, glm::mat4 modelview, glm::vec3 color)
{
    shader->enable();

    shader->bindUniformMatrixMat4("projection",projection);
    shader->bindUniformMatrixMat4("modelview", modelview);
    shader->bindUniformFloat3("outColor", color.x, color.y, color.z);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(vaoHandle);
    glLineWidth(3);
    //glDrawElements(GL_LINES, numberOfVertieces * 2, GL_UNSIGNED_INT, NULL);
    glDrawArrays(GL_LINE_LOOP, 0, numberOfVertieces);
    //glDisable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    shader->disable();

}

void CircleGenerator::allowedDraw()
{


}
