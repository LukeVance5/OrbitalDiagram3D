#include "Simulation.h"
#include "Mesh.h"
#include "Sphere.h"
#include "PhysicsEngine.h"
#include <GLFW/glfw3.h>
#include <iostream>

Simulation::Simulation() : engine() {
    lastTime = 0.0f;
	currentTime = 0.0f;
	for (int i = 0; i < sizeof(simSteps) / sizeof(simSteps[0]); i++) {
        simulationSteps.push_back(simSteps[i]);
    }
}

void Simulation::simulationStep(double deltaTime) {
	lastTime = currentTime;
    if (this->paused) {
        return;
	}
	ObjectStruct ms = ObjectStructs.front();
	engine.UpdateBodies(ObjectStructs.front().bodies,(float) simulationSteps.at(physics_step) * deltaTime);
	return;
}

void Simulation::addObject(std::shared_ptr<Body> body) {
    if (this->host == nullptr) {
        addHost(body);
	} else if (body->getParent() == nullptr) {
        body->setParent(this->host);
	}
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

void Simulation::addHost(std::shared_ptr<Body> host) {
	if (host->getParent() != nullptr) {
        throw std::runtime_error("Host body cannot have a parent.");
    }
	this->host = host;
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

void Simulation::pauseUnpauseSimulation() {
    this->paused = !this->paused;
}
std::vector<ObjectStruct> Simulation::getObjectStructs() {
    return ObjectStructs;
}

std::shared_ptr<Body> Simulation::getHost() {
    return host;
}