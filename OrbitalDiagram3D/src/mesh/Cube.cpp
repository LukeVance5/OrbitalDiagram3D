#include "Cube.h"
#include <memory>
#include <glad/glad.h>
	std::shared_ptr<Cube> Cube::Instance() {
	static std::shared_ptr<Cube> instance = std::shared_ptr<Cube>(new Cube());
	return instance;
	}

	Cube::Cube() {
		generateVertices();
		generateIndices();
		generateBuffers();
	}

	void Cube ::bind() const {
		glBindVertexArray(VAO);
	}

	std::size_t Cube::drawCount() const {
		return indices.size();
	}

	GLenum Cube::primitive() const {
		return GL_TRIANGLES;
	}
	//!!! Draw call for cube
	void Cube::draw(const RenderContext& renderContext) const {
		std::shared_ptr<Shader> shader = renderContext.shader;
		shader->activate();
		shader->setMat4("view", renderContext.view);
		shader->setMat4("projection", renderContext.projection);
		shader->setInt("skybox", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, renderContext.textureID);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



		glBindVertexArray(0);
	}
	void Cube::generateIndices() {
		vertices.clear();
		vertices.reserve(8);
		float skyboxVertices[] =
		{
			//   Coordinates
			-1.0f, -1.0f,  1.0f,//        7--------6
			 1.0f, -1.0f,  1.0f,//       /|       /|
			 1.0f, -1.0f, -1.0f,//      4--------5 |
			-1.0f, -1.0f, -1.0f,//      | |      | |
			-1.0f,  1.0f,  1.0f,//      | 3------|-2
			 1.0f,  1.0f,  1.0f,//      |/       |/
			 1.0f,  1.0f, -1.0f,//      0--------1
			-1.0f,  1.0f, -1.0f
		};
		for (int i = 0; i < 8; i++) {
			Vertex3D vertex{};
			vertex.Position = glm::vec3(
				skyboxVertices[i * 3 + 0],
				skyboxVertices[i * 3 + 1],
				skyboxVertices[i * 3 + 2]
			);
			vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
		return;
	}
	void Cube::generateVertices() {
		indices.clear();
		indices.reserve(36);
		unsigned int skyboxIndices[] =
		{
			// Right
			1, 2, 6,
			6, 5, 1,
			// Left
			0, 4, 7,
			7, 3, 0,
			// Top
			4, 5, 6,
			6, 7, 4,
			// Bottom
			0, 3, 2,
			2, 1, 0,
			// Back
			0, 1, 5,
			5, 4, 0,
			// Front
			3, 7, 6,
			6, 2, 3
		};
		for (int i = 0; i < sizeof(skyboxIndices) / sizeof(skyboxIndices[0]); i++) {
			indices.push_back(skyboxIndices[i]);
		}
		return;
	}
	void Cube::generateBuffers() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		// Bind VBO to VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Fill VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(
			0,                                // location in shader
			3,                                // size (vec3 → 3 floats)
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			sizeof(Vertex3D),                   // stride (sizeof whole struct)
			(void*)offsetof(Vertex3D, Position) // offset in struct
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		return;
	}