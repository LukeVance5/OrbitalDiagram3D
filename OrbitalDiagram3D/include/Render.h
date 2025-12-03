#pragma once
#include "Body.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <string_view>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Simulation.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "OrbitalDiagram3DExport.h"
#include "Skybox.h"
#ifndef RENDERER_H
#define RENDERER_H


class OD_API Render {
	public:
		Render(GLFWwindow* window, Simulation* simulation, Camera* camera, unsigned int width, unsigned height);
		void update();
		void changeScreen(int width, int height);
	private:
		Simulation* simulation;
		GLFWwindow* window;
		Camera* camera;
		std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
		std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
		Skybox skybox;
		unsigned int SCR_WIDTH, SCR_HEIGHT;
		void draw(std::vector<std::shared_ptr<Body>> bodies, unsigned int indices_size, std::shared_ptr<Shader>& shader);
		void initShaders();
		void initMeshes();
		glm::mat4 getProjectionMatrix();
		static bool isAllowed(const std::string& type);
		static inline constexpr std::array<std::string_view, 1> ALLOWED_OBJECTS = {
	"body"};
};
#endif // !RENDERER_H"
