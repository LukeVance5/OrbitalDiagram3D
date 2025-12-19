#include "pch.h"
#include "tests/simulationTest.h"
#include <glm/glm.hpp>
#include "Simulation.h"
#include "Sphere.h"
#include "Units.h"
#include <memory>

namespace SimulationTests
{

	class SimulationTestFixtureBase : public ::testing::Test {
	protected:
		const float SUN_MASS = 1.989e30f;
		const float MOON_MASS = 7.342e22f;
		const float EARTH_MASS = 5.972e24f;
		const float SUN_RADIUS = 6.9634e8f / Units::DISTANCE_SCALE;
		const float MOON_RADIUS = 1.7371e6f / Units::DISTANCE_SCALE;
		const float EARTH_RADIUS = 6.378e6f / Units::DISTANCE_SCALE;
		const float EARTH_SUN_DISTANCE = 1.495e11f / Units::DISTANCE_SCALE;
		const float MOON_EARTH_DISTANCE = 3.844e8f / Units::DISTANCE_SCALE;
		const float EARTH_ORBITAL_SPEED = 29780.0f / Units::DISTANCE_SCALE;
		const float MOON_ORBITAL_SPEED = 1022.0f / Units::DISTANCE_SCALE;
		void SetUp() override {
			// Setup code if needed
		}
	};
	TEST_F(SimulationTestFixtureBase, AddObjectIncreasesBodyStructs) {
		Simulation* sim = new Simulation();
		size_t initialSize = sim->getObjectStructs().size();
		EXPECT_EQ(0, initialSize);
		std::shared_ptr<Body> body = Body::create("testBody", SUN_RADIUS, SUN_MASS, glm::vec3(0), glm::vec3(0), "body");
		sim->addObject(body);
		size_t newSize = sim->getObjectStructs().size();
		EXPECT_EQ(1, newSize);
	}

	TEST_F(SimulationTestFixtureBase, EarthMoonSunSystemParentsAddedX) {
		std::shared_ptr<Body> sun = Body::create("Sun", SUN_RADIUS, SUN_MASS, glm::vec3(0), glm::vec3(0), "body");
		std::shared_ptr<Body> earth = Body::create("Earth", EARTH_RADIUS, EARTH_MASS, glm::vec3(EARTH_SUN_DISTANCE, 0, 0), glm::vec3(0, 0, EARTH_ORBITAL_SPEED), "body", sun);
		std::shared_ptr<Body> moon = Body::create("Moon", 1737.1f, 7.342e22f, glm::vec3(MOON_EARTH_DISTANCE, 0, 0), glm::vec3(0, 0, MOON_ORBITAL_SPEED), "body", earth);
		EXPECT_EQ(0, sun->velocity[2]);
		EXPECT_EQ(EARTH_ORBITAL_SPEED, earth->velocity[2]);
		EXPECT_EQ(MOON_ORBITAL_SPEED + EARTH_ORBITAL_SPEED, moon->velocity[2]);
	}
}
