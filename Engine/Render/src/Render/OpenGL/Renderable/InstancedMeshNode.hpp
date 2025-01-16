// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Node/InstancedMeshNode.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class InstancedMeshNode : public Scene::IRendererObject {
public:
	InstancedMeshNode(Scene::InstancedMeshNode &instancedMeshNode, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _instancedMeshNode(instancedMeshNode), _renderer(renderer) {
	}

	~InstancedMeshNode() override {
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

private:
	Scene::InstancedMeshNode &_instancedMeshNode;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
