// Copyright 2024 Stone-Engine

#include "Scene/Node/CameraNode.hpp"

#include "Utils/Glm.hpp"

namespace Stone::Scene {

STONE_ABSTRACT_NODE_IMPLEMENTATION(CameraNode)

CameraNode::CameraNode(const std::string &name) : PivotNode(name), _near(0.1f), _far(100.0f) {
}

void CameraNode::writeToJson(Json::Object &json) const {
	PivotNode::writeToJson(json);

	json["near"] = Json::number(_near);
	json["far"] = Json::number(_far);
}

float CameraNode::getNear() const {
	return _near;
}

void CameraNode::setNear(float near) {
	_near = near;
}

float CameraNode::getFar() const {
	return _far;
}

void CameraNode::setFar(float far) {
	_far = far;
}

const char *CameraNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_BLUE;
}

STONE_NODE_IMPLEMENTATION(PerspectiveCameraNode)

PerspectiveCameraNode::PerspectiveCameraNode(const std::string &name)
	: CameraNode(name), _fov(glm::radians(45.0f)), _aspect(1.0f) {
}

void PerspectiveCameraNode::writeToJson(Json::Object &json) const {
	CameraNode::writeToJson(json);

	json["fov"] = Json::number(_fov);
	json["aspect"] = Json::number(_aspect);
}

glm::mat4 PerspectiveCameraNode::getProjectionMatrix() const {
	return glm::perspective(_fov, _aspect, _near, _far);
}

float PerspectiveCameraNode::getFov() const {
	return _fov;
}

void PerspectiveCameraNode::setFov(float fov) {
	_fov = fov;
}

float PerspectiveCameraNode::getAspect() const {
	return _aspect;
}

void PerspectiveCameraNode::setAspect(float aspect) {
	_aspect = aspect;
}

STONE_NODE_IMPLEMENTATION(OrthographicCameraNode)

OrthographicCameraNode::OrthographicCameraNode(const std::string &name) : CameraNode(name), _size(10.0f) {
}

void OrthographicCameraNode::writeToJson(Json::Object &json) const {
	CameraNode::writeToJson(json);

	json["size"] = to_json(_size);
}

glm::mat4 OrthographicCameraNode::getProjectionMatrix() const {
	return glm::ortho(-_size.x / 2, _size.x / 2, -_size.y / 2, _size.y / 2, _near, _far);
}

} // namespace Stone::Scene
