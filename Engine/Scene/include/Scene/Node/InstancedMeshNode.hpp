// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/MeshNode.hpp"
#include "Scene/Transform.hpp"

namespace Stone::Scene {

class InstancedMeshNode : public MeshNode {
	STONE_NODE(InstancedMeshNode);

public:
	explicit InstancedMeshNode(const std::string &name = "instancedmesh");
	InstancedMeshNode(const InstancedMeshNode &other) = default;

	~InstancedMeshNode() override = default;

	void writeToJson(Json::Object &json) const override;

	void addInstance(const Transform3D &transform);
	void removeInstance(int index);
	void clearInstances();

	[[nodiscard]] const std::vector<Transform3D> &getInstancesTransforms() const;
	void withInstanceTransforms(const std::function<void(std::vector<Transform3D> &)> &func);

protected:
	std::vector<Transform3D> _instancesTransforms;
};

} // namespace Stone::Scene
