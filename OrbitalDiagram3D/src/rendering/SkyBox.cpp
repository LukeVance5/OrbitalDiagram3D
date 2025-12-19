#include "Skybox.h"
#include "TextureLoader.h"
#include <glad/glad.h>

Skybox::Skybox() {
	cubeMesh = Cube::Instance();
	bindTextures();
	this->shader = std::make_shared<Shader>("assets/shaders/vertex_skybox.glsl", "assets/shaders/fragment_skybox.glsl");
	if (!this->shader) {
		std::cerr << "Skybox shader failed to compile!" << std::endl;
	}
	return;
}

void Skybox::bindTextures() {
	std::cout << "bindTextures called" << std::endl;
	std::vector<std::string> faces
	{
		"assets/images/skybox/right.png",
		"assets/images/skybox/left.png",
		"assets/images/skybox/top.png",
		"assets/images/skybox/bottom.png",
		"assets/images/skybox/front.png",
		"assets/images/skybox/back.png"
	};
	textureID = TextureLoader::LoadTextureCube(faces);
	return;
}

void Skybox::draw(glm::mat4 projection, const Camera* camera) {
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	RenderContext ctx;
	ctx.view = glm::mat4(glm::mat3(camera->getViewMatrix()));
	ctx.projection = projection;
	ctx.textureID = textureID;
	ctx.shader = this->shader;
	cubeMesh->bind();
	cubeMesh->draw(ctx);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	return;
}