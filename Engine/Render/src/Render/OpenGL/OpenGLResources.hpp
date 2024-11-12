// Copyright 2024 Stone-Engine

#pragma once

#include <memory>

namespace Stone::Render::OpenGL {

class OpenGLRenderer;

class OpenGLResources {
public:
	OpenGLResources() = default;

	OpenGLResources(const std::shared_ptr<OpenGLRenderer> &renderer);

	virtual ~OpenGLResources() = default;

private:
	std::weak_ptr<OpenGLRenderer> _renderer;
};

} // namespace Stone::Render::OpenGL
