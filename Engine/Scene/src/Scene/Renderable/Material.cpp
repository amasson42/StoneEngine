// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Material.hpp"

#include "Scene/Renderable/Texture.hpp"
#include "Scene/Renderer/RendererObjectManager.hpp"
#include "Utils/Glm.hpp"

namespace Stone::Scene {

void Material::writeToJson(Json::Object &json) const {
	Object::writeToJson(json);

	auto &textures((json["textures"] = Json::object()).get<Json::Object>());
	for (auto &it : _textures)
		textures[location_to_string(it.first)] = it.second ? Json::number(it.second->getId()) : Json::null();

	auto &vectors((json["vectors"] = Json::object()).get<Json::Object>());
	for (auto &it : _vectors)
		vectors[location_to_string(it.first)] = to_json(it.second);

	auto &scalars((json["scalars"] = Json::object()).get<Json::Object>());
	for (auto &it : _scalars)
		scalars[location_to_string(it.first)] = Json::number(it.second);
}

void Material::setTextureParameter(const Location &location, std::shared_ptr<Texture> texture) {
	_textures[location] = std::move(texture);
	markDirty();
}

std::shared_ptr<Texture> Material::getTextureParameter(const Location &location) const {
	auto it = _textures.find(location);
	if (it != _textures.end()) {
		return it->second;
	}
	return nullptr;
}

void Material::setVectorParameter(const Location &location, const glm::vec3 &vector) {
	_vectors[location] = vector;
	markDirty();
}

glm::vec3 Material::getVectorParameter(const Location &location) const {
	auto it = _vectors.find(location);
	if (it != _vectors.end()) {
		return it->second;
	}
	return glm::vec3(0.0f);
}

void Material::setScalarParameter(const Location &location, float scalar) {
	_scalars[location] = scalar;
	markDirty();
}

float Material::getScalarParameter(const Location &location) const {
	auto it = _scalars.find(location);
	if (it != _scalars.end()) {
		return it->second;
	}
	return 0.0f;
}

void Material::forEachTextures(
	const std::function<void(const Location &, const std::shared_ptr<Texture> &)> &lambda) const {
	for (const auto &it : _textures) {
		lambda(it.first, it.second);
	}
}

void Material::forEachVectors(const std::function<void(const Location &, glm::vec3)> &lambda) const {
	for (const auto &it : _vectors) {
		lambda(it.first, it.second);
	}
}

void Material::forEachScalars(const std::function<void(const Location &, float)> &lambda) const {
	for (const auto &it : _scalars) {
		lambda(it.first, it.second);
	}
}

void Material::setFragmentShader(std::shared_ptr<FragmentShader> fragmentShader) {
	_fragmentShader = std::move(fragmentShader);
	markDirty();
}

const std::shared_ptr<FragmentShader> &Material::getFragmentShader() const {
	return _fragmentShader;
}

std::string location_to_string(const Material::Location &location) {
	if (std::holds_alternative<std::string>(location)) {
		return std::get<std::string>(location);
	} else {
		return std::to_string(std::get<int>(location));
	}
}

Material::Location string_to_location(const std::string &str) {
	if (str.empty()) {
		return 0;
	}
	if (str[0] >= '0' && str[0] <= '9') {
		return std::stoi(str);
	}
	return str;
}

} // namespace Stone::Scene
