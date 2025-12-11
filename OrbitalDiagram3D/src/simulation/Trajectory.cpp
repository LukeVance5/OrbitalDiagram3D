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
        float cosVarpi = glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), eccentricityVector) / eccentricity;
        float sinVarpi = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), eccentricityVector) / eccentricity; // Reference Z-axis

        float Varpi = glm::atan(sinVarpi, cosVarpi);
        if (Varpi < 0.0f) { Varpi += 2.0f * Units::PI; }
        traj.omega = Varpi;
    }
    else {
        // Case: Tilted Orbit (0 < i < 180)

        // 1. Longitude of Ascending Node (Omega) using atan2
        // Node vector n = (h.z, 0, -h.x). Angle is measured in the XZ (reference) plane.
        // atan2(y-component, x-component) -> atan2(n.z, n.x)
        float Omega = glm::atan(node.z, node.x);
        if (Omega < 0.0f) { Omega += 2.0f * Units::PI; }
        traj.Omega = Omega;

        glm::vec3 n_norm = glm::normalize(node);
        glm::vec3 w_norm = glm::cross(traj.Normal, n_norm); // traj.Normal is h_norm

        // Calculate dot products for atan2(sin_component, cos_component)
        float cos_omega = glm::dot(eccentricityVector, n_norm) / eccentricity;
        float sin_omega = glm::dot(eccentricityVector, w_norm) / eccentricity;

        float omega = glm::atan(sin_omega, cos_omega);
        if (omega < 0.0f) { omega += 2.0f * Units::PI; }
        traj.omega = omega;
    }

    // --- True Anomaly (v) ---
    // Angle from eccentricity vector (periapsis) to position vector (r)

    // Check for circular orbit singularity (e ~ 0)
    if (eccentricity < 1e-6f) {
        // Case: Circular Orbit (e = 0)
        // True Anomaly is defined relative to the Ascending Node (True Longitude)

        // Calculate angle from node to position vector (True Longitude)
        // This is complex and often handled by setting v=0 and relying on a separate long. element.
        // For simplicity and matching common conventions for e=0:
        // Use the angle from the Ascending Node vector n to the position vector r (True Longitude)
        float cos_nu_circ = glm::dot(node, r) / (nodeMag * r_mag);
        float sin_nu_circ = glm::dot(h, glm::cross(node, r)) / (h_mag * nodeMag * r_mag);
        float nu = glm::atan(sin_nu_circ, cos_nu_circ);
        if (nu < 0.0f) { nu += 2.0f * Units::PI; }
        traj.v = nu;

    }
    else {
        // Case: Elliptical Orbit (e > 0)
        // nu is measured from e-vector (periapsis) to r-vector (position)

        // cos_nu calculation (Angle between e-vector and r-vector)
        float cos_nu = glm::dot(eccentricityVector, r) / (eccentricity * r_mag);

        // Radial velocity (used for quadrant check: moving away or towards periapsis)
        float r_dot_v = glm::dot(r, v);

        // Use acos for 0 to pi, then check the sign of the radial velocity for 0 to 2pi range
        float nu = glm::acos(glm::clamp(cos_nu, -1.0f, 1.0f));

        if (r_dot_v < 0.0f) {
            // Body is moving toward the central mass (past apoapsis, nu > 180 deg)
            nu = 2.0f * Units::PI - nu;
        }
        traj.v = nu;
    }


    // --- Final Assignments ---
    traj.angularMomentum = h;
    traj.Normal = glm::normalize(h);
    traj.node = node; // Node vector (not normalized for consistency with singularity check)
    traj.eccentriciyVector = eccentricityVector;
    traj.eccentricity = eccentricity;

    return traj;
}
