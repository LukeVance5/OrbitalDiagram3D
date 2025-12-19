#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H
#include "Cube.h"
#include "Shader.h"
#include <memory>

class Skybox {
public:
    Skybox();
    void draw(glm::mat4 projection, const Camera* camera);
private:
	void bindTextures();
    std::shared_ptr<Cube> cubeMesh;
    unsigned int textureID = 0;
    std::shared_ptr<Shader> shader;
};
#endif