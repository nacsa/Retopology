/*
 *
 * Copyright © 2010-2011 Balázs Tóth <tbalazs@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef _SHADER_
#define _SHADER_

#include <string>

#include <glm/glm.hpp>
#include <GL/gl3w.h>
#include <qbytearray.h>

using glm::mat4;
using glm::mat3;

class Shader{
private:
  GLuint shaderProgram;
  GLuint vertexProgram;
  GLuint fragmentProgram;
  GLuint geometryProgram;

  bool fileToString(const char* path, char* &out, int& len);
  bool qfileToString(const char* path, QByteArray *blob, int& len);

public:
  Shader();
  Shader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = 0);
  void load(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = 0);
  virtual ~Shader();

  GLuint getProgram(){return shaderProgram;}
  GLuint getGeometryProgram(){return geometryProgram;}

  void shaderFromQFile(const char* shaderPath, GLenum shaderType, GLuint& handle);
  void shaderFromFile(const char* shaderPath, GLenum shaderType, GLuint& handle);
  void linkShaders(GLuint& vertexShader, GLuint& fragmentShader, GLuint& geometryShader, GLuint& handle);

  std::string getShaderInfoLog(GLuint& object);
  std::string getProgramInfoLog(GLuint& object);

  void enable();
  void disable();

  GLuint getHandle(){
    return shaderProgram;
  }

  void bindUniformBool(const char* name, bool b);
  void bindUniformInt(const char* name, int i);
  void bindUniformInt2(const char* name, int i1, int i2);
  void bindUniformFloat(const char* name, float f);
  void bindUniformFloat2(const char* name, float f1, float f2);
  void bindUniformFloat3(const char* name, float f1, float f2, float f3);
  void bindUniformTexture(const char* name, GLuint texture, GLuint unit);


  void bindUniformMatrix3(const char* name, float* m, unsigned int arraySize = 1);
  void bindUniformMatrix(const char* name, float* m, unsigned int arraySize = 1);
  void bindUniformVector(const char* name, float* m, unsigned int arraySize = 1);
  void bindUniformFloat4Array(const char* name, float* m, unsigned int arraySize = 1);
  void bindUniformIntArray(const char* name, int* iv, unsigned int arraySize);
  void bindUniformMatrixMat4( const char *name, const mat4 & m);
  void bindUniformMatrixMat3( const char *name, const mat3 & m);

  void bindAttribLocation(GLuint id, const char* name);
};

#endif
