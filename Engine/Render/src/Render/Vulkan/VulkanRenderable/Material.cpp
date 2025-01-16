// Copyright 2024 Stone-Engine

#include "Material.hpp"

namespace Stone::Render::Vulkan {

Material::Material(const std::shared_ptr<Scene::Material> &material, const std::shared_ptr<VulkanRenderer> &renderer) {
	(void)material;
	(void)renderer;
}

void Material::render(Scene::RenderContext &context) {
	(void)context;
}

} // namespace Stone::Render::Vulkan
