// Copyright 2024 Stone-Engine

#include "Scene/Node/MeshNode.hpp"

#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/Mesh.hpp"

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(MeshNode)

MeshNode::MeshNode(const std::string &name) : RenderableNode(name), _mesh(nullptr), _material(nullptr) {
}

void MeshNode::writeToJson(Json::Object &json) const {
	RenderableNode::writeToJson(json);

	json["mesh"] = _mesh ? Json::number(_mesh->getId()) : Json::null();
	json["material"] = _material ? Json::number(_material->getId()) : Json::null();
}

std::shared_ptr<IMeshInterface> MeshNode::getMesh() const {
	return _mesh;
}

void MeshNode::setMesh(std::shared_ptr<IMeshInterface> mesh) {
	_mesh = std::move(mesh);
	markDirty();
}

std::shared_ptr<Material> MeshNode::getMaterial() const {
	return _material;
}

void MeshNode::setMaterial(std::shared_ptr<Material> material) {
	_material = std::move(material);
	markDirty();
}

const char *MeshNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_GREEN;
}

} // namespace Stone::Scene
