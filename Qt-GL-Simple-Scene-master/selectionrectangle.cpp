#include "selectionrectangle.h"

void SelectionRectangle::bindBufferData()
{
    GLuint vertexSize = 3 * sizeof(GLfloat);
    glBindVertexArray(vaoHandle);

    if(vertexBuffer == 0)
        glGenBuffers(1, &vertexBuffer);

    if(indicesBuffer == 0)
        glGenBuffers(1, &indicesBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), position, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
   // glDisableVertexAttribArray(0);
}

void SelectionRectangle::bindCoordinates()
{
    position[0] = convertCoordinates(startX, width);
    position[1] = convertCoordinates(startY, height);
    position[2] = 0.0;
    position[3] = convertCoordinates(startX, width);
    position[4] = convertCoordinates(endY, height);
    position[5] = 0.0f;
    position[6] = convertCoordinates(endX, width);
    position[7] = convertCoordinates(endY, height);
    position[8] = 0.0f;
    position[9] = convertCoordinates(endX, width);
    position[10] = convertCoordinates(startY, height);
    position[11] = 0.0f;
}

float SelectionRectangle::convertCoordinates(int x, int w)
{
    return (float)x/(float)w * 2.0f - 1.0f;
}

SelectionRectangle::SelectionRectangle()
{
    vertexBuffer = 0;
    indicesBuffer = 0;

    startX = -1.0f;
    startY = 1.0f;
    endX = 1.0f;
    endY = -1.0f;

    /*
    position[0] = -1.0f;
    position[1] = 1.0f;
    position[2] = 0.0f;
    position[3] = -1.0f;
    position[4] = -1.0f;
    position[5] = 0.0f;
    position[6] = 1.0f;
    position[7] = -1.0f;
    position[8] = 0.0f;
    position[9] = 1.0f;
    position[10] = 1.0f;
    position[11] = 0.0f;
    */
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;

    shouldDraw = false;
}

void SelectionRectangle::setStartPosition(int x, int y)
{
    startX = x;
    startY = y;
    bindCoordinates();
    bindBufferData();

}

void SelectionRectangle::setEndPosition(int x, int y)
{
    endX = x;
    endY = y;
    bindCoordinates();
    bindBufferData();

}

void SelectionRectangle::setShouldDraw(bool shouldDraw)
{
    this->shouldDraw = shouldDraw;
}

bool SelectionRectangle::isShouldDraw()
{
    return shouldDraw;
}

void SelectionRectangle::setSize(int h, int w)
{
    height = h;
    width = w;
}

void SelectionRectangle::init(GLuint vaoHandle)
{
    this->vaoHandle = vaoHandle;


    shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\selectrect_vertex.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\selectrect_fragment.glsl");


    bindCoordinates();
    bindBufferData();


}

void SelectionRectangle::draw()
{
    shader->enable();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    shader->disable();

}

void SelectionRectangle::allowedDraw()
{
    if(shouldDraw)
        draw();

}




