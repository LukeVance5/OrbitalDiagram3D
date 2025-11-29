#pragma once
#include <string>
#include <memory>
#include "mesh/Mesh.h"
#include "rendering/Shader.h"
#ifndef BODY_H
#define BODY_H
class Body{
	public:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Mesh> mesh;
		std::string name;
		float mass;
		float radius;
		glm::vec3 position;
		glm::vec3 velocity;
		unsigned int textureID = 0;
		Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader);
		Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::shared_ptr<Mesh> mesh,std::shared_ptr<Shader> shader, unsigned int textureID);
};
#endif 
