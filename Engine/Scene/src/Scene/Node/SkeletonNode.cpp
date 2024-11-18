// Copyright 2024 Stone-Engine

#include "Scene/Node/SkeletonNode.hpp"

#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(SkeletonNode)

SkeletonNode::Bone::Bone(const std::shared_ptr<PivotNode> &pivot)
	: pivot(pivot), inverseBindMatrix(1.0f), restPose(pivot->getTransform()) {
}

SkeletonNode::SkeletonNode(const std::string &name) : Node(name), _bones() {
}

void SkeletonNode::writeToJson(Json::Object &json) const {
	Node::writeToJson(json);

	Json::Array &bones((json["bones"] = Json::array()).get<Json::Array>());
	for (const auto &bone : _bones) {
		bones.push_back(Json::string(bone.pivot.lock()->getGlobalName()));
	}
}

const std::vector<SkeletonNode::Bone> &SkeletonNode::getBones() const {
	return _bones;
}

void SkeletonNode::addBone(const std::shared_ptr<PivotNode> &pivot) {
	glm::mat4 inverseBindMatrix;
	if (!_bones.empty()) {
		inverseBindMatrix = glm::inverse(pivot->getTransformMatrixRelativeToNode(_bones[0].pivot.lock()));
	} else {
		inverseBindMatrix = glm::mat4(1);
	}
	addBone(pivot, inverseBindMatrix);
}

void SkeletonNode::addBone(const std::shared_ptr<PivotNode> &pivot, const glm::mat4 &offset) {
	Bone bone(pivot);
	bone.inverseBindMatrix = offset;
	_bones.push_back(bone);
}

const char *SkeletonNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_CYAN;
}

} // namespace Stone::Scene
