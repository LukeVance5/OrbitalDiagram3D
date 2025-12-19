#include "Trajectory.h"
#include <memory>
#include "Units.h"
#include "Body.h"

Trajectory::TrajectoryStruct
Trajectory::calculateTrajectory(const Body& body, const Body& parent)
{
    // Relative position and velocity (DOUBLE)
    glm::dvec3 r = glm::dvec3(body.position) - glm::dvec3(parent.position);
    glm::dvec3 v = glm::dvec3(body.velocity) - glm::dvec3(parent.velocity);

    if (glm::length(r) == 0.0 || glm::length(v) == 0.0) {
        throw std::runtime_error("Relative position or velocity is zero vector in trajectory calculation.");
    }

    double r_mag = glm::length(r);
    glm::dvec3 r_norm = glm::normalize(r);

    double mu = Units::G * (body.mass + parent.mass); // assume G is double

    // --- Angular momentum ---
    glm::dvec3 h = glm::cross(r, v);
    double h_mag = glm::length(h);

    // --- Eccentricity vector ---
    glm::dvec3 eccentricityVector =
        (glm::cross(v, h) / mu) - r_norm;

    double eccentricity = glm::length(eccentricityVector);

    if (eccentricity > MAX_ECCENTRICIY) {
        throw std::runtime_error("Body is in an escape trajectory.");
    }

    TrajectoryStruct traj{};

    // --- Energy & semi-major axis ---
    double energy = glm::dot(v, v) * 0.5 - mu / r_mag;
    traj.semiMajorAxis = -mu / (2.0 * energy);

    // --- Inclination ---
    glm::dvec3 k(0.0, 1.0, 0.0);
    double i = glm::acos(glm::clamp(h.y / h_mag, -1.0, 1.0));
    traj.inclination = i;

    // --- Ascending node ---
    glm::dvec3 node = glm::cross(k, h);
    double nodeMag = glm::length(node);

    constexpr double EPS = 1e-12;

    // --- Argument of periapsis & RAAN ---
    if (nodeMag < EPS) {
        // In-plane orbit
        traj.Omega = 0.0;

        double cosVarpi =
            glm::dot(glm::dvec3(0, 0, 1), eccentricityVector) / eccentricity;

        double sinVarpi =
            glm::dot(glm::dvec3(1, 0, 0), eccentricityVector) / eccentricity;

        traj.omega = wrapAngle(glm::atan(sinVarpi, cosVarpi));
    }
    else {
        /*
        double Omega = glm::atan(node.x, node.z);
        if (Omega < MIN_ANGLE) Omega += 2.0 * Units::PI;
        traj.Omega = Omega;

        double dot = glm::dot(eccentricityVector, node);
        double norms = eccentricity * nodeMag;

        double omega = glm::acos(glm::clamp(dot / norms, -1.0, 1.0));
        if (omega < MIN_ANGLE) omega += 2.0 * Units::PI;

        traj.omega = omega;
        */
        double Omega = glm::atan(node.x, node.z);
        if (Omega < 0.0) Omega += 2.0 * Units::PI;
        traj.Omega = wrapAngle(Omega);

        // --- Argument of periapsis (ROBUST atan2 FORM) ---
        double cos_omega =
            glm::dot(node, eccentricityVector) / (nodeMag * eccentricity);

        double sin_omega =
            glm::dot(glm::cross(node, eccentricityVector), h) /
            (nodeMag * eccentricity * h_mag);

        traj.omega = wrapAngle(glm::atan(sin_omega, cos_omega));
    }

    // --- True anomaly ---
    if (eccentricity < EPS) {
        // Circular orbit
        double cos_nu =
            glm::dot(node, r) / (nodeMag * r_mag);

        double sin_nu =
            glm::dot(h, glm::cross(node, r)) / (h_mag * nodeMag * r_mag);

        traj.v = wrapAngle(glm::atan(sin_nu, cos_nu));
    }
    else {
        double cos_nu =
            glm::dot(eccentricityVector, r) / (eccentricity * r_mag);

        double sin_nu =
            glm::dot(glm::cross(eccentricityVector, r), h) /
            (eccentricity * r_mag * h_mag);

        traj.v = wrapAngle(glm::atan(sin_nu, cos_nu));
    }

    // --- Final assignments ---
    traj.angularMomentum = glm::vec3(h);
    traj.Normal = glm::vec3(glm::normalize(h));
    traj.node = glm::vec3(node);
    traj.eccentriciyVector = glm::vec3(eccentricityVector);
    traj.eccentricity = eccentricity;

    return traj;
}
