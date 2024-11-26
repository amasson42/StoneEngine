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

void AssetResource::writeToJson(Json::Object &json) const {
	Core::Assets::Resource::writeToJson(json);

	auto &meshes((json["meshes"] = Json::array()).get<Json::Array>());
	for (const auto &mesh : _meshes) {
		meshes.push_back(Json::number(mesh->getId()));
	}

	auto &textures((json["textures"] = Json::array()).get<Json::Array>());
	for (const auto &texture : _textures) {
		textures.push_back(Json::number(texture->getId()));
	}

	auto &materials((json["materials"] = Json::array()).get<Json::Array>());
	for (const auto &material : _materials) {
		materials.push_back(Json::number(material->getId()));
	}

	json["root_node"] = _rootNode ? Json::number(_rootNode->getId()) : Json::null();
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
