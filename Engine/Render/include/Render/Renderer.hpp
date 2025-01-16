// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderer/ISceneRenderer.hpp"

#include <memory>

namespace Stone::Render {

class Renderer : public std::enable_shared_from_this<Renderer>, public Scene::ISceneRenderer {
public:
	Renderer() = default;
	Renderer(const Renderer &) = delete;

	virtual ~Renderer() = default;

	virtual void updateFrameSize(std::pair<uint32_t, uint32_t> size) = 0;
};

} // namespace Stone::Render
