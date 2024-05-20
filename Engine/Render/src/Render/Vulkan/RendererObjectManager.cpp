// Copyright 2024 Stone-Engine

#include "RendererObjectManager.hpp"

#include "Device.hpp"
#include "Render/Vulkan/VulkanRenderer.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "Scene/RenderElement/Shader.hpp"
#include "Scene/RenderElement/Texture.hpp"
#include "VulkanRenderable/MeshNode.hpp"
#include "VulkanRenderable/Shader.hpp"
#include "VulkanRenderable/Texture.hpp"

namespace Stone::Render::Vulkan {

RendererObjectManager::RendererObjectManager(const std::shared_ptr<VulkanRenderer> &renderer)
	: Scene::RendererObjectManager(), _renderer(renderer) {
}

void RendererObjectManager::updateMeshNode(const std::shared_ptr<Scene::MeshNode> &meshNode) {
	Scene::RendererObjectManager::updateMeshNode(meshNode);

	if (meshNode->getRendererObject<Vulkan::MeshNode>()) {
		return;
	}

	auto newMeshNode = std::make_shared<Vulkan::MeshNode>(meshNode, _renderer);
	setRendererObjectTo(meshNode.get(), newMeshNode);
}

void RendererObjectManager::updateTexture(const std::shared_ptr<Scene::Texture> &texture) {
	Scene::RendererObjectManager::updateTexture(texture);

	if (texture->getRendererObject<Vulkan::Texture>()) {
		return;
	}

	auto newTexture = std::make_shared<Vulkan::Texture>(texture, _renderer);
	setRendererObjectTo(texture.get(), newTexture);
}

void RendererObjectManager::updateShader(const std::shared_ptr<Scene::Shader> &shader) {
	Scene::RendererObjectManager::updateShader(shader);

	if (shader->getRendererObject<Vulkan::Shader>()) {
		return;
	}

	auto newShader = std::make_shared<Vulkan::Shader>(shader, _renderer);
	setRendererObjectTo(shader.get(), newShader);
}

} // namespace Stone::Render::Vulkan
