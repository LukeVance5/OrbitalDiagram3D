#include "camera.h"
#include <stdio.h>
#include <iostream>
Camera::Camera(glm::vec3 position) 
 : cameraPos(position),
 worldUp(glm::vec3(0.0f,1.0f, 0.0f)),
 yaw(-90.0f),
 pitch(0.0f),
 zoom(90.0f),
 cameraFront(glm::vec3(0.0f,0.0f, -1.0f)){
	updateCameraVectors();
}

void Camera::updateCameraDirection(double dx, double dy) {
	float mouseSensitivity;
	if (trackedBody != nullptr) {
		mouseSensitivity = mouseSensitivityTracked;
		yaw -= dx * mouseSensitivity;
		pitch -= dy * mouseSensitivity;
	} else {
		mouseSensitivity = mouseSensitivityRegular;
		yaw += dx * mouseSensitivity;
		pitch += dy * mouseSensitivity;
	}
	
	if (pitch > 89.0f) {
		pitch = 89.0f;
	} else if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	updateCameraVectors();
}
void Camera::updateCameraPosition(CameraDirection direction, double dt) {
	float velocity = (float) dt * speeds[speedIndex];
	switch (direction) {
	case CameraDirection::FORWARD:
		cameraPos += cameraFront * velocity;
		break;
	case CameraDirection::BACKWARD:
		cameraPos -= cameraFront * velocity;
		break;
	case CameraDirection::RIGHT:
		cameraPos += cameraRight * velocity;
		break;
	case CameraDirection::LEFT:
		cameraPos -= cameraRight * velocity;
		break;
	case CameraDirection::UP:
		cameraPos += cameraUp * velocity;
		break;
	case CameraDirection::DOWN:
		cameraPos -= cameraUp * velocity;
		break;
	default:
		break;
	}
}
void Camera::updateCameraZoom(double dy) {
	if (trackedBody != nullptr) {
		radialDistance -= (radialDistance * (float) dy ) / 10.0f;
		if (radialDistance < trackedBody->radius * 1.1f) {
			radialDistance = trackedBody->radius * 1.1f;
		} else if (radialDistance > trackedBody->radius * 1000.0f) {
			radialDistance = trackedBody->radius * 1000.0f;
		}
	} else {
		if (zoom >= 1.0f && zoom <= 90.0f) {
			zoom -= (float)dy;
		}
		else if (zoom < 1.0f) {
			zoom = 1.0f;
		}
		else if (zoom > 90.0f) {
			zoom = 90.0f;
		}
	}
	return;
	
}
glm::mat4 Camera::getViewMatrix() const {
	if (trackedBody != nullptr) {
		return glm::lookAt(cameraPos, trackedBody->position, worldUp);
	}
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::increaseCameraSpeed() {
	if (speedIndex < speedSteps - 1) {
		speedIndex++;
	}
}

void Camera::decreaseCameraSpeed() {
	if (speedIndex > 0) {
		speedIndex--;
	}
}
void Camera::updateCameraVectors() {
	if (trackedBody != nullptr) {
		updateIfTracked();
		return;
	}
	glm::vec3 direction{};
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	cameraFront = glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}
/*
This function causes the camera to use a coordinate system centered on the tracked body.
The reason why pitch and yaw are left in their degree form is to allow for the user to rotate freely around the body

CameraRight and CameraUp a recalculated to be used in untrackedBody().
*/
void Camera::updateIfTracked() {
	float x = radialDistance * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	float y = radialDistance * sin(glm::radians(pitch));
	float z = radialDistance * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraPos = trackedBody->position + glm::vec3(x, y, z);
	cameraFront = glm::normalize(trackedBody->position - cameraPos);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}
std::shared_ptr<Body> Camera::getTrackedBody() {
	return trackedBody;
}
// Tracks the next body in the list of objectStructs
void Camera::trackNextBody(const std::vector<ObjectStruct>& objectStructs) {
	if (objectStructs.empty()) return;
	trackBodyIndex++;
	std::cout << "Tracking body index: " << trackBodyIndex << std::endl;
	unsigned int bodyCount = 0;
	for (const auto& objStruct : objectStructs) {
		for (const auto& body : objStruct.bodies) {
			if (bodyCount == trackBodyIndex) {
				trackedBody = body;
				radialDistance = trackedBody->radius * 5.0f;
				return;
			}
			bodyCount++;
		}
	}
	if (trackBodyIndex >= bodyCount) {
		untrackBody();
	}
}

// Untracks the body but keeps the camera in the same position and orientation
void Camera::untrackBody() {
	trackBodyIndex = -1;
	trackedBody = nullptr;
	pitch = glm::asin(cameraFront.y);
	yaw = glm::atan(cameraFront.z, cameraFront.x);
	pitch = glm::degrees(pitch);
	yaw = glm::degrees(yaw);
	pitch = glm::degrees(glm::clamp(glm::radians(pitch), -glm::radians(89.0f), glm::radians(89.0f)));
}
