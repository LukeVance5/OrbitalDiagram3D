#include "Body.h"

Body::Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::string type) {
	this->name = name;
	this->radius = radius;
	this->mass = mass;
	this->type = type;
	this->position = position;
	this->velocity = velocity;
}


Body::Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::string type, unsigned int textureID) {
	this->name = name;
	this->radius = radius;
	this->mass = mass;
	this->type = type;
	this->position = position;
	this->velocity = velocity;
	this->textureID = textureID;
}
