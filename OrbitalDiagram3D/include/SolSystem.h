#pragma once
#ifndef SOL_SYSTEM_H
#define SOL_SYSTEM_H
#include <memory>
#include "Simulation.h"
class SolSystem {
public:
	SolSystem();
	Simulation* getSimulation();
private:
	const float EARTH_MASS = 5.972e24f;
	const float SUN_MASS = 1.9885e30f;
	const float MOON_MASS = 7.348e22f;
	Simulation* sim;
	void addInnerPlanets();
};
#endif