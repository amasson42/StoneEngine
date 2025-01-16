// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Shader/ShaderParameters.hpp"

namespace Stone::Scene {

class ShaderGenerator {


public:
	ShaderGenerator() = default;

	~ShaderGenerator() = default;

	void generateFragmentShader(const ShaderParameters &params, std::ostream &output);
};

} // namespace Stone::Scene
