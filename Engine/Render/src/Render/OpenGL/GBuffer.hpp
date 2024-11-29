// Copyright 2024 Stone-Engine

#pragma once

#include "GlFramebuffer.hpp"
#include "GlShaders.hpp"
#include "Renderable/Texture.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

struct GBuffer {

	GBuffer(unsigned int width, unsigned int height) : width(width), height(height) {
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		gPosition = std::make_unique<Texture>(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition->getGlTexture(), 0);

		gNormal = std::make_unique<Texture>(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal->getGlTexture(), 0);

		gAlbedoSpec =
			std::make_unique<Texture>(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec->getGlTexture(), 0);

		unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3, attachments);

		GlVertexShader vertexShader(R"(#version 400 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out vec2 fragUV;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	fragUV = uv;
}

)");

		GlFragmentShader fragmentShader(R"(#version 400 core

in vec2 fragUV;

uniform sampler2D u_position;
uniform sampler2D u_normal;
uniform sampler2D u_albedo_spec;

out vec4 FragColor;

void main() {
	FragColor = vec4(fragUV, 1.0, 1.0);
}

)");

		program = std::make_shared<GlShaderProgram>(vertexShader, fragmentShader);

		frameMesh = std::make_shared<GlFramebufferMesh>();
	}

	virtual ~GBuffer() {
		glDeleteFramebuffers(1, &gBuffer);
	}

	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	}

	void render() {
		program->use();

		program->setUniformTexture("u_position", *gPosition, 0);
		program->setUniformTexture("u_normal", *gNormal, 1);
		program->setUniformTexture("u_albedo_spec", *gAlbedoSpec, 2);

		frameMesh->draw();
	}

	unsigned int width;
	unsigned int height;

	GLuint gBuffer;

	std::unique_ptr<Texture> gPosition;
	std::unique_ptr<Texture> gNormal;
	std::unique_ptr<Texture> gAlbedoSpec;


	std::shared_ptr<GlFramebufferMesh> frameMesh;

	std::shared_ptr<GlShaderProgram> program;
};

} // namespace Stone::Render::OpenGL
