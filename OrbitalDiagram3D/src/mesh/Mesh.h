#pragma once
#ifndef MESH_H
#define MESH_H
#include <vector>
#include "Vertex.h"
class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
private:
};
#endif 

