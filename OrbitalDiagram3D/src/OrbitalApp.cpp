#include "OrbitalApp.h"
#include <iostream>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>


#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SolSystem.h"
unsigned int w = 1600;
unsigned int h = 900;



OrbitalApp::OrbitalApp() : camera(glm::vec3(149597.0f, 0.0f, 40.0f)), SCR_WIDTH(w), SCR_HEIGHT(h), render(nullptr), simulation(nullptr) {


}

int OrbitalApp::run() {
	std::cout << "Hello, World!" << std::endl;
	// initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OrbitalDiagram3D", NULL, NULL);
	int val = setUpWindow(window);
	if (val == -1) {
		return -1;
	}
	SolSystem system;
	simulation = system.getSimulation();
	render = new Render(window, simulation, &camera, SCR_WIDTH, SCR_HEIGHT);
	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		deltaTime = static_cast<float> (currentTime - lastFrame);
		lastFrame = static_cast<float> (currentTime);
		simulation->simulationStep(deltaTime);
		processInput(window, deltaTime);
		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render->update();
	} 

	glfwTerminate();
	return 0;
}

int OrbitalApp::setUpWindow(GLFWwindow* window) {
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	// Set the user pointer to this instance
	glfwSetWindowUserPointer(window, this);

	// Register the static callback
	glfwSetFramebufferSizeCallback(window, OrbitalApp::framebuffer_size_callback);


	glfwSetKeyCallback(window, Keyboard::keyCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 0;
}

void OrbitalApp::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Get instance pointer
	OrbitalApp* app = static_cast<OrbitalApp*>(glfwGetWindowUserPointer(window));
	if (!app) return;

	app->onFramebufferResize(width, height);
}
void OrbitalApp::processInput(GLFWwindow* window, double dt) {
	if (Keyboard::key(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	// change mix value
	if (Keyboard::key(GLFW_KEY_W)) {
		camera.updateCameraPosition(CameraDirection::FORWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_S)) {
		camera.updateCameraPosition(CameraDirection::BACKWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_A)) {
		camera.updateCameraPosition(CameraDirection::LEFT, dt);
	}
	if (Keyboard::key(GLFW_KEY_D)) {
		camera.updateCameraPosition(CameraDirection::RIGHT, dt);
	}
	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
		camera.updateCameraPosition(CameraDirection::DOWN, dt);
	}
	if (Keyboard::key(GLFW_KEY_SPACE)) {
		camera.updateCameraPosition(CameraDirection::UP, dt);
	}
	if ((Keyboard::key(GLFW_KEY_UP)) and (Keyboard::keyChanged(GLFW_KEY_UP))) {
		camera.increaseCameraSpeed();
	}
	if ((Keyboard::key(GLFW_KEY_DOWN)) and (Keyboard::keyChanged(GLFW_KEY_DOWN))) {
		camera.decreaseCameraSpeed();
	}
	if ((Keyboard::key(GLFW_KEY_PERIOD)) and (Keyboard::keyChanged(GLFW_KEY_PERIOD))) {
		simulation->increaseSimulationStep();
	}
	if ((Keyboard::key(GLFW_KEY_COMMA)) and (Keyboard::keyChanged(GLFW_KEY_COMMA))) {
		simulation->decreaseSimulationStep();
	}
	if ((Keyboard::key(GLFW_KEY_TAB)) and (Keyboard::keyChanged(GLFW_KEY_TAB))) {
		camera.trackNextBody(simulation->getObjectStructs());
	}
	if ((Keyboard::key(GLFW_KEY_GRAVE_ACCENT)) and (Keyboard::keyChanged(GLFW_KEY_GRAVE_ACCENT))) {
		std::cout << "Accent went down" << std::endl;
		camera.untrackBody();
	}
	double dx = Mouse::getDX(), dy = Mouse::getDY();
	if ((dx != 0 || dy != 0) && (Mouse::button(GLFW_MOUSE_BUTTON_RIGHT))) {
		camera.updateCameraDirection(dx, dy);
	}

	double scrollDy = Mouse::getScrollDY();
	if (scrollDy != 0) {
		camera.updateCameraZoom(scrollDy);
	}
}

void OrbitalApp::onFramebufferResize(int width, int height)
{
	glViewport(0, 0, width, height);

	// Update actual app members
	SCR_WIDTH = width;
	SCR_HEIGHT = height;

	if (render)
		render->changeScreen(width, height);
}