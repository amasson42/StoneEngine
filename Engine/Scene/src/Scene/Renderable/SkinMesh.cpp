// Copyright 2024 Stone-Engine

#include "Scene/Renderable/SkinMesh.hpp"

#include "Scene/Node/SkeletonNode.hpp"

namespace Stone::Scene {

void DynamicSkinMesh::writeToJson(Json::Object &json) const {
	Object::writeToJson(json);

	json["vertices"] = Json::number(static_cast<double>(_vertices.size()));
	json["indices"] = Json::number(static_cast<double>(_indices.size()));
}

const std::vector<WeightVertex> &DynamicSkinMesh::getVertices() const {
	return _vertices;
}

const std::vector<uint32_t> &DynamicSkinMesh::getIndices() const {
	return _indices;
}

void DynamicSkinMesh::withElementsRef(
	const std::function<void(std::vector<WeightVertex> &, std::vector<uint32_t> &)> &func) {
	func(_vertices, _indices);
	markDirty();
}

void StaticSkinMesh::writeToJson(Json::Object &json) const {
	Object::writeToJson(json);

	json["source_mesh"] = _dynamicMesh ? Json::number(_dynamicMesh->getId()) : Json::null();
}

const std::shared_ptr<DynamicSkinMesh> &StaticSkinMesh::getSourceMesh() const {
	return _dynamicMesh;
}

void StaticSkinMesh::setSourceMesh(const std::shared_ptr<DynamicSkinMesh> &sourceMesh) {
	_dynamicMesh = sourceMesh;
	markDirty();
}

} // namespace Stone::Scene
