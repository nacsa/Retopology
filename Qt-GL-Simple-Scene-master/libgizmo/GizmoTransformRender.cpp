///////////////////////////////////////////////////////////////////////////////////////////////////
// LibGizmo
// File Name :
// Creation : 10/01/2012
// Author : Cedric Guillemet
// Description : LibGizmo
//
///Copyright (C) 2012 Cedric Guillemet
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//of the Software, and to permit persons to whom the Software is furnished to do
///so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef NDEBUG
#include <stdio.h>
#endif

#include "stdafx.h"
#include "GizmoTransformRender.h"

/*
#ifdef GIZMO_ENABLE_QT
#define gizmo_ogl_LoadFunctions()
#endif
*/
static void AttachShader(const char *source, GLenum type, GLuint program)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
//#ifndef NDEBUG
 //   {
        GLint compiled, len;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
            if (len > 0) {
                char *message = new char[len];
                glGetShaderInfoLog(shader, len, &len, message);
                fprintf(stderr, "shader: %s", message);
                delete[] message;
            }
        }
 //   }
//#endif
    glAttachShader(program, shader);
    glDeleteShader(shader);
}

CGizmoTransformRender::CGizmoTransformRender()
    : m_Program(0),
      m_ColorUniform(0),
      m_ModelviewMatrixUniform(0),
      m_ProjectionMatrixUniform(0)
{
}

CGizmoTransformRender::~CGizmoTransformRender()
{
    m_ColorUniform = 0;
    m_ModelviewMatrixUniform = 0;
    m_ProjectionMatrixUniform = 0;
    if (m_Program) {
        glDeleteProgram(m_Program);
        m_Program = 0;
    }
}

void CGizmoTransformRender::Initialize()
{
    if (!m_Program) {
        //gizmo_ogl_LoadFunctions();
        m_Program = glCreateProgram();
        static const char vertexShader[] = ""
                "#if __VERSION__ < 130\n"
                "#define in attribute\n"
                "#define out varying\n"
                "#endif\n"
                "uniform mat4 modelViewMatrix;\n"
                "uniform mat4 projectionMatrix;\n"
                "uniform vec4 color;\n"
                "in vec3 inPosition;\n"
                "out vec4 outColor;\n"
                "void main() {\n"
                "  outColor = color;\n"
                "  gl_Position = (projectionMatrix * modelViewMatrix) * vec4(inPosition, 1.0);\n"
                "}\n";
        AttachShader(vertexShader, GL_VERTEX_SHADER, m_Program);
        static const char fragmentShader[] = ""
                "#if __VERSION__ < 130\n"
                "#define in varying\n"
                "#define outPixelColor gl_FragColor\n"
                "#else\n"
                "out vec4 outPixelColor;\n"
                "#endif\n"
                "in vec4 outColor;\n"
                "void main() {\n"
                "  outPixelColor = outColor;\n"
                "}\n"
                ;
        AttachShader(fragmentShader, GL_FRAGMENT_SHADER, m_Program);
        glBindAttribLocation(m_Program, 0, "inPosition");
        glLinkProgram(m_Program);
//#ifndef NDEBUG
      //  {
            GLint compiled, len;
            glGetProgramiv(m_Program, GL_LINK_STATUS, &compiled);
            if (!compiled) {
                glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &len);
                if (len > 0) {
                    char *message = new char[len];
                    glGetProgramInfoLog(m_Program, len, &len, message);
                    fprintf(stderr, "shader: %s", message);
                    delete[] message;
                }
            }
    //    }
//#endif
        m_ColorUniform = glGetUniformLocation(m_Program, "color");
        m_ModelviewMatrixUniform = glGetUniformLocation(m_Program, "modelViewMatrix");
        m_ProjectionMatrixUniform = glGetUniformLocation(m_Program, "projectionMatrix");

        glGenVertexArrays(1, vaoHandle);
    }
}

void CGizmoTransformRender::DrawCircle(const tvector3 &orig, const tvector3 &color, const tvector3 &vtx, const tvector3 &vty)
{
    static const int size = 50;
    tvector3 vertices[size];
    for (int i = 0; i < size; i++) {
        tvector3 vt;
        vt  = vtx * cos((2 * ZPI / size) * i);
        vt += vty * sin((2 * ZPI / size) * i);
        vt += orig;
        vertices[i] = vt;
    }
    glDisable(GL_DEPTH_TEST);
    ActivateProgram();
    glUniform4f(m_ColorUniform, color.x, color.y, color.z, 1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), vertices);
    glDrawArrays(GL_LINE_LOOP, 0, size);
    DeactivateProgram();
    glEnable(GL_DEPTH_TEST);
}


void CGizmoTransformRender::DrawCircleHalf(const tvector3 &orig, const tvector3 &color, const tvector3 &vtx, const tvector3 &vty, tplane &camPlan)
{
    static const int size = 30;
    tvector3 vertices[size];
    int j = 0;
    for (int i = 0; i < size; i++) {
        tvector3 vt;
        vt  = vtx * cos((ZPI / size) * i);
        vt += vty * sin((ZPI / size) * i);
        vt += orig;
        if (camPlan.DotNormal(vt)) {
            vertices[j++] = vt;
        }
    }
    glDisable(GL_DEPTH_TEST);
    ActivateProgram();
    glUniform4f(m_ColorUniform, color.x, color.y, color.z, 1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), vertices);
    glDrawArrays(GL_LINE_STRIP, 0, j);
    DeactivateProgram();
    glEnable(GL_DEPTH_TEST);
}

void CGizmoTransformRender::DrawAxis(const tvector3 &orig, const tvector3 &axis, const tvector3 &vtx, const tvector3 &vty, float fct, float fct2, const tvector3 &col)
{
    tvector3 vertices1[2] = { orig, orig + axis };
    tvector3 vertices2[62];
    int j = 0;
    for (int i = 0; i <= 30; i++) {
        tvector3 pt;
        pt  = vtx * cos(((2 * ZPI) / 30.0f) * i) * fct;
        pt += vty * sin(((2 * ZPI) / 30.0f) * i) * fct;
        pt += axis * fct2;
        pt += orig;
        pt  = vtx * cos(((2 * ZPI) / 30.0f) * (i + 1)) * fct;
        pt += vty * sin(((2 * ZPI) / 30.0f) * (i + 1)) * fct;
        pt += axis * fct2;
        pt += orig;
        vertices2[j++] = pt;
        vertices2[j++] = orig + axis;
    }
    glDisable(GL_DEPTH_TEST);
    ActivateProgram();
    glUniform4f(m_ColorUniform, col.x, col.y, col.z, 1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices1[0]), vertices1);
    glDrawArrays(GL_LINES, 0, 2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices2[0]), vertices2);
    glDrawArrays(GL_TRIANGLE_FAN, 0, j);
    DeactivateProgram();
    glEnable(GL_DEPTH_TEST);
}

void CGizmoTransformRender::DrawCamem(const tvector3 &orig, const tvector3 &vtx, const tvector3 &vty, float ng)
{
    tvector3 vertices[52];
    int j = 0;
    vertices[j++] = orig;
    for (int i = 0 ; i <= 50 ; i++) {
        tvector3 vt;
        vt = vtx * cos((ng / 50) * i);
        vt += vty * sin((ng / 50) * i);
        vt += orig;
        vertices[j++] = vt;
    }
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    ActivateProgram();
    glUniform4f(m_ColorUniform, 0xf0 / 255.0, 0x12 / 255.0, 0xbe / 255.0, 0.5);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, j);
    j = 0;
    vertices[j++] = &orig.x;
    for (int i = 0 ; i <= 50 ; i++) {
        tvector3 vt;
        vt  = vtx * cos(((ng)/50)*i);
        vt += vty * sin(((ng)/50)*i);
        vt += orig;
        vertices[j++] = vt;
    }
    glDisable(GL_BLEND);
    glUniform4f(m_Program, 1, 1, 0.2, 1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), vertices);
    glDrawArrays(GL_LINE_LOOP, 0, j);
    DeactivateProgram();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
}

void CGizmoTransformRender::DrawQuad(const tvector3 &orig, float size, bool bSelected, const tvector3 &axisU, const tvector3 &axisV)
{
    tvector3 pts[4];
    pts[0] = orig;
    pts[1] = orig + (axisU * size);
    pts[2] = orig + (axisU + axisV)*size;
    pts[3] = orig + (axisV * size);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    ActivateProgram();
    if (!bSelected) {
        glUniform4f(m_ColorUniform, 1, 1, 0, 0.5f);
    }
    else {
        glUniform4f(m_ColorUniform, 1, 1, 1, 0.6f);
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pts[0]), pts);
    glDrawArrays(GL_QUADS, 0, sizeof(pts) / sizeof(pts[0]));
    if (!bSelected) {
        glUniform4f(m_ColorUniform, 1, 1, 0.2f, 1);
    }
    else {
        glUniform4f(m_ColorUniform, 1, 1, 1, 0.6f);
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pts[0]), pts);
    glDrawArrays(GL_LINE_STRIP, 0, sizeof(pts) / sizeof(pts[0]));
    DeactivateProgram();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}


void CGizmoTransformRender::DrawTri(const tvector3 &orig, float size, bool bSelected, const tvector3 &axisU, const tvector3 &axisV)
{
    tvector3 pts[3];
    pts[0] = orig;
    pts[1] = axisU;
    pts[2] = axisV;
    pts[1] *= size;
    pts[2] *= size;
    pts[1] += orig;
    pts[2] += orig;
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    ActivateProgram();
    if (!bSelected) {
        glUniform4f(m_ColorUniform, 1, 1, 0, 0.5f);
    }
    else {
        glUniform4f(m_ColorUniform, 1, 1, 1, 0.6f);
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pts[0]), pts);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(pts) / sizeof(pts[0]));
    if (!bSelected) {
        glUniform4f(m_ColorUniform, 1, 1, 0.2f, 1);
    }
    else {
        glUniform4f(m_ColorUniform, 1, 1, 1, 0.6f);
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pts[0]), pts);
    glDrawArrays(GL_LINE_STRIP, 0, sizeof(pts) / sizeof(pts[0]));
    DeactivateProgram();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void CGizmoTransformRender::ActivateProgram()
{
    glUseProgram(m_Program);
    glUniformMatrix4fv(m_ModelviewMatrixUniform, 1, GL_FALSE, m_Model);
    glUniformMatrix4fv(m_ProjectionMatrixUniform, 1, GL_FALSE, m_Proj);

    glEnableVertexAttribArray(0);
}

void CGizmoTransformRender::DeactivateProgram()
{
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDisableVertexAttribArray(0);
    glUseProgram(0);
}
