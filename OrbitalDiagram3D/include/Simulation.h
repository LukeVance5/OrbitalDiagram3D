#pragma once
#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include <memory>
#include "OrbitalDiagram3DExport.h" 
#include "Body.h"
struct ObjectStruct {
	std::string type;
	std::vector<std::shared_ptr<Body>> bodies;
};
class OD_API Simulation {
public:
	Simulation();
	~Simulation() = default;
	void addObject(std::shared_ptr<Body> body);
	void simulationStep(double deltaTime);
	void increaseSimulationStep();
	void decreaseSimulationStep();
	std::vector<ObjectStruct> getObjectStructs();
	unsigned int physics_step = 3;
	std::shared_ptr<Body> getHost();
private:
	void addHost(std::shared_ptr<Body> host);
	std::vector<ObjectStruct> ObjectStructs;
	std::vector<int> simulationSteps;
	unsigned int maxSimulationStep = 4;
	unsigned int minSimulationStep = 0;
	double currentTime;
	double lastTime;
	int simSteps[5] = { 1,60, 3600,86400,259200 };
	std::shared_ptr<Body> host;
};
#endif 