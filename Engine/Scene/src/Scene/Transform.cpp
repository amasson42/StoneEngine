// Copyright 2024 Stone-Engine

#include "Scene/Transform.hpp"

#include "Utils/Glm.hpp"
#include "Utils/Json.hpp"

#include <glm/gtx/matrix_decompose.hpp>

namespace Stone::Scene {

Transform2D::Transform2D()
	: _position(0.0f, 0.0f), _rotation(0.0f), _scale(1.0f, 1.0f), _transformMatrix(1.0f), _transformMatrixDirty(true) {
	calculateTransformMatrix(_transformMatrix);
}

void Transform2D::setPosition(const glm::vec2 &position) {
	_position = position;
	_transformMatrixDirty = true;
}

void Transform2D::setRotation(float rotation) {
	_rotation = rotation;
	_transformMatrixDirty = true;
}

void Transform2D::setScale(const glm::vec2 &scale) {
	_scale = scale;
	_transformMatrixDirty = true;
}

void Transform2D::setMatrix(const glm::mat3 &matrix) {
	_position = glm::vec2(matrix[2]);
	_rotation = atan2f(matrix[1][0], matrix[0][0]);
	_scale = glm::vec2(glm::length(glm::vec2(matrix[0])), glm::length(glm::vec2(matrix[1])));
	calculateTransformMatrix(_transformMatrix);
	_transformMatrixDirty = false;
}

const glm::vec2 &Transform2D::getPosition() const {
	return _position;
}

float Transform2D::getRotation() const {
	return _rotation;
}

const glm::vec2 &Transform2D::getScale() const {
	return _scale;
}

void Transform2D::translate(const glm::vec2 &translation) {
	_position += translation;
	_transformMatrixDirty = true;
}

void Transform2D::rotate(float rotation) {
	_rotation += rotation;
	_transformMatrixDirty = true;
}

void Transform2D::scale(const glm::vec2 &scale) {
	_scale *= scale;
	_transformMatrixDirty = true;
}

const glm::mat3 &Transform2D::getTransformMatrix() {
	if (_transformMatrixDirty) {
		calculateTransformMatrix(_transformMatrix);
		_transformMatrixDirty = false;
	}
	return _transformMatrix;
}

glm::mat3 Transform2D::getTransformMatrix() const {
	if (!_transformMatrixDirty) {
		return _transformMatrix;
	}

	glm::mat3 transformMatrix;
	calculateTransformMatrix(transformMatrix);
	return transformMatrix;
}

Json::Value Transform2D::toJson() const {
	return Json::object({
		{"position",		 to_json(_position)},
		{"rotation", Json::number(_rotation)},
		{	 "scale",		  to_json(_scale)},
	});
}

void Transform2D::calculateTransformMatrix(glm::mat3 &m) const {
	const float cosa = cosf(_rotation);
	const float sina = sinf(_rotation);
	glm::value_at(m, 0, 0) = _scale.x * cosa;
	glm::value_at(m, 1, 0) = _scale.x * sina;
	glm::value_at(m, 2, 0) = 0;
	glm::value_at(m, 0, 1) = _scale.y * -sina;
	glm::value_at(m, 1, 1) = _scale.y * cosa;
	glm::value_at(m, 2, 1) = 0;
	glm::value_at(m, 0, 2) = _position.x;
	glm::value_at(m, 1, 2) = _position.y;
	glm::value_at(m, 2, 2) = 1;
}

Transform3D::Transform3D()
	: _position(0.0f, 0.0f, 0.0f), _rotation(1.0f, 0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f), _transformMatrix(1.0f),
	  _transformMatrixDirty(true) {
	calculateTransformMatrix(_transformMatrix);
}

void Transform3D::setPosition(const glm::vec3 &position) {
	_position = position;
	_transformMatrixDirty = true;
}

void Transform3D::setRotation(const glm::quat &rotation) {
	_rotation = rotation;
	_transformMatrixDirty = true;
}

void Transform3D::setEulerAngles(const glm::vec3 &eulerAngles) {
	_rotation = glm::quat(eulerAngles);
	_transformMatrixDirty = true;
}

void Transform3D::setScale(const glm::vec3 &scale) {
	_scale = scale;
	_transformMatrixDirty = true;
}

void Transform3D::setMatrix(const glm::mat4 &matrix) {
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(matrix, _scale, _rotation, _position, skew, perspective);
	calculateTransformMatrix(_transformMatrix);
	_transformMatrixDirty = false;
}

const glm::vec3 &Transform3D::getPosition() const {
	return _position;
}

const glm::quat &Transform3D::getRotation() const {
	return _rotation;
}

glm::vec3 Transform3D::getEulerAngles() const {
	return glm::eulerAngles(_rotation);
}

const glm::vec3 &Transform3D::getScale() const {
	return _scale;
}

void Transform3D::translate(const glm::vec3 &translation) {
	_position += translation;
	_transformMatrixDirty = true;
}

void Transform3D::rotate(const glm::quat &rotation) {
	_rotation = rotation * _rotation;
	_transformMatrixDirty = true;
}

void Transform3D::rotate(float angle, const glm::vec3 &axis) {
	_rotation = glm::angleAxis(angle, axis) * _rotation;
	_transformMatrixDirty = true;
}

void Transform3D::rotate(const glm::vec3 &eulerAngles) {
	_rotation = glm::quat(eulerAngles) * _rotation;
	_transformMatrixDirty = true;
}

void Transform3D::scale(const glm::vec3 &scale) {
	_scale *= scale;
	_transformMatrixDirty = true;
}

const glm::mat4 &Transform3D::getTransformMatrix() {
	if (_transformMatrixDirty) {
		calculateTransformMatrix(_transformMatrix);
		_transformMatrixDirty = false;
	}
	return _transformMatrix;
}

glm::mat4 Transform3D::getTransformMatrix() const {
	if (!_transformMatrixDirty) {
		return _transformMatrix;
	}

	glm::mat4 transformMatrix;
	calculateTransformMatrix(transformMatrix);
	return transformMatrix;
}

Json::Value Transform3D::toJson() const {
	return Json::object({
		{"position", to_json(_position)},
		{"rotation", to_json(_rotation)},
		{	 "scale",	  to_json(_scale)},
	});
}

void Transform3D::calculateTransformMatrix(glm::mat4 &m) const {
	m = glm::mat4(1.0f);
	m = glm::translate(m, _position);
	m = glm::rotate(m, glm::angle(_rotation), glm::axis(_rotation));
	m = glm::scale(m, _scale);
}

} // namespace Stone::Scene


// FIXME: Replace json.serialize with direct output to stream
std::ostream &operator<<(std::ostream &stream, const Stone::Scene::Transform2D &transform) {
	Stone::Json::Value json = transform.toJson();
	stream << json.serialize();
}

std::ostream &operator<<(std::ostream &stream, const Stone::Scene::Transform3D &transform) {
	Stone::Json::Value json = transform.toJson();
	stream << json.serialize();
}
