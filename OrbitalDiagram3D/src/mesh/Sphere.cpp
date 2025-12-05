#include "Sphere.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

std::shared_ptr <Sphere> Sphere::Instance() {
    static std::shared_ptr<Sphere> instance = std::shared_ptr<Sphere>(new Sphere());
    return instance;
}
Sphere::Sphere() {
	float sectors = 48;
	float stacks = 48;
    generateVertices(stacks,sectors);
    generateIndices(stacks, sectors);
    generateBuffers();
}

void Sphere::generateVertices(float stacks, float sectors) {
    float radius = 1;
    float x, y, z, xy;
    float nx, ny, nz;
    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;
    vertices.clear();
    vertices.reserve((stacks + 1) * (sectors + 1));
    for (int i = 0; i <= stacks; i++) {
        float stackAngle = PI / 2 - i * stackStep;  // from +pi/2 to -pi/2
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; j++) {
            float sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            // normal = position normalized
            nx = x / radius;
            ny = y / radius;
            nz = z / radius;
            float u = (float)j / sectors;
            float v = (float)i / stacks;
            vertices.push_back({
                glm::vec3(x, y, z),
                glm::vec3(nx, ny, nz),
                glm::vec2(u, v) });
        }
    }
    return;
}

void Sphere::generateIndices(float stacks, float sectors) {
    indices.clear();
    indices.reserve(stacks * sectors * 6);
    for (int i = 0; i < stacks; i++) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; j++)
        {
            if (i != 0)
                indices.push_back(k1 + j),
                indices.push_back(k2 + j),
                indices.push_back(k1 + j + 1);

            if (i != (stacks - 1))
                indices.push_back(k1 + j + 1),
                indices.push_back(k2 + j),
                indices.push_back(k2 + j + 1);
        }
    } 
    return;
}

void Sphere::generateBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    // Bind VBO to VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Fill VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(
        0,                                // location in shader
        3,                                // size (vec3 → 3 floats)
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        sizeof(Vertex),                   // stride (sizeof whole struct)
        (void*)offsetof(Vertex, Position) // offset in struct
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal)
    );
    glEnableVertexAttribArray(1);

    
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoords)
    );
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}