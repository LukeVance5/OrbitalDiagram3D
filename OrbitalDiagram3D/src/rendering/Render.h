#pragma once
#include "body/Body.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "simulation/Simulation.h"
#include "mesh/Mesh.h"
#include "io/Camera.h"
#ifndef RENDERER_H
#define RENDERER_H

class Render {
	public:
		Render(GLFWwindow* window, Simulation* simulation, Camera* camera, unsigned int width, unsigned height);
		void update();
		void changeScreen(int width, int height);
	private:
		Simulation* simulation;
		GLFWwindow* window;
		Camera* camera;
		unsigned int SCR_WIDTH, SCR_HEIGHT;
		void draw(std::vector<std::shared_ptr<Body>> bodies);
};
#endif // !RENDERER_H"
