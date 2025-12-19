#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OrbitalDiagram3DExport.h"

class OD_API Shader {
public:
	unsigned int id;
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	Shader(const char* vertexShaderPath, const char* geometryShaderPath, const char* fragmentShaderPath);
	void activate();

	// utils
	std::string loadShaderSrc(const char* filepath);
	GLuint compileShader(const char* filepath, GLenum type);

	// uniform functions
	void setMat4(const std::string& name, glm::mat4 val);

	void setInt(const std::string& name, int value);
	void set2Float(const std::string& name, float v1, float v2);
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4);

	void setFloat(const std::string& name, float value);
};

#endif