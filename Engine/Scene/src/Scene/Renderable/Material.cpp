// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Material.hpp"

#include "Scene/Renderable/Texture.hpp"
#include "Scene/Renderer/RendererObjectManager.hpp"
#include "Utils/Glm.hpp"

namespace Stone::Scene {

std::ostream &Material::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",textures:{";
	for (auto &it : _textures)
		stream << it.first << ":" << (it.second ? std::to_string(it.second->getId()) : "null") << ",";
	stream << "},vectors:{";
	for (auto &it : _vectors)
		stream << it.first << ":" << it.second << ",";
	stream << "},scalars:{";
	for (auto &it : _scalars)
		stream << it.first << ":" << it.second << ",";
	stream << "}";
	if (closing_bracer)
		stream << "}";
	return stream;
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

std::ostream &operator<<(std::ostream &stream, const Material::Location &location) {
	if (std::holds_alternative<std::string>(location)) {
		stream << std::get<std::string>(location);
	} else {
		stream << std::get<int>(location);
	}
	return stream;
}

} // namespace Stone::Scene
