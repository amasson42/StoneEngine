// Copyright 2024 Stone-Engine

#include "Scene/Renderable/Mesh.hpp"

#include "Scene/RendererObjectManager.hpp"

namespace Stone::Scene {

std::ostream &DynamicMesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",vertices:" << _vertices.size();
	stream << ",indices:" << _indices.size();
	if (closing_bracer)
		stream << "}";
	return stream;
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

std::ostream &StaticMesh::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Object::writeToStream(stream, false);
	stream << ",dynamic_mesh:" << _dynamicMesh ? std::to_string(_dynamicMesh->getId()) : "null";
	if (closing_bracer)
		stream << "}";
	return stream;
}

const std::shared_ptr<DynamicMesh> &StaticMesh::getSourceMesh() const {
	return _dynamicMesh;
}

void StaticMesh::setSourceMesh(const std::shared_ptr<DynamicMesh> &sourceMesh) {
	_dynamicMesh = sourceMesh;
	markDirty();
}


} // namespace Stone::Scene
