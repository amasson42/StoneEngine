// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderable/IMeshObject.hpp"

namespace Stone::Scene {

#define FOR_EACH_SHADER_PARAMETERS(__WithParam)                                                                        \
	__WithParam(diffuse) __WithParam(specular) __WithParam(ambient) __WithParam(emissive) __WithParam(shininess)       \
		__WithParam(opacity) __WithParam(roughness) __WithParam(metallic) __WithParam(normal) __WithParam(occlusion)   \
			__WithParam(height)

struct ShaderParameters {

	enum class Type : uint8_t {
		None = 0,
		Scalar,
		Vector,
		Texture,
	};

	union {
		struct {
#define __DECLARE_PARAM(param) Type param : 2;
			FOR_EACH_SHADER_PARAMETERS(__DECLARE_PARAM)
		};
		uint32_t data; // sizeof() should be greater or equal to the struct size
	};
	bool _;

#define __INITIALIZE_PARAM(param) param(Type::None),
	ShaderParameters();

	void setParamWithName(const std::string &name, Type value);

	void setFromMaterial(const Scene::Material &material);

	bool operator==(const ShaderParameters &other) const {
		return data == other.data;
	}
};

} // namespace Stone::Scene

namespace std {
template <>
struct hash<Stone::Scene::ShaderParameters> {
	std::size_t operator()(const Stone::Scene::ShaderParameters &params) const noexcept {
		return std::hash<uint32_t>()(params.data);
	}
};
} // namespace std
