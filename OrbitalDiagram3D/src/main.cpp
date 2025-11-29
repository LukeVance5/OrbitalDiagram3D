#include <iostream>
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include "io/Camera.h"
#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "mesh/Sphere.h"
#include "body/Body.h"
#include "rendering/Render.h"
#include "rendering/Shader.h"
#include "rendering/TextureLoader.h"
#include "simulation/Simulation.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int setUpWindow(GLFWwindow* window);
void processInput(GLFWwindow* window, double dt, Simulation* simulation);

glm::mat4 view = glm::mat4(1.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 40.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
Render* render;
int main() {
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
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/vertex_core.glsl", "assets/fragment_core.glsl");
	Simulation* simulation = &Simulation::Instance();
	unsigned int moonTextureID = TextureLoader::LoadTexture("assets/images/moon.jpg");
	unsigned int earthTextureID = TextureLoader::LoadTexture("assets/images/earth.jpg");
	std::cout << "Loaded texture ID: " << earthTextureID << std::endl;
	std::shared_ptr<Mesh> mesh = Sphere::Instance();
	std::shared_ptr<Body> body1 = std::make_shared<Body>("first", 1.734f, 7.348e22f, glm::vec3(-384.0f,0,0), glm::vec3(0,0,0.00122f),mesh, shader, moonTextureID);
	std::shared_ptr<Body> body2 = std::make_shared<Body>("second", 6.378f, 5.972e24f, glm::vec3(10,0,0), glm::vec3(0), mesh, shader, earthTextureID);
	simulation->addObject(body1);
	simulation->addObject(body2);
	render = new Render(window, simulation, &camera, SCR_WIDTH, SCR_HEIGHT);
	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		simulation->simulationStep();
		processInput(window, deltaTime, simulation);
		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render->update();
	}
	
	glfwTerminate();
	return 0;
}

int setUpWindow(GLFWwindow* window) {
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	glfwSetKeyCallback(window, Keyboard::keyCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	render->changeScreen(width, height);
}

void processInput(GLFWwindow* window, double dt, Simulation* simulation) {
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
		simulation->increaseSimulationStep();
	}
	if ((Keyboard::key(GLFW_KEY_DOWN)) and (Keyboard::keyChanged(GLFW_KEY_DOWN))) {
		simulation->decreaseSimulationStep();
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
