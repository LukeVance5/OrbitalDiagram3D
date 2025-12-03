#include "Simulation.h"
#include "Mesh.h"
#include "Sphere.h"
#include "Physics.h"
#include <GLFW/glfw3.h>
#include <iostream>

Simulation::Simulation() {
    lastTime = 0.0f;
	currentTime = 0.0f;
	for (int i = 0; i < sizeof(simSteps) / sizeof(simSteps[0]); i++) {
        simulationSteps.push_back(simSteps[i]);
    }
}

void Simulation::simulationStep() {
    currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	ObjectStruct ms = ObjectStructs.front();
	Physics::UpdateBodies(ObjectStructs.front().bodies,(float) simulationSteps.at(physics_step) * deltaTime);
    std::cout << ms.bodies.front()->position[0] << " bodies after update:" << std::endl;
	return;
}

void Simulation::addObject(std::shared_ptr<Body> body) {
    std::string type = body->type;
    auto it = std::find_if(ObjectStructs.begin(), ObjectStructs.end(),
        [type](const ObjectStruct& ms) { return ms.type == type; });
    if (it != ObjectStructs.end()) {
        it->bodies.push_back(body);
    }
    else {
        ObjectStructs.push_back(ObjectStruct{ type, {body} });
    }
}

void Simulation::increaseSimulationStep() {
    if (physics_step < simulationSteps.size() - 1) {
        physics_step++;
	}
}

void Simulation::decreaseSimulationStep() {
    if (physics_step > 0) {
        physics_step--;
    }
}
std::vector<ObjectStruct> Simulation::getObjectStructs() {
    return ObjectStructs;
}