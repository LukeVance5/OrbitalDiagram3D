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
}
void Simulation::simulationStep() {
    currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	MeshStruct ms = BodyStructs.front();
    std::cout << deltaTime << " Time " << std::endl;
	//std::cout << ms.bodies.size() << " bodies before update:" << std::endl;
    unsigned int physics_step = 86400; //3600; // 1 hour per simulation step
	Physics::UpdateBodies(BodyStructs.front().bodies,(float) physics_step * deltaTime);
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

std::vector<MeshStruct> Simulation::getBodyStructs() {
    return BodyStructs;
}