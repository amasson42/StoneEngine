// Copyright 2024 Stone-Engine

#include "RendererObjectManager.hpp"

#include "Device.hpp"
#include "Render/Vulkan/VulkanRenderer.hpp"
#include "Scene/Node/MeshNode.hpp"
#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/Mesh.hpp"
#include "Scene/Renderable/Shader.hpp"
#include "Scene/Renderable/Texture.hpp"
#include "VulkanRenderable/Material.hpp"
#include "VulkanRenderable/Mesh.hpp"
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
	updateRendererObject(*meshNode, newMeshNode);
}

void RendererObjectManager::updateMaterial(const std::shared_ptr<Scene::Material> &material) {
	Scene::RendererObjectManager::updateMaterial(material);

	if (material->getRendererObject<Vulkan::Material>()) {
		return;
	}

	auto newMaterial = std::make_shared<Vulkan::Material>(material, _renderer);
	updateRendererObject(*material, newMaterial);
}

void RendererObjectManager::updateDynamicMesh(const std::shared_ptr<Scene::DynamicMesh> &mesh) {
	Scene::RendererObjectManager::updateDynamicMesh(mesh);

	if (mesh->getRendererObject<Vulkan::Mesh>()) {
		return;
	}

	auto newMesh = std::make_shared<Vulkan::Mesh>(mesh, _renderer);
	updateRendererObject(*mesh, newMesh);
}

void RendererObjectManager::updateTexture(const std::shared_ptr<Scene::Texture> &texture) {
	Scene::RendererObjectManager::updateTexture(texture);

	if (texture->getRendererObject<Vulkan::Texture>()) {
		return;
	}

	auto newTexture = std::make_shared<Vulkan::Texture>(texture, _renderer);
	updateRendererObject(*texture, newTexture);
}

void RendererObjectManager::updateFragmentShader(const std::shared_ptr<Scene::FragmentShader> &shader) {
	Scene::RendererObjectManager::updateFragmentShader(shader);

	if (shader->getRendererObject<Vulkan::Shader>()) {
		return;
	}

	auto newShader = std::make_shared<Vulkan::Shader>(shader, _renderer);
	updateRendererObject(*shader, newShader);
}

} // namespace Stone::Render::Vulkan
