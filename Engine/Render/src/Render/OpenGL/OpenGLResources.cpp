// Copyright 2024 Stone-Engine

#include "Render/OpenGL/OpenGLResources.hpp"

#include "Render/OpenGL/OpenGLRenderer.hpp"

namespace Stone::Render::OpenGL {

OpenGLResources::OpenGLResources(const std::shared_ptr<OpenGLRenderer> &renderer) : _renderer(renderer) {
}


} // namespace Stone::Render::OpenGL
