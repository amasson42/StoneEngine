// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Node/SkinMeshNode.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class SkinMeshNode : public Scene::IRendererObject {
public:
	SkinMeshNode(Scene::SkinMeshNode &skinMeshNode, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _skinMeshNode(skinMeshNode), _renderer(renderer) {
	}

	~SkinMeshNode() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	Scene::SkinMeshNode &_skinMeshNode;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
