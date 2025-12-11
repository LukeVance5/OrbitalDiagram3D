#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include <vector>
#include <memory>
#include "Vertex.h"
#include "Mesh.h"

class Circle : public Mesh {
public:
	static std::shared_ptr<Circle> Instance();
private:
	const float PI = 3.14159265359f;
	Circle();
	Circle(const Circle&) = delete;
	Circle& operator= (const Circle&) = delete;
	void generateVertices(const int SEGMENTS);
	void generateBuffers();
};
#endif