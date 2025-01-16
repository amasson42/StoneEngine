// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderer/RenderContext.hpp"

#include <vulkan/vulkan.h>

namespace Stone::Render::Vulkan {

struct RenderContext : public Scene::RenderContext {
	VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
	VkExtent2D extent = {};
	uint32_t imageIndex = 0;
};

} // namespace Stone::Render::Vulkan
