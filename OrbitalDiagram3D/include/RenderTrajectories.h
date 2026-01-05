#pragma once
#ifndef RENDERTRAJECTORIES_H
#define RENDERTRAJECTORIES_H
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Circle.h"
#include "LargeCircle.h"
#include "Trajectory.h"
#include "Camera.h"
#include "Shader.h"
class RenderTrajectories {
public:
	RenderTrajectories();
	void drawTrajectories(std::shared_ptr<Body> host, Camera* camera, glm::mat4 projection, int width, int height);
private:
	void createOrbitalData(std::vector<OrbitData>& orbits,std::shared_ptr<Body> curr ,std::shared_ptr<Body> host, glm::vec3 cameraPos);
	glm::mat3 calculateRotations(const Trajectory::TrajectoryStruct& traj);
	std::shared_ptr<Circle> circleMesh;
	std::shared_ptr<LargeCircle> parentCircleMesh;
	std::shared_ptr<Shader> shader;
	const float OPACITY = 0.7f;
};
#endif
