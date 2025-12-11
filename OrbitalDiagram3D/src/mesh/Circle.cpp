#include "Circle.h"
#include <glad/glad.h>

std::shared_ptr<Circle> Circle::Instance() {
	static std::shared_ptr<Circle> instance = std::shared_ptr<Circle>(new Circle());
	return instance;
}

Circle::Circle() {
    const int SEGMENTS = 64;
	generateVertices(SEGMENTS);
	generateBuffers();
}



void Circle::generateVertices(const int SEGMENTS) {
    const float radius = 1.0f;

    vertices.clear();
    vertices.reserve(SEGMENTS + 1);

    // Ring
    for (int i = 0; i <= SEGMENTS; i++) {
        float angle = (float)i / (float)SEGMENTS * 2.0f * PI;

        float x = cos(angle);
        float y = sin(angle);

        Vertex v;
        v.Position = glm::vec3(x * radius, y * radius, 0.0f);
        v.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
        v.TexCoords = glm::vec2((x * 0.5f) + 0.5f, (y * 0.5f) + 0.5f);

        vertices.push_back(v);
    }
}

void Circle::generateBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(VertexTraj),
        vertices.data(),
        GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTraj), (void*)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTraj),
        (void*)offsetof(VertexTraj, Color));

    glBindVertexArray(0);
}
