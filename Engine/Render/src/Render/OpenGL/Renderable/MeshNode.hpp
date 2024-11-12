// Copyright 2024 Stone-Engine

#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "Scene/Renderable/Mesh.hpp"
#include "Scene/Renderer/RendererDefaults.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class MeshNode : public Scene::IRendererObject {
public:
	MeshNode(Scene::MeshNode &meshNode, const std::shared_ptr<OpenGLRenderer> &renderer)
		: _meshNode(meshNode), _renderer(renderer) {

		std::shared_ptr<Scene::Material> usedMaterial =
			meshNode.getMaterial() ? meshNode.getMaterial()
			: meshNode.getMesh() != nullptr && meshNode.getMesh()->getDefaultMaterial() != nullptr
				? meshNode.getMesh()->getDefaultMaterial()
				: nullptr;

		usedMaterial->getRendererObject<Material>()->makeMeshProgram();
	}

	~MeshNode() override = default;

	void render(Scene::RenderContext &context) override {
		auto mesh = _meshNode.getMesh();
		if (mesh == nullptr)
			return;

		auto rendererMesh = mesh->getRendererObject<RendererMesh>();
		if (rendererMesh == nullptr) {
			return;
		}
		const VRAMMesh &vramMesh = rendererMesh->getVRAMMesh();

		std::shared_ptr<Scene::Material> sceneMaterial = _meshNode.getMaterial()	  ? _meshNode.getMaterial()
														 : mesh->getDefaultMaterial() ? mesh->getDefaultMaterial()
																					  : nullptr;

		assert(sceneMaterial != nullptr);
		assert(sceneMaterial->isDirty() == false);

		std::shared_ptr<Material> material = sceneMaterial->getRendererObject<Material>();

		material->useMeshProgram();
		material->render(context);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glBindVertexArray(vramMesh.elementsBuffer);
		glDrawElements(GL_TRIANGLES, vramMesh.numIndices, GL_UNSIGNED_INT, 0);
	}

private:
	Scene::MeshNode &_meshNode;
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
