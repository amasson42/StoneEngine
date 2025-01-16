// Copyright 2024 Stone-Engine

#pragma once

#include "GBuffer.hpp"
#include "Scene/Renderer/RenderContext.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

class OpenGLRenderer;

struct RenderContext : public Scene::RenderContext {
	GBuffer *gBuffer;
};

} // namespace Stone::Render::OpenGL
