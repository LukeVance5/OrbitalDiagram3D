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

void Skybox::draw(glm::mat4 projection, glm::mat4 view) {
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	this->shader->activate();
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	shader->setInt("skybox", 0);
	
	glBindVertexArray(cubeMesh->VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	return;
}