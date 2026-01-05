#pragma once
#ifndef LARGECIRCLE_H
#define LARGECIRCLE_H
#include <vector>
#include <memory>
#include "Vertex.h"
#include "Mesh.h"
#include "OrbitalData.h"

struct RenderContext;
class LargeCircle : public Mesh {
public:
	static std::shared_ptr<LargeCircle> Instance();
	void bind() const override;
	std::size_t drawCount() const override;
	GLenum primitive() const override;
	void draw(const RenderContext& renderContext) const override;
	void addInstances(const std::vector<OrbitData>& instanceData);
private:
	GLuint VAO, VBO, instanceVBO;
	const float PI = 3.14159265359f;
	std::vector<Vertex2D> vertices;
	LargeCircle(const int SEGMENTS);
	LargeCircle(const LargeCircle&) = delete;
	LargeCircle& operator= (const LargeCircle&) = delete;
	std::size_t instanceCount = 0;
	void generateVertices(const int SEGMENTS);
	void generateBuffers();
};
#endif