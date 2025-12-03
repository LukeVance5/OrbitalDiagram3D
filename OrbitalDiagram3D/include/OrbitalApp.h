#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Simulation.h"
#include "Render.h"
#include "OrbitalDiagram3DExport.h"
#ifndef ORBITALAPP_H
#define ORBITALAPP_H

class OD_API OrbitalApp {
	public:
	OrbitalApp();
	~OrbitalApp() = default;
	int run();
	private:
		glm::mat4 view = glm::mat4(1.0f);
		Camera camera;
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		unsigned int SCR_WIDTH, SCR_HEIGHT;
		Simulation* simulation;
		Render* render;
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		int setUpWindow(GLFWwindow* window);
		void processInput(GLFWwindow* window, double dt);
		void onFramebufferResize(int width, int height);
};
#endif
