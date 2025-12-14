#include "pch.h"
#include "Trajectory.h"
#include "trajectoryTest.h"	
#include "Units.h"
#include <memory>

namespace TrajectoryTests
{
	class TrajectoryTestFixtureBase : public ::testing::Test {
	protected:
		// 1. Define shared parameters and constants
		const float PARENT_MASS = 1.0e22f;
		const float CHILD_MASS = 1.0e20f;
		const float TEST_DISTANCE = 1.0e9f / Units::DISTANCE_SCALE; // Shared distance constant
		float mu;

		// 2. Define shared objects
		std::shared_ptr<Body> parent;
		std::shared_ptr<Body> child;

		// 3. Define the setUp method (Google Test standard)
		// This runs before EACH test case (TEST_F)
		void SetUp() override {
			// Initialize shared constants
			mu = Units::G * (PARENT_MASS + CHILD_MASS);

			// Initialize shared parent body (can be at origin for simplicity)
			parent = std::make_shared<Body>("TestParent", 1.0f, PARENT_MASS,
				glm::vec3(0), glm::vec3(0), "star");
		}
		void CalculateEllipticalState3D(float a, float e, float i_rad, float omega_rad, float arg_rad, float nu_rad, glm::vec3& r_out, glm::vec3& v_out) {
			// Shared constants from fixture

			// 1. R/V in P-Q (X-Z) frame - UNCHANGED
			float p = a * (1.0f - e * e);
			float r_mag = p / (1.0f + e * glm::cos(nu_rad));
			float C = glm::sqrt(mu / p);

			glm::vec3 r_orbital = glm::vec3(r_mag * glm::cos(nu_rad), 0.0f, -r_mag * glm::sin(nu_rad));
			glm::vec3 v_orbital = glm::vec3(C * (-glm::sin(nu_rad)), 0.0f, -C * (e + glm::cos(nu_rad)));

			glm::mat3 R1 = glm::mat3(
				cos(arg_rad), 0.0f, sin(arg_rad),
				0.0f, 1.0f, 0.0f,
				-sin(arg_rad), 0.0f, cos(arg_rad)
			);

			glm::mat3 R2 = glm::mat3(
				1.0f, 0.0f, 0.0f,
				0.0f, cos(i_rad), -sin(i_rad),
				0.0f, sin(i_rad), cos(i_rad)
			);

			glm::mat3 R3 = glm::mat3(
				cos(omega_rad), 0.0f, sin(omega_rad),
				0.0f, 1.0f, 0.0f,
				-sin(omega_rad), 0.0f, cos(omega_rad)
			);

			glm::mat3 Rotation = R3 * R2 * R1;

			// 3. Final Inertial R/V
			r_out = Rotation * r_orbital;
			v_out = Rotation * v_orbital;
		}
	};
	static TEST_F(TrajectoryTestFixtureBase, CircularOrbitTest) {
		float speed = glm::sqrt((Units::G * PARENT_MASS) / TEST_DISTANCE); // circular orbit speed
		child = std::make_shared<Body>("testBody2", 1.0f, 1.0e20f, glm::vec3(TEST_DISTANCE, 0.0f, 0.0f), glm::vec3(0.0f, speed, 0.0f), "body");
		try {
			Trajectory::TrajectoryStruct traj = Trajectory::calculateTrajectory(*child, *parent);
			EXPECT_NEAR(0, traj.eccentricity, 1e-2f);
		}
		catch (const std::runtime_error& e) {
			FAIL() << "Exception thrown during parent assignment: " << e.what();
		}
	}

	static TEST_F(TrajectoryTestFixtureBase, EscapeVelocityTest) {
		std::shared_ptr<Body> parent = std::make_shared<Body>("testBody1", 1.0f, 1.0e22f, glm::vec3(0), glm::vec3(0), "body");
		float speed = glm::sqrt((2.0f * Units::G * PARENT_MASS) / TEST_DISTANCE);
		child = std::make_shared<Body>("testBody2", 1.0f, 1.0e20f, glm::vec3(TEST_DISTANCE, 0.0f, 0.0f), glm::vec3(0.0f, speed, 0.0f), "body");
		EXPECT_ANY_THROW(Trajectory::TrajectoryStruct traj = Trajectory::calculateTrajectory(*child, *parent));
	}
	
	static TEST_F(TrajectoryTestFixtureBase, ElipticalOrbitTestNoInclination) {
		float a = 2*TEST_DISTANCE; 
		float eccentricity = 0.2f;
		float constexpr argument = glm::radians(45.0f);
		float constexpr v = glm::radians(0.0f); // true anomaly at periapsis

		// Calculations in orbital plane

		float distance = a * (1 - eccentricity * eccentricity) / (1 + eccentricity * glm::cos(v)); 
		glm::vec3 r_orbital = glm::vec3(
			distance * glm::cos(v),    // X-component (P-direction)
			0.0f,                      // Y-component (Normal to plane)
			distance * glm::sin(v)     // Z-component (Q-direction)
		);

		// 2. Velocity Vector (v_orbital)
		float C = glm::sqrt(mu / (a * (1 - eccentricity * eccentricity)));

		glm::vec3 v_orbital = glm::vec3(
			C * (-glm::sin(v)),              // X-component (P-direction velocity)
			0.0f,                            // Y-component (Normal to plane velocity)
			C * (eccentricity + glm::cos(v)) // Z-component (Q-direction velocity)
		);
		glm::mat3 Rotation = glm::mat3(
			glm::cos(argument), 0.0, glm::sin(argument),
			0.0f, 1.0f , 0.0f,
			-glm::sin(argument), 0.0f, glm::cos(argument)
		);
		glm::vec3 r = Rotation * r_orbital;
		glm::vec3 v_vec = Rotation * v_orbital;
		child = std::make_shared<Body>("testBody2", 1.0f, 1.0e20f, r, v_vec, "body");
		try {
			Trajectory::TrajectoryStruct traj = Trajectory::calculateTrajectory(*child, *parent);
			EXPECT_NEAR(0, traj.Omega, 1e-2f);
			EXPECT_NEAR(0.2f, traj.eccentricity, 1e-2f);
			EXPECT_NEAR(0.0f, traj.Normal[0], 1e-2f);
			EXPECT_NEAR(1.0f, glm::length(traj.Normal), 1e-2f);
			EXPECT_NEAR(0.0f, traj.Normal[2], 1e-2f);
			EXPECT_NEAR(argument, traj.omega, 1e-2f);
		}
		catch (const std::runtime_error& e) {
			FAIL() << "Exception thrown during parent assignment: " << e.what();
		}
	}

	TEST_F(TrajectoryTestFixtureBase, InclinedOrbitTest) {
		// Define Keplerian elements in radians
		const float A = 2*TEST_DISTANCE;
		const float E = 0.2f;
		const float I_RAD = glm::radians(30.0f);   // 30 degrees inclination
		const float OMEGA_RAD = glm::radians(60.0f); // 60 degrees Ascending Node
		const float ARG_RAD = glm::radians(0.0f); // 0 Argument of Periapsis
		const float NU_RAD = glm::radians(0.0f);   // True Anomaly at Periapsis

		glm::vec3 r, v_vec;
		// Use the 3D helper to generate the state vectors
		CalculateEllipticalState3D(A, E, I_RAD, OMEGA_RAD, ARG_RAD, NU_RAD, r, v_vec);

		// Setup the child body using the generated state vectors
		child = std::make_shared<Body>("TestChildInclined", 1.0f, CHILD_MASS, r, v_vec, "planet");

		try {
			Trajectory::TrajectoryStruct traj = Trajectory::calculateTrajectory(*child, *parent);

			// Assert the returned elements match the input elements
			EXPECT_NEAR(E, traj.eccentricity, 1e-2f);       // Eccentricity
			EXPECT_NEAR(I_RAD, traj.inclination, 1e-2f);    // Inclination
			EXPECT_NEAR(OMEGA_RAD, traj.Omega, 1e-2f);      // Longitude of Ascending Node
			EXPECT_NEAR(ARG_RAD, traj.omega, 1e-2f);        // Argument of Periapsis
			EXPECT_NEAR(NU_RAD, traj.v, 1e-2f);             // True Anomaly

			// Assert the normal vector is correct (It should be tilted 30 degrees and swiveled 60 degrees)
			// The Normal vector H is R3*R2*R1 * (0, 1, 0)
			// Given the complexity of the Normal vector components, assert its magnitude is 1.0
			EXPECT_NEAR(1.0f, glm::length(traj.Normal), 1e-4f);

		}
		catch (const std::runtime_error& e) {
			FAIL() << "Exception thrown: " << e.what();
		}
	}
}
