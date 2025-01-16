// Copyright 2024 Stone-Engine

#pragma once

#include "../RenderContext.hpp"
#include "Scene/Renderable/IRenderable.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace Stone::Scene {
class DynamicMesh;
} // namespace Stone::Scene

namespace Stone::Render::Vulkan {

class VulkanRenderer;
class Device;
class RenderPass;
class SwapChain;

class Mesh : public Scene::IRendererObject {
public:
	Mesh(const std::shared_ptr<Scene::DynamicMesh> &mesh, const std::shared_ptr<VulkanRenderer> &renderer);

	~Mesh() override = default;

	void render(Scene::RenderContext &context) override;
};

} // namespace Stone::Render::Vulkan
