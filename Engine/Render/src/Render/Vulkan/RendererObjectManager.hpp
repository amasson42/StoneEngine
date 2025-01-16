// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderer/RendererObjectManager.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

class VulkanRenderer;

class RendererObjectManager : public Scene::RendererObjectManager {

public:
	RendererObjectManager(const std::shared_ptr<VulkanRenderer> &renderer);
	RendererObjectManager(const RendererObjectManager &other) = default;
	~RendererObjectManager() override = default;


	void updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) override;

	// void updateInstancedMeshNode(const std::shared_ptr<Scene::InstancedMeshNode> &instancedMeshNode) override;

	// void updateSkinMeshNode(const std::shared_ptr<Scene::SkinMeshNode> &skinMeshNode) override;

	void updateMaterial(const std::shared_ptr<Scene::Material> &material) override;

	void updateDynamicMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh) override;

	// void updateSkinMesh(const std::shared_ptr<Scene::SkinMesh> &skinmesh) override;

	void updateTexture(const std::shared_ptr<Scene::Texture> &texture) override;

	void updateFragmentShader(const std::shared_ptr<Scene::FragmentShader> &shader) override;

private:
	std::shared_ptr<VulkanRenderer> _renderer;
};

} // namespace Stone::Render::Vulkan
