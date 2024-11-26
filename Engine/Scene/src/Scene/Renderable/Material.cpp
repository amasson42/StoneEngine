// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Material.hpp"

#include "Scene/Renderable/Texture.hpp"
#include "Scene/RendererObjectManager.hpp"
#include "Utils/Glm.hpp"

namespace Stone::Scene {

void Material::writeToJson(Json::Object &json) const {
	Object::writeToJson(json);

	auto &textures((json["textures"] = Json::object()).get<Json::Object>());
	for (auto &it : _textures)
		textures[it.first] = it.second ? Json::number(it.second->getId()) : Json::null();

	auto &vectors((json["vectors"] = Json::object()).get<Json::Object>());
	for (auto &it : _vectors)
		vectors[it.first] = to_json(it.second);

	auto &scalars((json["scalars"] = Json::object()).get<Json::Object>());
	for (auto &it : _scalars)
		scalars[it.first] = Json::number(it.second);
}

void Material::setTextureParameter(const std::string &name, std::shared_ptr<Texture> texture) {
	_textures[name] = std::move(texture);
	markDirty();
}

std::shared_ptr<Texture> Material::getTextureParameter(const std::string &name) const {
	auto it = _textures.find(name);
	if (it != _textures.end()) {
		return it->second;
	}
	return nullptr;
}

void Material::setVectorParameter(const std::string &name, const glm::vec3 &vector) {
	_vectors[name] = vector;
	markDirty();
}

glm::vec3 Material::getVectorParameter(const std::string &name) const {
	auto it = _vectors.find(name);
	if (it != _vectors.end()) {
		return it->second;
	}
	return glm::vec3(0.0f);
}

void Material::setScalarParameter(const std::string &name, float scalar) {
	_scalars[name] = scalar;
	markDirty();
}

float Material::getScalarParameter(const std::string &name) const {
	auto it = _scalars.find(name);
	if (it != _scalars.end()) {
		return it->second;
	}
	return 0.0f;
}

void Material::forEachTextures(
	const std::function<void(std::pair<const std::string, std::shared_ptr<Texture>> &)> &lambda) {
	for (auto &it : _textures) {
		lambda(it);
	}
}

void Material::forEachVectors(const std::function<void(std::pair<const std::string, glm::vec3> &)> &lambda) {
	for (auto &it : _vectors) {
		lambda(it);
	}
}

void Material::forEachScalars(const std::function<void(std::pair<const std::string, float> &)> &lambda) {
	for (auto &it : _scalars) {
		it.second += 1;
		lambda(it);
	}
}

void Material::setVertexShader(std::shared_ptr<Shader> vertexShader) {
	_vertexShader = std::move(vertexShader);
	markDirty();
}

std::shared_ptr<Shader> Material::getVertexShader() const {
	return _vertexShader;
}

void Material::setFragmentShader(std::shared_ptr<Shader> fragmentShader) {
	_fragmentShader = std::move(fragmentShader);
	markDirty();
}

std::shared_ptr<Shader> Material::getFragmentShader() const {
	return _fragmentShader;
}

} // namespace Stone::Scene
