#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "OrbitalDiagram3DExport.h"
#include "Simulation.h"
enum class CameraDirection {
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class OD_API Camera {
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 worldUp;

	float yaw; 
	float pitch;
	float zoom;

	Camera(glm::vec3 position);

	void updateCameraDirection(double dx, double dy);
	void updateCameraPosition(CameraDirection direction, double dt);
	void updateCameraZoom(double dy);
	void increaseCameraSpeed();
	void decreaseCameraSpeed();
	void moveIfTracking();
	void trackNextBody(const std::vector<ObjectStruct>& objectStructs);
	void untrackBody();
	glm::mat4 getViewMatrix();
private:
	std::shared_ptr<Body> trackedBody = nullptr;
	void updateCameraVectors();
	unsigned int speeds[5] = { 10,20, 50,100,200 };
	unsigned int speedIndex = 0;
	unsigned int speedSteps = 5;
	float mouseSensitivity = 0.1f;
	int trackBodyIndex = -1;
};

#endif CAMERA_H