// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Mesh.hpp"

namespace Stone::Scene {

void DynamicMesh::writeToJson(Json::Object &json) const {
	Object::writeToJson(json);

	json["vertices"] = Json::number(_vertices.size());
	json["indices"] = Json::number(_indices.size());
}

const std::vector<Vertex> &DynamicMesh::getVertices() const {
	return _vertices;
}

const std::vector<uint32_t> &DynamicMesh::getIndices() const {
	return _indices;
}

void DynamicMesh::withElementsRef(const std::function<void(std::vector<Vertex> &, std::vector<uint32_t> &)> &func) {
	func(_vertices, _indices);
	markDirty();
}

void StaticMesh::writeToJson(Json::Object &json) const {
	Object::writeToJson(json);

	json["dynamic_mesh"] = _dynamicMesh ? Json::number(_dynamicMesh->getId()) : Json::null();
}

const std::shared_ptr<DynamicMesh> &StaticMesh::getSourceMesh() const {
	return _dynamicMesh;
}

void StaticMesh::setSourceMesh(const std::shared_ptr<DynamicMesh> &sourceMesh) {
	_dynamicMesh = sourceMesh;
	markDirty();
}


} // namespace Stone::Scene
