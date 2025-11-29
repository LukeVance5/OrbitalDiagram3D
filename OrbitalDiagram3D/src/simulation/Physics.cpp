#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "Body/body.h"
#include "simulation/Physics.h"

const float G = 6.674e-29f;
std::vector<glm::vec3> Physics::CalculateForces(const std::vector<std::shared_ptr<Body>>& bodies) {
	std::vector<glm::vec3> forces;
	if (bodies.empty()) return forces;

	// Initialize forces with zero vectors (avoid out-of-range writes)
	forces.assign(bodies.size(), glm::vec3(0.0f));

	for (size_t i = 0; i < bodies.size(); ++i) {
		for (size_t j = 0; j < bodies.size(); ++j) {
			if (i == j) continue;
			glm::vec3 direction = bodies[j]->position - bodies[i]->position;
			float distance = glm::length(direction);
			if (distance <= 0.0f) continue; // avoid division by zero or negative
			float forceMagnitude = (float) ((double)G) * (((double)bodies[i]->mass) * ((double)bodies[j]->mass)) / (((double)distance) * ((double)distance));
			glm::vec3 forceVector = glm::normalize(direction) * forceMagnitude;
			forces[i] += forceVector;
		}
	}
	return forces;
}

void Physics::UpdateBodies(std::vector<std::shared_ptr<Body>>& bodies, float deltaTime) {
	if (bodies.empty()) return;
	std::vector<glm::vec3> forces = CalculateForces(bodies);
	// forces should be same size as bodies; guard just in case
	if (forces.size() != bodies.size()) return;
	for (size_t i = 0; i < bodies.size(); ++i) {
		glm::vec3 acceleration = forces[i] / bodies[i]->mass;
		bodies[i]->position += bodies[i]->velocity * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;
		bodies[i]->velocity += acceleration * deltaTime;
	}
}