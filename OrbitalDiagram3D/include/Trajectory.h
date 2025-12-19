#pragma once

#ifndef TRAJECTORY_H
#define TRAJECTORY_H
#include <stdexcept>
#include <glm/glm.hpp>
#include "OrbitalDiagram3DExport.h"
#include "Units.h"
// All angles in this case should be in radians


class Body;
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
	constexpr float MIN_ANGLE = -1e-2f; 
	constexpr float MAX_ECCENTRICIY = 0.98f;
	constexpr float EPSILON = 1e-6f;
	OD_API TrajectoryStruct calculateTrajectory(const Body& body, const Body& parent);
	inline double wrapAngle(double a) {
		a = std::fmod(a, 2.0 * Units::PI);
		if (a < 0) a += 2.0 * Units::PI;
		return a;
	}
};
 
#endif