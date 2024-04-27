// Copyright 2024 Stone-Engine

#include "Scene/Node/WorldNode.hpp"

#include "Scene/Node/CameraNode.hpp"

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(WorldNode)

WorldNode::WorldNode(const std::string &name) : Node(name), _activeCamera() {
}

WorldNode::WorldNode(const WorldNode &other) : Node(other), _activeCamera(other._activeCamera) {
}

WorldNode::~WorldNode() {
}

std::ostream &WorldNode::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Node::writeToStream(stream, false);
	stream << ",active_camera:" << (_activeCamera.expired() ? "null" : _activeCamera.lock()->getGlobalName());
	if (closing_bracer)
		stream << "}";
	return stream;
}

void WorldNode::setActiveCamera(std::shared_ptr<CameraNode> camera) {
	_activeCamera = camera;
}

std::shared_ptr<CameraNode> WorldNode::getActiveCamera() const {
	return _activeCamera.lock();
}

void WorldNode::render() {
	RenderContext context;

	if (auto camera = _activeCamera.lock()) {
		context.viewMatrix = glm::inverse(camera->getWorldTransformMatrix());
		context.projectionMatrix = camera->getProjectionMatrix();
	}

	Node::render(context);
}

const char *WorldNode::_termClassColor() const {
	return TERM_COLOR_RED;
}

} // namespace Stone::Scene
