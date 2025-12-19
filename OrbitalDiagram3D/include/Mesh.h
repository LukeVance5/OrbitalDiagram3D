#pragma once
#ifndef IMESH_H
#define IMESH_H

#include <glad/glad.h>
#include <cstddef>

struct RenderContext; // forward declaration

class Mesh {
public:
    virtual ~Mesh() = default;

    // Bind VAO (and any mesh-owned state)
    virtual void bind() const = 0;

    // Number of vertices or indices to draw
    virtual std::size_t drawCount() const = 0;

    // OpenGL primitive type (GL_TRIANGLES, GL_LINES, etc.)
    virtual GLenum primitive() const = 0;

    // Perform the draw (instanced or not)
    virtual void draw(const RenderContext& ctx) const = 0;
};

#endif
