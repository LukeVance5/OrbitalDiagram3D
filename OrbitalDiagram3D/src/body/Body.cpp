#include "Body.h"

Body::Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader) {
	this->name = name;
	this->radius = radius;
	this->mass = mass;
	this->mesh = mesh;
	this->shader = shader;
	this->position = position;
	this->velocity = velocity;
}


Body::Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, unsigned int textureID) {
	this->name = name;
	this->radius = radius;
	this->mass = mass;
	this->mesh = mesh;
	this->shader = shader;
	this->position = position;
	this->velocity = velocity;
	this->textureID = textureID;
}
