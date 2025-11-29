#include "Render.h"

Render::Render(GLFWwindow* window, Simulation* simulation, Camera* camera,unsigned int width, unsigned height) {
	this->window = window;
	this->simulation = simulation;
	this->SCR_HEIGHT = height;
	this->SCR_WIDTH = width;
	this->camera = camera;
}

void Render::update() {
	std::vector<MeshStruct> meshStructs = simulation->getBodyStructs();
	for (auto& meshStruct : meshStructs) {
		std::shared_ptr<Mesh> mesh = meshStruct.mesh;
		glBindVertexArray(mesh->VAO);
		draw(meshStruct.bodies);
	}
	glBindVertexArray(0);
	glfwSwapBuffers(window);
	glfwPollEvents();
	return;
}

void Render::changeScreen(int width, int height) {
	this->SCR_HEIGHT = height;
	this->SCR_WIDTH = width;
}
void Render::draw(std::vector<std::shared_ptr<Body>> bodies) {
	glm::mat4 view = glm::mat4(1.0f);
	float nearPlane = 0.1f;
	float farPlane = 1e9f;
	for (auto& body : bodies) {
		//render
		// create transformation for screen
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera->getViewMatrix();
		projection = glm::perspective(glm::radians(camera->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, nearPlane, farPlane);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, body->position);
		model = glm::scale(model, glm::vec3(body->radius));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f,0.0f));
		body->shader->activate();
		if (body->textureID != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, body->textureID);
			body->shader->setInt("hasTexture", 1);
			body->shader->setInt("texture1", 0);
		}
		else {
			body->shader->setInt("hasTexture", 0);
		}
		body->shader->setMat4("model", model);
		body->shader->setMat4("view", view);
		body->shader->setMat4("projection", projection);
		glDrawElements(GL_TRIANGLES, body->mesh->indices.size(), GL_UNSIGNED_INT, 0);
	}
	
}