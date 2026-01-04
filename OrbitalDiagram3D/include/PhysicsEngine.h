#pragma once
#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H
#include "Trajectory.h"
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "OrbitalDiagram3DExport.h"
class Body;

class OD_API PhysicsEngine {
public:
	PhysicsEngine();
	void UpdateBodies(std::vector<std::shared_ptr<Body>>& bodies, float deltaTime);
	std::vector<glm::vec3> calculateForces(std::vector<std::shared_ptr<Body>>& bodies);
	std::vector<glm::vec3> calculateForces(std::vector<std::shared_ptr<Body>>& bodies, std::vector<glm::vec3> positions);
	Trajectory::TrajectoryStruct calculateTrajectory(const Body& body, const Body& parent);
	void CalculateEllipticalState3D(float a, float e, float i_rad, float omega_rad, float arg_rad, float nu_rad, float mu, glm::vec3& r_out, glm::vec3& v_out);
private:
	void closerHill(std::shared_ptr<Body>& curr, std::shared_ptr<Body>& pParent);
	float Distance(std::shared_ptr<Body>& const body1, std::shared_ptr<Body>& const body2);
	void updateVectors(std::vector<std::shared_ptr<Body>>& bodies, float deltatime);
	void updateTrajectories(std::vector<std::shared_ptr<Body>>& bodies);
};
#endif