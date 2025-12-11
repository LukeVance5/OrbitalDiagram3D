#pragma once

#ifndef TRAJECTORY_H
#define TRAJECTORY_H
#include <memory>
#include "Body.h"
#include "Units.h"
#include <stdexcept>
#include "OrbitalDiagram3DExport.h"
// All angles in this case should be in radians


namespace Trajectory {
	struct OD_API TrajectoryStruct {
		glm::vec3 angularMomentum;
		glm::vec3 Normal; // vector perpendicular to orbital plane
		glm::vec3 node; // vector pointing towards ascending node
		glm::vec3 eccentriciyVector;
		float semiMajorAxis;
		float eccentricity;
		float inclination; // Angles
		float omega; // argument of periapsis
		float Omega; // longitude of ascending node
		float v; // true anomaly
	};
	constexpr float MAX_ECCENTRICIY = 0.98f;
	constexpr float EPSILON = 1e-6f;
	TrajectoryStruct OD_API calculateTrajectory(const Body& body, const Body& parent);
};
 
#endif