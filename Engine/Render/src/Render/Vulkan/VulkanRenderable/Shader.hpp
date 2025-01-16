// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderable/IRenderable.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Scene {
class FragmentShader;
} // namespace Stone::Scene

namespace Stone::Render::Vulkan {

class VulkanRenderer;

class Shader : public Scene::IRendererObject {
public:
	Shader(const std::shared_ptr<Scene::FragmentShader> &shader, const std::shared_ptr<VulkanRenderer> &renderer);
	~Shader() override = default;

	void render(Scene::RenderContext &context) override;
};

} // namespace Stone::Render::Vulkan
