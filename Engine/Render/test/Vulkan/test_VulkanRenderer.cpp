/*
#include "Render/Vulkan/RendererSettings.hpp"
#include "Render/Vulkan/VulkanRenderer.hpp"

#include <gtest/gtest.h>

using namespace Stone::Render::Vulkan;
using namespace Stone::Scene;

TEST(VulkanRender, InstanciateCore) {
	RendererSettings settings;

	try {
		VulkanRenderer renderer(settings);
	} catch (const std::runtime_error &e) {
		if (std::string(e.what()) == "Failed to create window surface !") {
			SUCCEED();
		} else {
			FAIL() << e.what();
		}
	} catch (const std::exception &e) {
		FAIL() << e.what();
	}
}
*/
