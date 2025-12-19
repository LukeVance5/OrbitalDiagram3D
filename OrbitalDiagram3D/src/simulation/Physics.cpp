#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <iostream>
#include "body.h"
#include "Physics.h"
#include "Units.h"
std::vector<glm::vec3> Physics::CalculateForces(std::vector<std::shared_ptr<Body>>& bodies) {
	std::vector<glm::vec3> forces;
	if (bodies.empty()) return forces;

	// Initialize forces with zero vectors (avoid out-of-range writes)
	forces.assign(bodies.size(), glm::vec3(0.0f));

	for (size_t i = 0; i < bodies.size(); ++i) {
		for (size_t j = 0; j < bodies.size(); ++j) {
			if (i == j) continue;
			closerHill(bodies[i], bodies[j]); // Sees if j is new parent of i
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

void Physics::UpdateBodies(std::vector<std::shared_ptr<Body>>& bodies, float deltaTime) {
	if (bodies.empty()) return;
	std::vector<glm::vec3> forces = CalculateForces(bodies);
	// forces should be same size as bodies; guard just in case
	if (forces.size() != bodies.size()) return;
	for (size_t i = 0; i < bodies.size(); ++i) {
		glm::vec3 acceleration = forces[i] / bodies[i]->mass;
		bodies[i]->position += bodies[i]->velocity * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;
		bodies[i]->velocity += acceleration * deltaTime;
		try {
			bodies[i]->clearTrajectory(); // Clear previous trajectory
			if (bodies[i]->getParent()  != nullptr) {
				Trajectory::TrajectoryStruct traj = calculateTrajectory(*bodies[i], *bodies[i]->getParent());
				bodies[i]->setTrajectory(traj);
				if (bodies[i]->name == "earth") {
					int debug = 1;
					glm::vec3 test_pos = glm::vec3(0.0f);
					glm::vec3 test_vel = glm::vec3(0.0f);
					CalculateEllipticalState3D(
						traj.semiMajorAxis,
						traj.eccentricity,
						traj.inclination,
						traj.Omega,
						traj.omega,
						traj.v,
						Units::G * (bodies[i]->mass + bodies[i]->getParent()->mass),
						test_pos,
						test_vel
					);
					if (glm::length(test_pos - bodies[i]->position) >
						0.01f * glm::length(bodies[i]->position))
					{
						std::cout << "Position mismatch: calculated " << test_pos.x << ", " << test_pos.y << ", " << test_pos.z
						<< " vs actual " << bodies[i]->position.x << ", " << bodies[i]->position.y << ", " << bodies[i]->position.z << std::endl;
					}
				}
			}
		}
		catch (const std::exception& e) {
			// Handle exceptions if necessary (e.g., log the error)
		}
	}
}
// This function finds the object with the largest pull that has a larger mass than itself, to help draw object trajectory
void Physics::closerHill(std::shared_ptr<Body>& curr, std::shared_ptr<Body>& pParent) {
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

float Physics::Distance(std::shared_ptr<Body>& const body1, std::shared_ptr<Body>& const body2) {
	glm::vec3 direction = body2->position - body1->position;
	return glm::length(direction);
}

Trajectory::TrajectoryStruct Physics::calculateTrajectory(const Body& body, const Body& parent) {
	return Trajectory::calculateTrajectory(body, parent);
}

void Physics::CalculateEllipticalState3D(float a, float e, float i_rad, float omega_rad, float arg_rad, float nu_rad,float mu, glm::vec3 & r_out, glm::vec3 & v_out) {
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