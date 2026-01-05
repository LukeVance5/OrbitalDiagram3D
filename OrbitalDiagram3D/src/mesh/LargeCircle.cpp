#include "LargeCircle.h"
#include "Shader.h"
#include "RenderContext.h"
std::shared_ptr<LargeCircle> LargeCircle::Instance() {
	const int SEGMENTS = 10000;
	static std::shared_ptr<LargeCircle> instance(new LargeCircle(SEGMENTS));
	return instance;
}

LargeCircle::LargeCircle(const int SEGMENTS) {
    generateVertices(SEGMENTS);
    generateBuffers();
}


void LargeCircle::addInstances(const std::vector<OrbitData>& instanceData) {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        instanceData.size() * sizeof(OrbitData),
        instanceData.data(),
        GL_DYNAMIC_DRAW
    );
    this->instanceCount = instanceData.size();
}

void LargeCircle::bind() const {
    glBindVertexArray(VAO);
}

void LargeCircle::draw(const RenderContext& renderContext) const {
    bind();
    std::shared_ptr<Shader> shader = renderContext.shader;

    shader->activate();
    shader->setMat4("uView", glm::mat4(glm::mat3(renderContext.camera->getViewMatrix())));
    shader->setMat4("uProj", renderContext.projection);
    shader->set2Float("uViewportSize", renderContext.screenWidth, renderContext.screenHeight);
    shader->setFloat("uLineWidthPx", 2.5f);

    glDrawArraysInstanced(
        GL_LINE_STRIP,
        0,
        this->vertices.size(),
        static_cast<GLsizei>(instanceCount)
    );

    glBindVertexArray(0);
}

std::size_t LargeCircle::drawCount() const {
    return vertices.size();
}

GLenum LargeCircle::primitive() const {
    return GL_LINE_STRIP;
}

void LargeCircle::generateVertices(const int SEGMENTS) {
    vertices.clear();
    vertices.reserve(SEGMENTS + 1); // line pairs for GL_LINES

    for (int i = 0; i <= SEGMENTS; ++i) {
        float a = float(i) / float(SEGMENTS) * 2.0f * PI;
        Vertex2D v{};
        v.position = glm::vec2(cos(a), sin(a));
        vertices.push_back(v);
    }
}


void LargeCircle::generateBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);

    // --------------------
    // Per-vertex buffer
    // --------------------
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex2D),
        vertices.data(),
        GL_STATIC_DRAW
    );

    // location 0 : vec2 (cosν, sinν)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex2D),
        (void*)offsetof(Vertex2D, position)
    );

    // --------------------
    // Per-instance buffer
    // --------------------
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    // mat4 model → locations 1,2,3,4
    for (int i = 0; i < 4; ++i) {
        glEnableVertexAttribArray(1 + i);
        glVertexAttribPointer(
            1 + i,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(OrbitData),
            (void*)(offsetof(OrbitData, model) + sizeof(glm::vec4) * i)
        );
        glVertexAttribDivisor(1 + i, 1);
    }

    // location 5 : semi-major axis
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(
        5,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(OrbitData),
        (void*)offsetof(OrbitData, semimajorAxis)
    );
    glVertexAttribDivisor(5, 1);

    // location 6 : eccentricity
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(
        6,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(OrbitData),
        (void*)offsetof(OrbitData, eccentricity)
    );
    glVertexAttribDivisor(6, 1);
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(
        7,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(OrbitData),
        (void*)offsetof(OrbitData, v)
    );
    glVertexAttribDivisor(7, 1);

    glEnableVertexAttribArray(8);
    glVertexAttribPointer(
        8,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(OrbitData),
        (void*)offsetof(OrbitData, color)
    );
    glVertexAttribDivisor(8, 1);
    glBindVertexArray(0);
}
