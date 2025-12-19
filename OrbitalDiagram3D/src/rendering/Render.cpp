#include "Render.h"
#include "Sphere.h"
#include "RenderContext.h"
Render::Render(GLFWwindow* window, Simulation* simulation, Camera* camera, unsigned int width, unsigned int height)
	: window(window),
	simulation(simulation),
	camera(camera),
	SCR_WIDTH(width),
	SCR_HEIGHT(height),
	skybox(), // constructs Skybox in place, no temporary
	renderTrajectories()
{
	meshes = std::unordered_map<std::string, std::shared_ptr<Mesh>>();
	shaders = std::unordered_map<std::string, std::shared_ptr<Shader>>();
	initShaders();
	initMeshes();
}

void Render::initShaders() {
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/shaders/vertex_core.glsl", "assets/shaders/fragment_core.glsl");
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
	projection = getProjectionMatrix();
    std::vector<ObjectStruct> objectStructs = simulation->getObjectStructs();
    for (auto& objectStruct : objectStructs) {
        if (!isAllowed(objectStruct.type)) {
            std::cout << "Warning: Object type " << objectStruct.type << " is not supported for rendering." << std::endl;
            continue;
        }
		RenderContext renderContext;
        std::shared_ptr<Mesh> mesh = this->meshes.at(objectStruct.type);
		renderContext.bodies = objectStruct.bodies;
		renderContext.shader = shaders[objectStruct.type];
		renderContext.view = glm::mat4(glm::mat3(camera->getViewMatrix()));
		renderContext.projection = projection;
		renderContext.camera = camera;
		mesh->bind(); 
		mesh->draw(renderContext);
    }
    glBindVertexArray(0);
	this->renderTrajectories.drawTrajectories(simulation->getHost(),camera,projection, this->SCR_WIDTH,this->SCR_HEIGHT);
	skybox.draw(projection,camera);
    glfwSwapBuffers(window);
    glfwPollEvents();
    return;
}
void Render::changeScreen(int width, int height) {
	this->SCR_HEIGHT = height;
	this->SCR_WIDTH = width;
}

glm::mat4 Render::getProjectionMatrix() {
	float nearPlane = 0.1f;
	float farPlane = 1e9f;
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, nearPlane, farPlane);
	return projection;
}