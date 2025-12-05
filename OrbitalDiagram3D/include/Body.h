#pragma once
#include <string>
#include <memory>
#include "OrbitalDiagram3DExport.h"
#include <glm/glm.hpp>
#ifndef BODY_H
#define BODY_H
class OD_API Body{
	public:
		std::string type;
		std::string name;
		float mass;
		float radius;
		glm::vec3 position;
		glm::vec3 velocity;
		unsigned int textureID = 0;
		std::shared_ptr<Body> parent;
		Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::string type);
		Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::string type, unsigned int textureID);
};
#endif 
