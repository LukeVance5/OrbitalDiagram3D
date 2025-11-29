#include "Simulation.h"
#include "mesh/Mesh.h"
#include "mesh/Sphere.h"
#include "Physics.h"
#include <GLFW/glfw3.h>
Simulation& Simulation::Instance() {
	static Simulation instance;
	return instance;
}

Simulation::Simulation() {
    lastTime = 0.0f;
	currentTime = 0.0f;
	std::vector<MeshStruct> BodyStructs;
    int simSteps[] = {1,60, 3600,86400,259200};
	for (int i = 0; i < sizeof(simSteps) / sizeof(simSteps[0]); i++) {
        simulationSteps.push_back(simSteps[i]);
    }
}
void Simulation::simulationStep() {
    currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	MeshStruct ms = BodyStructs.front();
	Physics::UpdateBodies(BodyStructs.front().bodies,(float) simulationSteps.at(physics_step) * deltaTime);
    std::cout << ms.bodies.front()->position[0] << " bodies after update:" << std::endl;
	return;
}

void Simulation::addObject(std::shared_ptr<Body> body) {
    std::shared_ptr<Mesh> mesh = body->mesh;
    auto it = std::find_if(BodyStructs.begin(), BodyStructs.end(),
        [mesh](const MeshStruct& ms) { return ms.mesh == mesh; });
    if (it != BodyStructs.end()) {
        it->bodies.push_back(body);
    }
    else {
        BodyStructs.push_back(MeshStruct{ mesh, {body} });
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
std::vector<MeshStruct> Simulation::getBodyStructs() {
    return BodyStructs;
}