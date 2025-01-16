/*
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Render/OpenGL/RendererSettings.hpp"

#include <gtest/gtest.h>

using namespace Stone::Render::OpenGL;
using namespace Stone::Scene;

TEST(OpenGLRender, InstanciateCore) {
	RendererSettings settings;

	try {
		OpenGLRenderer renderer(settings);
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
