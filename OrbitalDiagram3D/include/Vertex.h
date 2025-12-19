#pragma once

#include <glm/glm.hpp>

struct Vertex3D {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Vertex2D {
    glm::vec2 position;
};