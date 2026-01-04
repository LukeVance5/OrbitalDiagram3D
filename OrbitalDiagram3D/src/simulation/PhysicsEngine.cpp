#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <iostream>
#include "body.h"
#include "PhysicsEngine.h"
#include "Units.h"

PhysicsEngine::PhysicsEngine() {
}
std::vector<glm::vec3> PhysicsEngine::calculateForces(std::vector<std::shared_ptr<Body>>& bodies) {
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
			float forceMagnitude = (float) ((double) Units::G) * (((double)bodies[i]->mass) * ((double)bodies[j]->mass)) / (((double)distance) * ((double)distance));
			glm::vec3 forceVector = glm::normalize(direction) * forceMagnitude;
			forces[i] += forceVector;
		}
	}
	return forces;
}
std::vector<glm::vec3> PhysicsEngine::calculateForces(std::vector<std::shared_ptr<Body>>& bodies, std::vector<glm::vec3> positions) {
	std::vector<glm::vec3> forces;
	if (bodies.empty()) return forces;

	// Initialize forces with zero vectors (avoid out-of-range writes)
	forces.assign(bodies.size(), glm::vec3(0.0f));

	for (size_t i = 0; i < bodies.size(); ++i) {
		for (size_t j = 0; j < bodies.size(); ++j) {
			if (i == j) continue;
			glm::vec3 direction = positions[j] - positions[i];
			float distance = glm::length(direction);
			if (distance <= 0.0f) continue; // avoid division by zero or negative
			float forceMagnitude = (float)((double)Units::G) * (((double)bodies[i]->mass) * ((double)bodies[j]->mass)) / (((double)distance) * ((double)distance));
			glm::vec3 forceVector = glm::normalize(direction) * forceMagnitude;
			forces[i] += forceVector;
		}
	}
	return forces;
}
void PhysicsEngine::UpdateBodies(std::vector<std::shared_ptr<Body>>& bodies, float deltaTime) {
	if (bodies.empty()) return;
	updateVectors(bodies, deltaTime);
	updateTrajectories(bodies);
	for (size_t i = 0; i < bodies.size(); ++i) {
		for (size_t j = 0; j < bodies.size(); ++j) {
			if (i == j) continue;
			closerHill(bodies[i], bodies[j]);
		}
	}
}

void PhysicsEngine::updateVectors(std::vector<std::shared_ptr<Body>>& bodies, float dt) {
	size_t n = bodies.size();
	if (n == 0) return;

	// Cache state
	std::vector<glm::vec3> positions(n);
	std::vector<glm::vec3> velocities(n);

	for (size_t i = 0; i < n; ++i) {
		positions[i] = bodies[i]->position;
		velocities[i] = bodies[i]->velocity;
	}

	// aₙ
	auto forces = calculateForces(bodies);
	std::vector<glm::vec3> accel(n);

	for (size_t i = 0; i < n; ++i)
		accel[i] = forces[i] / bodies[i]->mass;

	// xₙ₊₁
	std::vector<glm::vec3> newPositions(n);
	for (size_t i = 0; i < n; ++i)
		newPositions[i] =
		positions[i]
		+ velocities[i] * dt
		+ 0.5f * accel[i] * dt * dt;

	// aₙ₊₁
	auto newForces = calculateForces(bodies, newPositions);
	std::vector<glm::vec3> newAccel(n);

	for (size_t i = 0; i < n; ++i)
		newAccel[i] = newForces[i] / bodies[i]->mass;

	// vₙ₊₁
	for (size_t i = 0; i < n; ++i)
		velocities[i] += 0.5f * (accel[i] + newAccel[i]) * dt;

	// Commit
	for (size_t i = 0; i < n; ++i) {
		bodies[i]->position = newPositions[i];
		bodies[i]->velocity = velocities[i];
	}
}

void PhysicsEngine::updateTrajectories(std::vector<std::shared_ptr<Body>>& bodies) {
	for (auto& body : bodies) {
		try {
			body->clearTrajectory(); // Clear previous trajectory
			if (body->getParent() != nullptr) {
				Trajectory::TrajectoryStruct traj = this->calculateTrajectory(*body, *body->getParent());
				body->setTrajectory(traj);
				}
		}
		catch (const std::exception& e) {
			// Handle exceptions if necessary (e.g., log the error)
		}
	}
}
// This function finds the object with the largest pull that has a larger mass than itself, to help draw object trajectory
void PhysicsEngine::closerHill(std::shared_ptr<Body>& curr, std::shared_ptr<Body>& pParent) {
	if (curr->mass > pParent->mass) {
		return;
	}
	else if (curr->getParent() == nullptr) {
		curr->setParent(pParent);
		return;
	}
	std::shared_ptr<Body> currParent = curr->getParent();

	float currHill = Distance(curr, currParent) * glm::pow((curr->mass / (3.0f * currParent->mass)), 1.0f / 3.0f);
	float newHill = Distance(curr, pParent) * glm::pow((curr->mass / (3.0f * pParent->mass)), 1.0f / 3.0f);

	if (newHill < currHill) {
		if (curr->name == "moon" && pParent->name == "sun") {
			int debug = 1;
		}
		curr->setParent(pParent);
	}
	return;
}

float PhysicsEngine::Distance(std::shared_ptr<Body>& const body1, std::shared_ptr<Body>& const body2) {
	glm::vec3 direction = body2->position - body1->position;
	return glm::length(direction);
}

Trajectory::TrajectoryStruct PhysicsEngine::calculateTrajectory(const Body& body, const Body& parent) {
	return Trajectory::calculateTrajectory(body, parent);
}

void PhysicsEngine::CalculateEllipticalState3D(float a, float e, float i_rad, float omega_rad, float arg_rad, float nu_rad,float mu, glm::vec3 & r_out, glm::vec3 & v_out) {
	// Shared constants from fixture

	// 1. R/V in P-Q (X-Z) frame - UNCHANGED
	float p = a * (1.0f - e * e);
	float r_mag = p / (1.0f + e * glm::cos(nu_rad));
	float C = glm::sqrt(mu / p);

	glm::vec3 r_orbital = glm::vec3(r_mag * glm::sin(nu_rad), 0.0f, r_mag * glm::cos(nu_rad));
	glm::vec3 v_orbital = glm::vec3(C * (e + glm::cos(nu_rad)), 0.0f, -C * glm::sin(nu_rad));

	glm::mat3 R1 = glm::transpose(glm::mat3(
		cos(arg_rad), 0.0f, sin(arg_rad),
		0.0f, 1.0f, 0.0f,
		-sin(arg_rad), 0.0f, cos(arg_rad))
	);

	glm::mat3 R2 = glm::transpose(glm::mat3(
		cos(i_rad), -sin(i_rad), 0.0f,
		sin(i_rad), cos(i_rad), 0.0f,
		0.0f, 0.0f, 1.0f
	));

	glm::mat3 R3 = glm::transpose(glm::mat3(
		cos(omega_rad), 0.0f, sin(omega_rad),
		0.0f, 1.0f, 0.0f,
		-sin(omega_rad), 0.0f, cos(omega_rad)
	));

	glm::mat3 Rotation = R3 * R2 * R1;

	// 3. Final Inertial R/V
	r_out = Rotation * r_orbital;
	v_out = Rotation * v_orbital;
}