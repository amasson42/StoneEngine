// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Shader.hpp"

#include "Utils/StringExt.hpp"

#include <iomanip>

namespace Stone::Scene {

Shader::Shader(const std::string &content)
	: Object(), IRenderable(), _contentType(ContentType::SourceFile), _content(content) {
	if (string_ends_with(content, ".glsl")) {
		_contentType = ContentType::SourceFile;
	} else if (string_ends_with(content, ".spv") || string_ends_with(content, ".metal")) {
		_contentType = ContentType::CompiledFile;
	} else if (string_starts_with(content, "#version")) {
		_contentType = ContentType::SourceCode;
	} else if (string_contains_non_printable(content)) {
		_contentType = ContentType::CompiledCode;
	}
}

Shader::Shader(ContentType contentType, std::string content)
	: Object(), IRenderable(), _contentType(contentType), _content(std::move(content)) {
}

void Shader::writeToJson(Json::Object &json) const {
	Object::writeToJson(json);

	json["function"] = Json::string(_function);

	switch (_contentType) {
	case ContentType::SourceCode: json["source_code"] = Json::string(_content); break;
	case ContentType::SourceFile: json["source_file"] = Json::string(_content); break;
	case ContentType::CompiledCode:
		{
			// TODO: write in base64
			std::stringstream hexastring;
			for (char c : _content) {
				hexastring << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
			}
			json["compiled_code"] = Json::string(hexastring.str());
		}
		break;
	case ContentType::CompiledFile: json["compiled_file"] = Json::string(_content); break;
	}
}

std::pair<Shader::ContentType, const std::string &> Shader::getContent() const {
	return {_contentType, _content};
}

const std::string &Shader::getFunction() const {
	return _function;
}

void Shader::setFunction(const std::string &function) {
	_function = function;
	markDirty();
}

int Shader::getMaxLocation() const {
	return _maxLocation;
}

int Shader::getLocation(const std::string &name) const {
	auto it = _locations.find(name);
	if (it == _locations.end()) {
		return -1;
	}
	return it->second;
}

void Shader::setContent(ContentType contentType, std::string content) {
	_contentType = contentType;
	_content = std::move(content);
	markDirty();
}

void Shader::setLocation(const std::string &name, int location) {
	_locations[name] = location;
	if (location > _maxLocation) {
		_maxLocation = location;
	}
	markDirty();
}

} // namespace Stone::Scene
