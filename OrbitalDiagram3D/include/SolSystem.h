#pragma once
#ifndef SOL_SYSTEM_H
#define SOL_SYSTEM_H
#include <string>
#include <memory>
#include "Simulation.h"
#include "PhysicsEngine.h"
class SolSystem {
public:
	SolSystem();
	Simulation* getSimulation();
private:
	const float EARTH_MASS = 5.972e24f;
	const float SUN_MASS = 1.9885e30f;
	const float MOON_MASS = 7.348e22f;
	const float MERCURY_MASS = 3.3011e23f;
	const float VENUS_MASS = 4.8675e24f;
	const float MARS_MASS = 6.4171e23f;
	Simulation* sim;
	PhysicsEngine engine;
	void addInnerPlanets();
	void addMercury();
	void addVenus();
	void addMars();
	void addEarthMoonSystem();
	const std::string body = "body";
};
#endif