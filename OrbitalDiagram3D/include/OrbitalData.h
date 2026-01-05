#pragma once
#ifndef ORBITALDATA_H
#define ORBITALDATA_H
#include <glm/glm.hpp>
struct OrbitData {
	glm::mat4 model; // rotation + translation
	float semimajorAxis = 0;
	float eccentricity = 0;
	float v = 0; // current true anomaly
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};
#endif