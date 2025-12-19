#pragma once
#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include "Camera.h"
#include "Shader.h"
#include <span>
struct RenderContext {
    // Camera & matrices
    Camera* camera = nullptr;
    glm::mat4 view{ 1.0f };
    glm::mat4 projection{ 1.0f };
	glm::mat4 model{ 1.0f };
    std::span<std::shared_ptr<Body>> bodies;
    // Common shader registry (owned by renderer)
    std::shared_ptr<Shader> shader = nullptr;
	unsigned int textureID = 0;
    // Frame-level info (optional but useful)
    float screenWidth = 0.0f;
	float screenHeight = 0.0f;
    float time = 0.0f;
};

#endif
