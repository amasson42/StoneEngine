// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/Node.hpp"
#include "Scene/Transform.hpp"

namespace Stone::Scene {

class PivotNode;

/**
 * @class SkeletonNode
 * @brief A node that contains a list of bones that can be used with animations.
 *
 * The `SkeletonNode` class contains a list of bones that are used by skeletal animations.
 * Each bone is represented by a pivot node and its rest pose.
 */
class SkeletonNode : public Node {
	STONE_NODE(SkeletonNode);

public:
	struct Bone {
		std::weak_ptr<PivotNode> pivot;
		glm::mat4 inverseBindMatrix;
		Transform3D restPose;

		explicit Bone(const std::shared_ptr<PivotNode> &pivot);
	};

	explicit SkeletonNode(const std::string &name = "skeleton");
	SkeletonNode(const SkeletonNode &other) = default;

	~SkeletonNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] const std::vector<Bone> &getBones() const;
	void addBone(const std::shared_ptr<PivotNode> &pivot);
	void addBone(const std::shared_ptr<PivotNode> &pivot, const glm::mat4 &offset);

protected:
	std::vector<Bone> _bones;

	[[nodiscard]] const char *_termClassColor() const override;
};

} // namespace Stone::Scene
