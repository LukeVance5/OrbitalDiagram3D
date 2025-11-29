#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H
namespace Physics {
	void UpdateBodies(std::vector<std::shared_ptr<Body>>& bodies, float deltaTime);
	std::vector<glm::vec3> CalculateForces(const std::vector<std::shared_ptr<Body>>& bodies);
}
#endif