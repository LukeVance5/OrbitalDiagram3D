#ifndef SPHERE_H
#define SPHERE_H
#include <vector>
#include <memory>
#include "Vertex.h"
#include "Mesh.h"


class Sphere: public Mesh {
	public:
		static std::shared_ptr<Sphere> Instance();
	private:
		const float PI = 3.14159265359f;
		Sphere();
		Sphere(const Sphere&) = delete;
		Sphere& operator= (const Sphere&) = delete;
		void generateIndices(float stacks, float sectors);
		void generateVertices(float stacks, float sectors);
		void generateBuffers();
}; 
#endif