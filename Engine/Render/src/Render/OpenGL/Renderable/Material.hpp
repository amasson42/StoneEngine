// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Material.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Stone::Render::OpenGL {

class Material : public Scene::IRendererObject {
public:
	Material(Scene::Material &material, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _material(material), _renderer(renderer) {
	}

	~Material() override {
		if (_gl_meshProgram != 0)
			glDeleteProgram(_gl_meshProgram);
		if (_gl_skinMeshProgram != 0)
			glDeleteProgram(_gl_skinMeshProgram);
		if (_gl_instancedMeshProgram != 0)
			glDeleteProgram(_gl_instancedMeshProgram);
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	Scene::Material &_material;
	std::weak_ptr<OpenGLRenderer> _renderer;

	GLuint _gl_meshProgram = 0;
	GLuint _gl_skinMeshProgram = 0;
	GLuint _gl_instancedMeshProgram = 0;

	GLuint _lastUsedProgram = 0;
};

} // namespace Stone::Render::OpenGL
