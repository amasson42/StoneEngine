// Copyrighted 2024 Stone-Engine

#include "Scene/Assets/AssetResource.hpp"

#include "Scene/Node/PivotNode.hpp"
#include "Scene/Renderable/IMeshObject.hpp"
#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/Texture.hpp"
#include "Utils/StringExt.hpp"

namespace Stone::Scene {

AssetResource::AssetResource(const std::shared_ptr<Core::Assets::Bundle> &bundle, const std::string &filepath)
	: Core::Assets::Resource(bundle, filepath) {
	loadData();
};

std::ostream &AssetResource::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Core::Assets::Resource::writeToStream(stream, false);
	stream << ",meshes:[";
	for (const auto &mesh : _meshes) {
		stream << mesh->getId() << ",";
	}
	stream << "],textures:[";
	for (const auto &texture : _textures) {
		stream << texture->getId() << ",";
	}
	stream << "],materials:[";
	for (const auto &material : _materials) {
		stream << material->getId() << ",";
	}
	stream << "],rootNode:";
	if (_rootNode) {
		stream << _rootNode->getId();
	} else {
		stream << "null";
	}
	if (closing_bracer)
		stream << "}";
	return stream;
}

const std::vector<std::shared_ptr<IMeshObject>> &AssetResource::getMeshes() const {
	return _meshes;
}

std::vector<std::shared_ptr<IMeshObject>> &AssetResource::getMeshesRef() {
	return _meshes;
}

const std::vector<std::shared_ptr<Texture>> &AssetResource::getTextures() const {
	return _textures;
}

std::vector<std::shared_ptr<Texture>> &AssetResource::getTexturesRef() {
	return _textures;
}

const std::vector<std::shared_ptr<Material>> &AssetResource::getMaterials() const {
	return _materials;
}

std::vector<std::shared_ptr<Material>> &AssetResource::getMaterialsRef() {
	return _materials;
}

const std::shared_ptr<PivotNode> &AssetResource::getRootNode() const {
	return _rootNode;
}

const Json::Object &AssetResource::getMetadatas() const {
	return _metadatas;
}

Json::Object &AssetResource::getMetadatas() {
	return _metadatas;
};


void AssetResource::loadData() {
	if (string_ends_with(_filename, ".stone")) {
		loadFromStone();
	} else {
		loadFromAssimp();
	}
}


} // namespace Stone::Scene
