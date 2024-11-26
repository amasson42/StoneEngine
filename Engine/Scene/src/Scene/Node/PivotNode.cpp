// Copyright 2024 Stone-Engine

#include "Scene/Node/PivotNode.hpp"

#include <sstream>

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(PivotNode)

PivotNode::PivotNode(const std::string &name) : Node(name), _transform() {
}

void PivotNode::writeToJson(Json::Object &json) const {
	Node::writeToJson(json);

	json["transform"] = _transform.toJson();
}

void PivotNode::render(RenderContext &context) {
	glm::mat4 previousModelMatrix = context.mvp.modelMatrix;

	context.mvp.modelMatrix = context.mvp.modelMatrix * getTransformMatrix();
	for (auto &child : getChildren()) {
		child->render(context);
	}
	context.mvp.modelMatrix = previousModelMatrix;
}

void PivotNode::transformRelativeMatrix(glm::mat4 &relative) const {
	relative = getTransformMatrix() * relative;
}

Transform3D &PivotNode::getTransform() {
	return _transform;
}

const Transform3D &PivotNode::getTransform() const {
	return _transform;
}

void PivotNode::setTransform(const Transform3D &transform) {
	_transform = transform;
}

const glm::mat4 &PivotNode::getTransformMatrix() {
	return _transform.getTransformMatrix();
}

glm::mat4 PivotNode::getTransformMatrix() const {
	return _transform.getTransformMatrix();
}

const char *PivotNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_RED;
}

} // namespace Stone::Scene
