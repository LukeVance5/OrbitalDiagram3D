#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include <memory>
#include "Vertex.h"
#include "Mesh.h"
#include "RenderContext.h"
class Cube: public Mesh {
public:
	static std::shared_ptr<Cube> Instance();
	void bind() const override;
	std::size_t drawCount() const override;
	GLenum primitive() const override;
	void draw(const RenderContext& renderContext) const override;
private:
	GLuint VAO, VBO, EBO;
	std::vector<Vertex3D> vertices;
	std::vector<unsigned int> indices;
	Cube();
	Cube(const Cube&) = delete;
	Cube& operator= (const Cube&) = delete;
	void generateIndices();
	void generateVertices();
	void generateBuffers();
};
#endif