// Copyright 2024 Stone-Engine

#pragma once

#include "SceneTypes.hpp"

namespace Stone::Scene {

/**
 * @brief Default values for a renderer.
 */
class RendererDefaults {
public:
	RendererDefaults();
	RendererDefaults(const RendererDefaults &) = delete;

	virtual ~RendererDefaults() = default;
};

} // namespace Stone::Scene
