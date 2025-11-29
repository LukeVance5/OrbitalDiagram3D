#pragma once
#ifndef Simulation
#include "mesh/Sphere.h"
#include "body/Body.h"
#include <vector>
#include <memory>
struct MeshStruct {
	std::shared_ptr<Mesh> mesh;
	std::vector<std::shared_ptr<Body>> bodies;
};
class Simulation {
public:
	static Simulation& Instance();
	void addObject(std::shared_ptr<Body> body);
	void simulationStep();
	std::vector<MeshStruct> getBodyStructs();
private:
	Simulation();
	Simulation(const Simulation&) = delete;
	Simulation& operator= (const Simulation&) = delete;
	std::vector<MeshStruct> BodyStructs;
	double currentTime;
	double lastTime;
};
#endif 