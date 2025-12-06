#include "Render.h"
#include "Sphere.h"
Render::Render(GLFWwindow* window, Simulation* simulation, Camera* camera, unsigned int width, unsigned int height)
	: window(window),
	simulation(simulation),
	camera(camera),
	SCR_WIDTH(width),
	SCR_HEIGHT(height),
	skybox() // constructs Skybox in place, no temporary
{
	meshes = std::unordered_map<std::string, std::shared_ptr<Mesh>>();
	shaders = std::unordered_map<std::string, std::shared_ptr<Shader>>();
	initShaders();
	initMeshes();
}

void Render::initShaders() {
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/vertex_core.glsl", "assets/fragment_core.glsl");
	shaders["body"] = shader;
}
// Instantiates the meshes used for rendering for each possible mesh type
void Render::initMeshes() {
	std::shared_ptr<Mesh> sphereMesh = Sphere::Instance();
	meshes["body"] = sphereMesh;
}
// Checks if the simulation has added a body that does not have a corresponding mesh type
bool Render::isAllowed(const std::string& type) {
	for (auto allowed : Render::ALLOWED_OBJECTS) {
		if (type == allowed) {   // std::string == std::string_view OK
			return true;
		}
	}
	return false;
}
void Render::update() {
	camera->updateCameraVectors();
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	projection = getProjectionMatrix(); 
	view = glm::mat4(glm::mat3(camera->getViewMatrix()));
    std::vector<ObjectStruct> objectStructs = simulation->getObjectStructs();
    for (auto& objectStruct : objectStructs) {
        if (!isAllowed(objectStruct.type)) {
            std::cout << "Warning: Object type " << objectStruct.type << " is not supported for rendering." << std::endl;
            continue;
        }
        std::shared_ptr<Mesh> mesh = this->meshes.at(objectStruct.type);
        glBindVertexArray(mesh->VAO);
        draw(objectStruct.bodies, mesh->indices.size(),shaders[objectStruct.type]);
    }
    glBindVertexArray(0);
	skybox.draw(projection, view);
    glfwSwapBuffers(window);
    glfwPollEvents();
    return;
}

void Render::changeScreen(int width, int height) {
	this->SCR_HEIGHT = height;
	this->SCR_WIDTH = width;
}
void Render::draw(std::vector<std::shared_ptr<Body>> bodies, unsigned int indices_size,std::shared_ptr<Shader>& shader) {
	glm::mat4 view = glm::mat4(1.0f);
	
	for (auto& body : bodies) {
		//render
		// create transformation for screen
		glm::mat4 projection = getProjectionMatrix();
		view = camera->getViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, body->position);
		model = glm::scale(model, glm::vec3(body->radius));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f,0.0f));
		shader->activate();
		if (body->textureID != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, body->textureID);
			shader->setInt("hasTexture", 1);
			shader->setInt("texture1", 0);
		}
		else {
			shader->setInt("hasTexture", 0);
		}
		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_size), GL_UNSIGNED_INT, 0);
	}
	
}

glm::mat4 Render::getProjectionMatrix() {
	float nearPlane = 0.1f;
	float farPlane = 1e9f;
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, nearPlane, farPlane);
	return projection;
}