#ifndef SPHERE_H
#define SPHERE_H
#include <vector>
#include <memory>
#include "Vertex.h"
#include "Mesh.h"
#include "OrbitalDiagram3DExport.h"
#include "Body.h"
#include "RenderContext.h"
class OD_API Sphere: public Mesh {
public:
    static std::shared_ptr<Sphere> Instance();
    void bind() const override;
    std::size_t drawCount() const override;
    GLenum primitive() const override;
    void draw(const RenderContext& renderContext) const override;

private:
    const float PI = 3.14159265359f;
    Sphere();

    GLuint VAO, VBO, EBO;
    std::vector<Vertex3D> vertices;
    std::vector<unsigned int> indices;
    std::vector<Body> instances;
    void generateVertices(float stacks, float sectors);
	void generateIndices(float stacks, float sectors);
    void generateBuffers();
};
#endif