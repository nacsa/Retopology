#ifndef SELECTIONRECTANGLE_H
#define SELECTIONRECTANGLE_H

#include <GL/gl3w.h>

#include "shader.hpp"

class SelectionRectangle
{


    bool shouldDraw;

    int height;
    int width;

    Shader* shader;
    GLuint vaoHandle;

    GLuint vertexBuffer;
    GLuint indicesBuffer;


    float position[12];


    unsigned int indices[6];



    void bindBufferData();
    void bindCoordinates();
    float convertCoordinates(int x, int width);

public:

    int startX;
    int startY;
    int endX;
    int endY;


    SelectionRectangle();

    void setStartPosition(int x, int y);
    void setEndPosition(int x, int y);

    void setShouldDraw(bool shouldDraw);
    bool isShouldDraw();

    void setSize(int h, int w);

    void init(GLuint vaoHandle);

    void draw();

    void allowedDraw();


};

#endif // SELECTIONRECTANGLE_H
