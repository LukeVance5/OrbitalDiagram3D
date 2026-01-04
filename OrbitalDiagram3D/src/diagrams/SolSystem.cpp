#include "SolSystem.h"
#include "Sphere.h"
#include "Body.h"
#include "Shader.h"
#include "TextureLoader.h"
#include "Units.h"

SolSystem::SolSystem() : engine() {
	this->sim = new Simulation();
	unsigned int sunTextureID = TextureLoader::LoadTexture("assets/images/sun.jpg");
	std::shared_ptr<Body> sun = Body::create("sun", 696.34f, SUN_MASS, glm::vec3(0, 0, 0), glm::vec3(0), body, sunTextureID);
	this->sim->addObject(sun);
	this->addInnerPlanets();
}

Simulation* SolSystem::getSimulation() {
	return this->sim;
}

void SolSystem::addInnerPlanets() {
	addMercury();
	addVenus();
	addEarthMoonSystem();
	addMars();
}


void SolSystem::addMercury() {
	unsigned int mercuryTextureID = TextureLoader::LoadTexture("assets/images/mercury.jpg");



	glm::vec3 mercuryVelocity = glm::vec3(0);
	glm::vec3 mercuryPosition = glm::vec3(0);

	float mu = Units::G* (MERCURY_MASS + SUN_MASS);
	engine.CalculateEllipticalState3D(
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


	std::shared_ptr<Body> mercury = Body::create("mercury", 2.4397f, MERCURY_MASS, mercuryPosition, mercuryVelocity, body, mercuryTextureID);
	mercury->orbitColor = glm::vec3(0.85f, 0.85f, 0.85f);
	this->sim->addObject(mercury);
}

void SolSystem::addVenus() {
	unsigned int venusTextureID = TextureLoader::LoadTexture("assets/images/venus.jpg");
	glm::vec3 venusVelocity = glm::vec3(0);
	glm::vec3 venusPosition = glm::vec3(0);
	float mu = Units::G * (SUN_MASS + VENUS_MASS);
	engine.CalculateEllipticalState3D(
		108208.0f,
		0.0067f,
		glm::radians(3.3947f),
		glm::radians(76.680f),
		glm::radians(54.884f),
		glm::radians(40.0f),
		mu,
		venusPosition,
		venusVelocity
	);
	std::shared_ptr<Body> venus = Body::create("venus", 6.0518f, VENUS_MASS, venusPosition, venusVelocity, body, venusTextureID);
	venus->orbitColor = glm::vec3(1.00f, 0.38f, 0.20f);
	this->sim->addObject(venus);
}

void SolSystem::addMars() {
	unsigned int marsTextureID = TextureLoader::LoadTexture("assets/images/mars.jpg");
	glm::vec3 marsVelocity = glm::vec3(0);
	glm::vec3 marsPosition = glm::vec3(0);
	float mu = Units::G * (SUN_MASS + MARS_MASS);
	engine.CalculateEllipticalState3D(
		227939.0f,
		0.0934f,
		glm::radians(1.850f),
		glm::radians(49.578f),
		glm::radians(286.502f),
		glm::radians(0.0f),
		mu,
		marsPosition,
		marsVelocity
	);
	std::shared_ptr<Body> mars = Body::create("mars", 3.3895f, MARS_MASS, marsPosition, marsVelocity, body, marsTextureID);
	mars->orbitColor = glm::vec3(0.78f, 0.32f, 0.20f);
	this->sim->addObject(mars);
}

void SolSystem::addEarthMoonSystem() {
	unsigned int moonTextureID = TextureLoader::LoadTexture("assets/images/moon.jpg");
	float mu = Units::G * (EARTH_MASS + MOON_MASS);
	unsigned int earthTextureID = TextureLoader::LoadTexture("assets/images/earth.jpg");
	glm::vec3 moonVelocity = glm::vec3(0);
	glm::vec3 moonPosition = glm::vec3(0);
	engine.CalculateEllipticalState3D(
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
	std::shared_ptr<Body> earth = Body::create("earth", 6.378f, EARTH_MASS, glm::vec3(149597.0f, 0, 0), glm::vec3(0, 0, 0.02987f), body, earthTextureID);
	std::shared_ptr<Body> moon = Body::create("moon", 3.734f, MOON_MASS, moonPosition, moonVelocity, body, earth, moonTextureID);
	earth->orbitColor = glm::vec3(0.1f, 0.25f, 0.6f);
	moon->orbitColor = glm::vec3(0.98f, 0.95f, 0.88f);
	this->sim->addObject(earth);
	this->sim->addObject(moon);
}

