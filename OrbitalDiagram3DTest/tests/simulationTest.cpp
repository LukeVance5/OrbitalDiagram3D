#include "pch.h"
#include "tests/simulationTest.h"
#include <glm/glm.hpp>
#include "Simulation.h"
#include "Sphere.h"
#include <memory>
TEST(SimulationTest, AddObjectIncreasesBodyStructs) {
	Simulation* sim = new Simulation();
	size_t initialSize = sim->getObjectStructs().size();
	EXPECT_EQ(0, initialSize);
	std::shared_ptr<Body> body = std::make_shared<Body>("testBody", 1.0f, 1.0e22f, glm::vec3(0), glm::vec3(0), "body");
	sim->addObject(body);
	size_t newSize = sim->getObjectStructs().size();
	EXPECT_EQ(1, newSize);
}