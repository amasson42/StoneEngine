// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Mesh.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

struct VRAMMesh {

	VRAMMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh, const std::shared_ptr<OpenGLRenderer> &renderer) {
		(void)renderer;
		if (mesh == nullptr) {
			return;
		}

		glGenVertexArrays(1, &elementsBuffer);
		if (elementsBuffer == 0) {
			throw std::runtime_error("Failed to generate vertex array buffer");
		}

		glGenBuffers(1, &verticesBuffer);
		if (verticesBuffer == 0) {
			glDeleteVertexArrays(1, &elementsBuffer);
			throw std::runtime_error("Failed to generate indices buffer");
		}

		glGenBuffers(1, &indicesBuffer);
		if (indicesBuffer == 0) {
			glDeleteBuffers(1, &verticesBuffer);
			glDeleteVertexArrays(1, &elementsBuffer);
			throw std::runtime_error("Failed to generate indices buffer");
		}

		glBindVertexArray(elementsBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->getVertices().size() * sizeof(Scene::Vertex), mesh->getVertices().data(),
					 GL_STATIC_DRAW);

		numIndices = mesh->getIndices().size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), mesh->getIndices().data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Scene::Vertex),
							  (void *)offsetof(Scene::Vertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Scene::Vertex), (void *)offsetof(Scene::Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Scene::Vertex),
							  (void *)offsetof(Scene::Vertex, tangent));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Scene::Vertex),
							  (void *)offsetof(Scene::Vertex, bitangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Scene::Vertex), (void *)offsetof(Scene::Vertex, uv));
	}

	~VRAMMesh() {
		if (verticesBuffer != 0) {
			glDeleteBuffers(1, &verticesBuffer);
		}
		if (indicesBuffer != 0) {
			glDeleteBuffers(1, &indicesBuffer);
		}
		if (elementsBuffer != 0) {
			glDeleteVertexArrays(1, &elementsBuffer);
		}
	}

	GLuint verticesBuffer = 0;
	GLuint indicesBuffer = 0;
	GLuint elementsBuffer = 0;
	GLsizei numIndices = 0;
};

class RendererMesh : public Scene::IRendererObject {

public:
	RendererMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _vramMesh(mesh, renderer) {
	}

	~RendererMesh() override {
	}

	const VRAMMesh &getVRAMMesh() const {
		return _vramMesh;
	}

private:
	VRAMMesh _vramMesh;
};


class DynamicMesh : public RendererMesh {
public:
	DynamicMesh(Scene::DynamicMesh &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: RendererMesh(std::static_pointer_cast<Scene::DynamicMesh>(mesh.shared_from_this()), renderer), _mesh(mesh),
		  _renderer(renderer) {
	}

	~DynamicMesh() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	Scene::DynamicMesh &_mesh;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

class StaticMesh : public RendererMesh {
public:
	StaticMesh(Scene::StaticMesh &mesh, const std::shared_ptr<OpenGLRenderer> &renderer)
		: RendererMesh(mesh.getSourceMesh(), renderer), _mesh(mesh), _renderer(renderer) {
	}

	~StaticMesh() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	Scene::StaticMesh &_mesh;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
