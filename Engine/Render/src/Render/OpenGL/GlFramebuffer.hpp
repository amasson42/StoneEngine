// Copyright 2024 Stone-Engine

#pragma once

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class GlFramebufferMesh {
public:
	GlFramebufferMesh() {
		float vertices[] = {
			// positions          // texture coords
			1.0f,  1.0f,  1.0f, 1.0f, // top right
			1.0f,  -1.0f, 1.0f, 0.0f, // bottom right
			-1.0f, -1.0f, 0.0f, 0.0f, // bottom left
			-1.0f, 1.0f,  0.0f, 1.0f  // top left
		};

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3	 // second triangle
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	virtual ~GlFramebufferMesh() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}

	void draw() {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	GLuint vbo;
	GLuint vao;
	GLuint ebo;
};


} // namespace Stone::Render::OpenGL
