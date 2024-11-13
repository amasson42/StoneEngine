// Copyright 2024 Stone-Engine

#pragma once

#include "../ShaderCollection.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Shader.hpp"

namespace Stone::Render::OpenGL {

class FragmentShader : public Scene::IRendererObject {
public:
	FragmentShader(Scene::FragmentShader &fragmentShader, const std::shared_ptr<OpenGLRenderer> &renderer) {
		_shaderCollection = std::make_shared<ShaderCollection>(fragmentShader, renderer->getOpenGLResources());
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

	const std::shared_ptr<ShaderCollection> &getShaderCollection() const {
		return _shaderCollection;
	}

private:
	std::shared_ptr<ShaderCollection> _shaderCollection;
};

} // namespace Stone::Render::OpenGL
