
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
protected:
	GLuint shaderProgram;

	bool fileToString(const char* path, char*& out, int& len) {
		std::ifstream file(path, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			return false;
		}
		len = (int)file.tellg();
		out = new char[len + 1];
		file.seekg(0, std::ios::beg);
		file.read(out, len);
		file.close();
		out[len] = 0;
		return true;
	}

	void shaderFromFile(const char* shaderPath, GLenum shaderType, GLuint& handle) {
		char* shaderSource = NULL;
		int len = 0;
		int errorFlag = -1;

		if (!fileToString(shaderPath, shaderSource, len)) {
			std::cout << "Error loading shader: " << shaderPath << std::endl;
			return;
		}

		handle = glCreateShader(shaderType);

		glShaderSource(handle, 1, (const char**)&shaderSource, &len);
		glCompileShader(handle);
		delete[] shaderSource;

		glGetShaderiv(handle, GL_COMPILE_STATUS, &errorFlag);
		if (!errorFlag) {
			std::cout << "Shader compile error: " << shaderPath << std::endl;
			std::cout << getShaderInfoLog(handle) << std::endl;
			return;
		}
	}

	virtual void linkShaders(GLuint& vertexShader, GLuint& fragmentShader, GLsizei attribs_size, const GLchar** attribs,
		GLuint& geometryShader, GLsizei feedback_varyings_size, const GLchar** feedback_varyings, GLuint & handle) {
		int errorFlag = -1;

		handle = glCreateProgram();
		glAttachShader(handle, vertexShader);
		glAttachShader(handle, fragmentShader);
		if (geometryShader != 0) {
			glAttachShader(handle, geometryShader);
			glProgramParameteriEXT(handle, GL_GEOMETRY_VERTICES_OUT_EXT, 4);
			glProgramParameteriEXT(handle, GL_GEOMETRY_INPUT_TYPE_EXT, GL_LINES_ADJACENCY_EXT);
			glProgramParameteriEXT(handle, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);

			const char* varname = "gl_Position";
			glTransformFeedbackVaryings(handle, 1, &varname, GL_SEPARATE_ATTRIBS);
		}
		for (int i = 0; i < attribs_size; i++) {
			glBindAttribLocation(handle, i, attribs[i]);
		}
		if (feedback_varyings_size != 0) {
			glTransformFeedbackVaryings(handle, feedback_varyings_size, feedback_varyings, GL_SEPARATE_ATTRIBS);
		}
		glLinkProgram(handle);
		glGetProgramiv(handle, GL_LINK_STATUS, &errorFlag);
		if (!errorFlag) {
			std::cout << "Shader link error: " << std::endl;
			std::cout << getProgramInfoLog(handle) << std::endl;
		}
	}

public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath, GLsizei attribs_size, const GLchar** attribs,
		const char* geometryShaderPath = 0, GLsizei feedback_varyings_size = 0, const GLchar** feedback_varyings = 0) {
		GLuint vertexProgram, fragmentProgram, geometryProgram = 0;

		shaderFromFile(vertexShaderPath, GL_VERTEX_SHADER, vertexProgram);
		shaderFromFile(fragmentShaderPath, GL_FRAGMENT_SHADER, fragmentProgram);
		if (geometryShaderPath) {
			shaderFromFile(geometryShaderPath, GL_GEOMETRY_SHADER_EXT, geometryProgram);
		}
		linkShaders(vertexProgram, fragmentProgram, attribs_size, attribs, geometryProgram,
			feedback_varyings_size, feedback_varyings, shaderProgram);
		// Can be freed after linking
		glDeleteShader(vertexProgram);
		glDeleteShader(fragmentProgram);
		if (geometryShaderPath) {
			glDeleteShader(geometryProgram);
		}
	}

	~Shader() {
		glDeleteProgram(shaderProgram);
	}
	
	GLuint getProgram() {
		return shaderProgram;
	}	

	std::string getShaderInfoLog(GLuint& object) {
		int logLength = 0;
		int charsWritten = 0;
		char* tmpLog;
		std::string log;

		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			tmpLog = new char[logLength];
			glGetShaderInfoLog(object, logLength, &charsWritten, tmpLog);
			log = tmpLog;
			delete[] tmpLog;
		}
		return log;
	}

	std::string getProgramInfoLog(GLuint& object) {
		int logLength = 0;
		int charsWritten  = 0;
		char *tmpLog;
		std::string log;
		
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			tmpLog = new char[logLength];
			glGetProgramInfoLog(object, logLength, &charsWritten, tmpLog);
			log = tmpLog;
			delete[] tmpLog;
		}
		return log;
	}

	void enable() {
		glUseProgram(shaderProgram);
	}
	
	void disable() {
		glUseProgram(NULL);
	}

	void bindUniformInt(const char* name, int i) {
		GLuint vectorLocation = glGetUniformLocation(shaderProgram, name);
		glUniform1i(vectorLocation, i);
	}

	void bindUniformInt2(const char* name, int i1, int i2) {
		GLuint vectorLocation = glGetUniformLocation(shaderProgram, name);
		glUniform2i(vectorLocation, i1, i2);
	}

	void bindUniformFloat(const char* name, float f) {
		GLuint location = glGetUniformLocation(shaderProgram, name);
		glUniform1f(location, f);
	}

	void bindUniformFloat2(const char* name, float f1, float f2) {
		GLuint location = glGetUniformLocation(shaderProgram, name);
		glUniform2f(location, f1, f2);
	}

	void bindUniformFloat3(const char* name, float f1, float f2, float f3) {
		GLuint location = glGetUniformLocation(shaderProgram, name);
		glUniform3f(location, f1, f2, f3);
	}

	void bindUniformFloat4v(const char* name, float *f) {
		GLuint location = glGetUniformLocation(shaderProgram, name);
		glUniform4fv(location, 1, f);  
	}

	void bindUniformTexture(const char* name, GLuint texture, GLuint unit) {
		GLuint location = glGetUniformLocation(shaderProgram, name);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(location, unit);
	}

	void bindAttribLocation(GLuint id, const char* name) {
		glEnableVertexAttribArray(id);
		glBindAttribLocation(shaderProgram, id, name);
	}

	GLint getAttribLocation(const char* name) {
		return glGetAttribLocation(shaderProgram, name);
	}

	void bindUniformMat3(const char* name, const float* m) {
		GLuint location = glGetUniformLocation(shaderProgram, name);
		glUniformMatrix3fv(location, 1, false, m);
	}

	void bindUniformMat4(const char* name, const float* m) {
		GLuint location = glGetUniformLocation(shaderProgram, name);
		glUniformMatrix4fv(location, 1, false, m);
	}
};

#endif
