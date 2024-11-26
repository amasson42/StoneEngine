// Copyright 2024 Stone-Engine

#include "Scene/Node/SkinMeshNode.hpp"

#include "Scene/Node/SkeletonNode.hpp"
#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/SkinMesh.hpp"
#include "Scene/RendererObjectManager.hpp"

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(SkinMeshNode)

SkinMeshNode::SkinMeshNode(const std::string &name) : RenderableNode(name), _mesh(), _material(), _skeleton() {
}

void SkinMeshNode::writeToJson(Json::Object &json) const {
	RenderableNode::writeToJson(json);

	json["mesh"] = _mesh ? Json::number(_mesh->getId()) : Json::null();
	json["material"] = _material ? Json::number(_material->getId()) : Json::null();
	json["skeleton"] = !_skeleton.expired() ? Json::string(_skeleton.lock()->getGlobalName()) : Json::null();
}

std::shared_ptr<ISkinMeshInterface> SkinMeshNode::getSkinMesh() const {
	return _mesh;
}

void SkinMeshNode::setSkinMesh(std::shared_ptr<ISkinMeshInterface> mesh) {
	_mesh = std::move(mesh);
	markDirty();
}

std::shared_ptr<Material> SkinMeshNode::getMaterial() const {
	return _material;
}

void SkinMeshNode::setMaterial(std::shared_ptr<Material> material) {
	_material = std::move(material);
	markDirty();
}

std::shared_ptr<SkeletonNode> SkinMeshNode::getSkeleton() const {
	return _skeleton.lock();
}

void SkinMeshNode::setSkeleton(const std::shared_ptr<SkeletonNode> &skeleton) {
	_skeleton = skeleton;
	markDirty();
}

const char *SkinMeshNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_MAGENTA;
}

} // namespace Stone::Scene
