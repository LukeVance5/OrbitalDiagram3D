#include "camera.h"
Camera::Camera(glm::vec3 position) 
 : cameraPos(position),
 worldUp(glm::vec3(0.0f,1.0f, 0.0f)),
 yaw(-90.0f),
 pitch(0.0f),
 zoom(45.0f),
 cameraFront(glm::vec3(0.0f,0.0f, -1.0f)){
	updateCameraVectors();
}

void Camera::updateCameraDirection(double dx, double dy) {
	yaw += dx*this->mouseSensitivity;
	pitch += dy*this->mouseSensitivity;
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
	if (zoom >= 1.0f && zoom <= 45.0f) {
		zoom -= (float)dy;
	} else if (zoom < 1.0f) {
		zoom = 1.0f;
	}
	else if (zoom > 45.0f) {
		zoom = 45.0f;
	}
}
glm::mat4 Camera::getViewMatrix() {
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
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::moveIfTracking() {
	if (trackedBody != nullptr) {
		cameraPos = trackedBody->position + glm::vec3(0.0f, trackedBody->radius * 5.0f, trackedBody->radius * 10.0f);
		cameraFront = glm::normalize(trackedBody->position - cameraPos);
		updateCameraVectors();
	}
}

void Camera::trackNextBody(const std::vector<ObjectStruct>& objectStructs) {
	if (objectStructs.empty()) return;
	trackBodyIndex++;
	unsigned int bodyCount = 0;
	for (const auto& objStruct : objectStructs) {
		for (const auto& body : objStruct.bodies) {
			if (bodyCount == trackBodyIndex) {
				trackedBody = body;
				return;
			}
			bodyCount++;
		}
	}
	if (trackBodyIndex >= bodyCount) {
		trackBodyIndex = -1;
		trackedBody = nullptr;
	}
}

void Camera::untrackBody() {
	trackBodyIndex = -1;
	trackedBody = nullptr;
}

