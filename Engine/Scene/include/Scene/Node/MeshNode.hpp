// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/RenderableNode.hpp"

namespace Stone::Scene {

class IMeshInterface;
class Material;

class MeshNode : public RenderableNode {
	STONE_NODE(MeshNode);

public:
	explicit MeshNode(const std::string &name = "mesh");
	MeshNode(const MeshNode &other) = default;

	~MeshNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] std::shared_ptr<IMeshInterface> getMesh() const;
	void setMesh(std::shared_ptr<IMeshInterface> mesh);

	[[nodiscard]] std::shared_ptr<Material> getMaterial() const;
	void setMaterial(std::shared_ptr<Material> material);

protected:
	std::shared_ptr<IMeshInterface> _mesh;
	std::shared_ptr<Material> _material;

	[[nodiscard]] const char *_termClassColor() const override;
};

} // namespace Stone::Scene
