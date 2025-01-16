// Copyright 2024 Stone-Engine

#pragma once

#include <functional>
#include <memory>


namespace Stone::Render::OpenGL {

struct RendererSettings {
	std::pair<uint32_t, uint32_t> frame_size = {};
};

} // namespace Stone::Render::OpenGL
