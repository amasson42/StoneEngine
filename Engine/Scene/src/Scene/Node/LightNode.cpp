// Copyright 2024 Stone-Engine

#include "Scene/Node/LightNode.hpp"

#include "Utils/Glm.hpp"

namespace Stone::Scene {

STONE_ABSTRACT_NODE_IMPLEMENTATION(LightNode);

LightNode::LightNode(const std::string &name) : PivotNode(name), _intensity(1.0f), _color(1.0f) {
}

void LightNode::writeToJson(Json::Object &json) const {
	PivotNode::writeToJson(json);

	json["intensity"] = Json::number(_intensity);
	json["color"] = to_json(_color);
}

bool LightNode::isCastingShadow() const {
	return false;
}

float LightNode::getIntensity() const {
	return _intensity;
}

void LightNode::setIntensity(float intensity) {
	_intensity = intensity;
}

const glm::vec3 &LightNode::getColor() const {
	return _color;
}

void LightNode::setColor(const glm::vec3 &color) {
	_color = color;
}

const char *LightNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_YELLOW;
}

STONE_NODE_IMPLEMENTATION(AmbientLightNode);

AmbientLightNode::AmbientLightNode(const std::string &name) : LightNode(name) {
}

void AmbientLightNode::writeToJson(Json::Object &json) const {
	LightNode::writeToJson(json);
}

STONE_NODE_IMPLEMENTATION(PointLightNode);

PointLightNode::PointLightNode(const std::string &name) : LightNode(name), _attenuation(1.0f), _specular(1.0f) {
}

const glm::vec3 &PointLightNode::getAttenuation() const {
	return _attenuation;
}

void PointLightNode::setAttenuation(const glm::vec3 &attenuation) {
	_attenuation = attenuation;
}

const glm::vec3 &PointLightNode::getSpecular() const {
	return _specular;
}

void PointLightNode::setSpecular(const glm::vec3 &specular) {
	_specular = specular;
}

void PointLightNode::writeToJson(Json::Object &json) const {
	LightNode::writeToJson(json);

	json["attenuation"] = to_json(_attenuation);
	json["specular"] = to_json(_specular);
}

STONE_ABSTRACT_NODE_IMPLEMENTATION(CastingLightNode);

CastingLightNode::CastingLightNode(const std::string &name)
	: LightNode(name), _castShadow(true), _shadowClipNear(0.1f), _shadowClipFar(100.0f), _shadowMapSize(1024),
	  _projectionMatrix(1) {
}

void CastingLightNode::writeToJson(Json::Object &json) const {
	LightNode::writeToJson(json);

	json["cast_shadow"] = Json::boolean(_castShadow);
	json["shadow_clip_near"] = Json::number(_shadowClipNear);
	json["shadow_clip_far"] = Json::number(_shadowClipFar);
	json["shadow_map_size"] = to_json(_shadowMapSize);
}

bool CastingLightNode::isCastingShadow() const {
	return _castShadow;
}

void CastingLightNode::setCastingShadow(bool castShadow) {
	_castShadow = castShadow;
}

const glm::mat4 &CastingLightNode::getProjectionMatrix() const {
	return _projectionMatrix;
}

float CastingLightNode::getShadowClipNear() const {
	return _shadowClipNear;
}

void CastingLightNode::setShadowClipNear(float shadowClipNear) {
	_shadowClipNear = shadowClipNear;
	_updateProjectionMatrix();
}

float CastingLightNode::getShadowClipFar() const {
	return _shadowClipFar;
}

void CastingLightNode::setShadowClipFar(float shadowClipFar) {
	_shadowClipFar = shadowClipFar;
	_updateProjectionMatrix();
}

const glm::ivec2 &CastingLightNode::getShadowMapSize() const {
	return _shadowMapSize;
}

void CastingLightNode::setShadowMapSize(const glm::ivec2 &shadowMapSize) {
	_shadowMapSize = shadowMapSize;
}

const char *CastingLightNode::_termClassColor() const {
	return TERM_COLOR_BOLD TERM_COLOR_ORANGE;
}

STONE_NODE_IMPLEMENTATION(DirectionalLightNode);

DirectionalLightNode::DirectionalLightNode(const std::string &name)
	: CastingLightNode(name), _infinite(true), _shadowOrthoSize(10.0f, 10.0f) {
	_updateProjectionMatrix();
}

void DirectionalLightNode::writeToJson(Json::Object &json) const {
	CastingLightNode::writeToJson(json);

	json["infinite"] = Json::boolean(_infinite);
	json["shadow_ortho_size"] = to_json(_shadowOrthoSize);
}

bool DirectionalLightNode::isInfinite() const {
	return _infinite;
}

void DirectionalLightNode::setInfinite(bool infinite) {
	_infinite = infinite;
}

const glm::vec2 &DirectionalLightNode::getShadowOrthoSize() const {
	return _shadowOrthoSize;
}

void DirectionalLightNode::setShadowOrthoSize(const glm::vec2 &shadowOrthoSize) {
	_shadowOrthoSize = shadowOrthoSize;
	_updateProjectionMatrix();
}

void DirectionalLightNode::_updateProjectionMatrix() {
	_projectionMatrix = glm::ortho(-_shadowOrthoSize.x / 2.0f, _shadowOrthoSize.x / 2.0f, -_shadowOrthoSize.y / 2.0f,
								   _shadowOrthoSize.y / 2.0f, _shadowClipNear, _shadowClipFar);
}

STONE_NODE_IMPLEMENTATION(SpotLightNode);

SpotLightNode::SpotLightNode(const std::string &name)
	: CastingLightNode(name), _coneAngle(glm::radians(45.0f)), _coneAttenuation(0.0f) {
	_updateProjectionMatrix();
}

void SpotLightNode::writeToJson(Json::Object &json) const {
	CastingLightNode::writeToJson(json);

	json["cone_angle"] = Json::number(_coneAngle);
	json["cone_attenuation"] = Json::number(_coneAttenuation);
}

float SpotLightNode::getConeAngle() const {
	return _coneAngle;
}

void SpotLightNode::setConeAngle(float coneAngle) {
	_coneAngle = coneAngle;
	_updateProjectionMatrix();
}

float SpotLightNode::getConeAttenuation() const {
	return _coneAttenuation;
}

void SpotLightNode::setConeAttenuation(float coneAttenuation) {
	_coneAttenuation = coneAttenuation;
}

void SpotLightNode::_updateProjectionMatrix() {
	_projectionMatrix = glm::perspective(_coneAngle, 1.0f, _shadowClipNear, _shadowClipFar);
}

} // namespace Stone::Scene
