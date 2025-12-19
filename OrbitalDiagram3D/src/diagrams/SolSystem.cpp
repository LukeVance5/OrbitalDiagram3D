#include "SolSystem.h"
#include "Sphere.h"
#include "Body.h"
#include "Shader.h"
#include "TextureLoader.h"
#include "Physics.h"
#include "Units.h"

SolSystem::SolSystem() {
	this->sim = new Simulation();
	addInnerPlanets();
}

Simulation* SolSystem::getSimulation() {
	return this->sim;
}

void SolSystem::addInnerPlanets() {
	unsigned int moonTextureID = TextureLoader::LoadTexture("assets/images/moon.jpg");
	unsigned int sunTextureID = TextureLoader::LoadTexture("assets/images/sun.jpg");
	unsigned int earthTextureID = TextureLoader::LoadTexture("assets/images/earth.jpg");
	std::string type = "body";
	std::shared_ptr<Body> body1 = Body::create("sun", 696.34f, SUN_MASS, glm::vec3(0, 0, 0), glm::vec3(0), type, sunTextureID);
	std::shared_ptr<Body> body2 = Body::create("earth", 6.378f, EARTH_MASS, glm::vec3(149597.0f, 0, 0), glm::vec3(0, 0, 0.02987f), type, earthTextureID);
	
	glm::vec3 mercuryVelocity = glm::vec3(0);
	glm::vec3 mercuryPosition = glm::vec3(0);
	glm::vec3 moonVelocity = glm::vec3(0);
	glm::vec3 moonPosition = glm::vec3(0);
	float mu = Units::G * body1->mass;
	Physics::CalculateEllipticalState3D(
		57910.0f,
		0.2056f,
		glm::radians(7.005f),
		glm::radians(48.331f),
		glm::radians(29.124f),
		glm::radians(0.0f),
		mu,
		mercuryPosition,
		mercuryVelocity
	);
	mu = Units::G * (EARTH_MASS + MOON_MASS);
	Physics::CalculateEllipticalState3D(
		384.748f,
		0.054f,
		glm::radians(5.15f),
		glm::radians(0.0f),
		glm::radians(0.0f),
		glm::radians(0.0f),
		mu,
		moonPosition,
		moonVelocity
	);
	std::shared_ptr<Body> body3 = Body::create("moon", 10.734f, MOON_MASS, moonPosition, moonVelocity, type, body2, moonTextureID);
	std::shared_ptr<Body> mercury = Body::create("mercury", 2.4397f, 3.3011e23f, mercuryPosition, mercuryVelocity, type, moonTextureID);
	this->sim->addObject(body1);
	this->sim->addObject(mercury);
	this->sim->addObject(body2);
	this->sim->addObject(body3);
}