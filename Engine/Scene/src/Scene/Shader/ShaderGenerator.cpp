// Copyright 2024 Stone-Engine

#include "Scene/Shader/ShaderGenerator.hpp"

namespace Stone::Scene {


void ShaderGenerator::generateFragmentShader(const ShaderParameters &params, std::ostream &output) {
	std::ostream &source = output;

	source << R"(#version 400 core

in FRAG_DATA {
	vec3 wposition;
	vec2 uv;
	vec3 wnormal;
	vec3 wtangent;
	vec3 wbitangent;
} fs_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

)";

	auto add_uniform_param = [&source](const char *name, ShaderParameters::Type type) {
		if (type == ShaderParameters::Type::Texture) {
			source << "uniform sampler2D " << name << ";" << std::endl;
		} else if (type == ShaderParameters::Type::Vector) {
			source << "uniform vec3 " << name << ";" << std::endl;
		} else if (type == ShaderParameters::Type::Scalar) {
			source << "uniform float " << name << ";" << std::endl;
		}
	};

#define __ADD_UNIFORM_PARAM(PARAM) add_uniform_param(#PARAM, params.PARAM);

	FOR_EACH_SHADER_PARAMETERS(__ADD_UNIFORM_PARAM);

	source << R"(
void main() {
	gPosition = fs_in.wposition;
	gNormal = normalize(fs_in.wnormal);
	gAlbedoSpec = vec4(1.0, 0.0, 0.0, 1.0);
}
)";
}


} // namespace Stone::Scene
