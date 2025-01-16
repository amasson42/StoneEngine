// Copyright 2024 Stone-Engine

#include "Shader.hpp"

namespace Stone::Render::Vulkan {

Shader::Shader(const std::shared_ptr<Scene::FragmentShader> &shader, const std::shared_ptr<VulkanRenderer> &renderer) {
	(void)shader;
	(void)renderer;
}

void Shader::render(Scene::RenderContext &context) {
	(void)context;
}

} // namespace Stone::Render::Vulkan
