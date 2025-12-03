#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include <memory>
#include "Vertex.h"
#include "Mesh.h"

class Cube: public Mesh {
public:
	static std::shared_ptr<Cube> Instance();
private:
	Cube();
	Cube(const Cube&) = delete;
	Cube& operator= (const Cube&) = delete;
	void generateIndices();
	void generateVertices();
	void generateBuffers();
};
#endif