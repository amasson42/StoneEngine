// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/RenderableNode.hpp"

namespace Stone::Scene {

class ISkinMeshInterface;
class SkeletonNode;
class Material;

class SkinMeshNode : public RenderableNode {
	STONE_NODE(SkinMeshNode);

public:
	explicit SkinMeshNode(const std::string &name = "skinmesh");
	SkinMeshNode(const SkinMeshNode &other) = default;

	~SkinMeshNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] std::shared_ptr<ISkinMeshInterface> getSkinMesh() const;
	void setSkinMesh(std::shared_ptr<ISkinMeshInterface> mesh);

	[[nodiscard]] std::shared_ptr<Material> getMaterial() const;
	void setMaterial(std::shared_ptr<Material> material);

	[[nodiscard]] std::shared_ptr<SkeletonNode> getSkeleton() const;
	void setSkeleton(const std::shared_ptr<SkeletonNode> &skeleton);

protected:
	std::shared_ptr<ISkinMeshInterface> _mesh;
	std::shared_ptr<Material> _material;
	std::weak_ptr<SkeletonNode> _skeleton;

	[[nodiscard]] const char *_termClassColor() const override;
};

} // namespace Stone::Scene
