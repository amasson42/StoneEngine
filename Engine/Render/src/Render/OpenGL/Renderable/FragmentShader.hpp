// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Shader.hpp"

namespace Stone::Render::OpenGL {

class FragmentShader : public Scene::IRendererObject {
public:
	FragmentShader(Scene::FragmentShader &fragmentShader, const std::shared_ptr<OpenGLRenderer> &renderer) {
		(void)fragmentShader;
		(void)renderer;
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
};

} // namespace Stone::Render::OpenGL
