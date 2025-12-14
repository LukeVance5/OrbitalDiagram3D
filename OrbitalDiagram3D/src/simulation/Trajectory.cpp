#include "Trajectory.h"

Trajectory::TrajectoryStruct Trajectory::calculateTrajectory(const Body& body, const Body& parent) {
    // Relative position and velocity
    glm::vec3 r = body.position - parent.position;
    glm::vec3 v = body.velocity - parent.velocity;

    // --- Initial Checks and Constants ---
    if (glm::length(r) == 0.0f || glm::length(v) == 0.0f) {
        throw std::runtime_error("Relative position or velocity is zero vector in trajectory calculation.");
    }

    float r_mag = glm::length(r);
    glm::vec3 r_norm = glm::normalize(r);
    float mu = Units::G * (body.mass + parent.mass); // Gravitational parameter

    // --- Core Vectors ---
    glm::vec3 h = glm::cross(r, v); // Angular momentum
    float h_mag = glm::length(h);

    // Eccentricity vector
    glm::vec3 eccentricityVector = (glm::cross(v, h) / mu) - r_norm;
    float eccentricity = glm::length(eccentricityVector);

    if (eccentricity > MAX_ECCENTRICIY) { // Note: MAX_ECCENTRICIY should be 1.0 for parabolic/hyperbolic trajectories
        throw std::runtime_error("Body is in an escape Trajectory");
    }

    TrajectoryStruct traj;

    // --- Energy and Semi-Major Axis ---
    float energy = glm::dot(v, v) * 0.5f - mu / r_mag;
    traj.semiMajorAxis = -mu / (2.0f * energy);

    // --- Inclination (i) ---
    // k is the Y-axis pole (0, 1, 0)
    glm::vec3 k = glm::vec3(0.0f, 1.0f, 0.0f);
    float i = glm::acos(glm::clamp(h.y / h_mag, -1.0f, 1.0f));
    traj.inclination = i;

    // --- Ascending Node Vector (n) ---
    glm::vec3 node = glm::cross(k, h);
    float nodeMag = glm::length(node);

    // --- Singularity Check for Inclination (i ~ 0 or i ~ 180 deg) ---
    if (nodeMag < 1e-6f) {
        // Case: In-Plane Orbit (i = 0 or 180)
        traj.Omega = 0.0f; // Longitude of Ascending Node is arbitrary/set to zero

        // Calculate Longitude of Periapsis (ϖ) relative to X-axis
        // Reference X-axis is (1, 0, 0)
        float cosVarpi = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), eccentricityVector) / eccentricity;
        float sinVarpi = glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), eccentricityVector) / eccentricity; // Reference Z-axis

        float Varpi = glm::atan(sinVarpi, cosVarpi);
        if (Varpi < MIN_ANGLE) { Varpi += 2.0f * Units::PI; }
        traj.omega = Varpi;
    }
    else {
        // Case: Tilted Orbit (0 < i < 180)
        float Omega = glm::atan(node.x, node.z);
        if (Omega < MIN_ANGLE) { Omega += 2.0f * Units::PI; }
        traj.Omega = Omega;
        // Calculate dot products for atan2(sin_component, cos_component)
		float dot = glm::dot(eccentricityVector, node);
		float norms = eccentricity * nodeMag;
        float omega = glm::acos(dot / norms);
        if (omega < MIN_ANGLE) { omega += 2.0f * Units::PI; }
        traj.omega = omega;
    }

    // --- True Anomaly (v) ---
    // Angle from eccentricity vector (periapsis) to position vector (r)

    // Check for circular orbit singularity (e ~ 0)
    if (eccentricity < 1e-6f) {
        // Case: Circular Orbit (e = 0)
        float cos_nu_circ = glm::dot(node, r) / (nodeMag * r_mag);
        float sin_nu_circ = glm::dot(h, glm::cross(node, r)) / (h_mag * nodeMag * r_mag);
        float nu = glm::atan(sin_nu_circ, cos_nu_circ);
        if (nu < MIN_ANGLE) { nu += 2.0f * Units::PI; }
        traj.v = fmod(nu, 2.0f * Units::PI);

    }
    else {
        // Case: Elliptical Orbit (e > 0)
        // nu is measured from e-vector (periapsis) to r-vector (position)

        // cos_nu calculation (Angle between e-vector and r-vector)
        float cos_nu = glm::dot(eccentricityVector, r) / (eccentricity * r_mag);

        // Use acos for 0 to pi, then check the sign of the radial velocity for 0 to 2pi range
        float nu = glm::acos(glm::clamp(cos_nu, -1.0f, 1.0f));
        if (nu < MIN_ANGLE) nu += 2.0f * Units::PI;
        traj.v = fmod(nu, 2.0f * Units::PI);
    }


    // --- Final Assignments ---
    traj.angularMomentum = h;
    traj.Normal = glm::normalize(h);
    traj.node = node; // Node vector (not normalized for consistency with singularity check)
    traj.eccentriciyVector = eccentricityVector;
    traj.eccentricity = eccentricity;

    return traj;
}
