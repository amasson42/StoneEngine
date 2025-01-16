// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderer/RendererObjectManager.hpp"

namespace Stone::Render::OpenGL {

class OpenGLRenderer;

class RendererObjectManager : public Scene::RendererObjectManager {

public:
	explicit RendererObjectManager(std::shared_ptr<OpenGLRenderer> renderer);
	RendererObjectManager(const RendererObjectManager &other) = default;

	~RendererObjectManager() override = default;

	void updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) override;
	void updateInstancedMeshNode(const std::shared_ptr<Scene::InstancedMeshNode> &instancedMeshNode) override;
	void updateSkinMeshNode(const std::shared_ptr<Scene::SkinMeshNode> &skinMeshNode) override;
	void updateMaterial(const std::shared_ptr<Scene::Material> &material) override;
	void updateDynamicMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh) override;
	void updateStaticMesh(const std::shared_ptr<Scene::StaticMesh> &mesh) override;
	void updateDynamicSkinMesh(const std::shared_ptr<Scene::DynamicSkinMesh> &skinmesh) override;
	void updateStaticSkinMesh(const std::shared_ptr<Scene::StaticSkinMesh> &skinmesh) override;
	void updateTexture(const std::shared_ptr<Scene::Texture> &texture) override;
	void updateFragmentShader(const std::shared_ptr<Scene::FragmentShader> &shader) override;


private:
	std::shared_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
