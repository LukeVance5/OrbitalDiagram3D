#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include <vector>
#include <memory>
#include "Vertex.h"
#include "Mesh.h"
#include "OrbitalData.h"

struct RenderContext;
class Circle : public Mesh {
public:
	static std::shared_ptr<Circle> Instance();
	void bind() const override;
	std::size_t drawCount() const override;
	GLenum primitive() const override;
	void draw(const RenderContext& renderContext) const override;
	void addInstances(const std::vector<OrbitData>& instanceData);
private:
	GLuint VAO, VBO, instanceVBO;
	const float PI = 3.14159265359f;
	std::vector<Vertex2D> vertices;
	Circle();
	Circle(const Circle&) = delete;
	Circle& operator= (const Circle&) = delete;
	std::size_t instanceCount = 0;
	void generateVertices(const int SEGMENTS);
	void generateBuffers();
};
#endif