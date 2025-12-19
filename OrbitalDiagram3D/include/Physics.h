#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H
#include "Trajectory.h"
namespace Physics {
	void UpdateBodies(std::vector<std::shared_ptr<Body>>& bodies, float deltaTime);
	std::vector<glm::vec3> CalculateForces(std::vector<std::shared_ptr<Body>>& bodies);
	void closerHill(std::shared_ptr<Body>& curr, std::shared_ptr<Body>& pParent);
	Trajectory::TrajectoryStruct calculateTrajectory(const Body& body, const Body& parent);
	float Distance(std::shared_ptr<Body>& const body1, std::shared_ptr<Body>& const body2);
	void CalculateEllipticalState3D(float a, float e, float i_rad, float omega_rad, float arg_rad, float nu_rad, float mu, glm::vec3& r_out, glm::vec3& v_out);
}
#endif