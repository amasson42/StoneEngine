// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene {

class LightNode : public PivotNode {
	STONE_ABSTRACT_NODE(LightNode);

public:
	explicit LightNode(const std::string &name = "light");
	LightNode(const LightNode &other) = default;

	~LightNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] virtual bool isCastingShadow() const;

	[[nodiscard]] float getIntensity() const;
	void setIntensity(float intensity);

	[[nodiscard]] const glm::vec3 &getColor() const;
	void setColor(const glm::vec3 &color);

protected:
	float _intensity;
	glm::vec3 _color;

	[[nodiscard]] const char *_termClassColor() const override;
};

class AmbientLightNode : public LightNode {
	STONE_NODE(AmbientLightNode);

public:
	explicit AmbientLightNode(const std::string &name = "ambientlight");
	AmbientLightNode(const AmbientLightNode &other);

	~AmbientLightNode() override = default;

	void writeToJson(Json::Object &json) const override;
};

class PointLightNode : public LightNode {
	STONE_NODE(PointLightNode);

public:
	explicit PointLightNode(const std::string &name = "pointlight");
	PointLightNode(const PointLightNode &other);

	~PointLightNode() override = default;

	[[nodiscard]] const glm::vec3 &getAttenuation() const;
	void setAttenuation(const glm::vec3 &attenuation);

	[[nodiscard]] const glm::vec3 &getSpecular() const;
	void setSpecular(const glm::vec3 &specular);

	void writeToJson(Json::Object &json) const override;

protected:
	glm::vec3 _attenuation;
	glm::vec3 _specular;
};

class CastingLightNode : public LightNode {
	STONE_ABSTRACT_NODE(CastingLightNode);

public:
	explicit CastingLightNode(const std::string &name = "castinglight");
	CastingLightNode(const CastingLightNode &other);

	~CastingLightNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] bool isCastingShadow() const override;
	void setCastingShadow(bool castShadow);

	[[nodiscard]] const glm::mat4 &getProjectionMatrix() const;

	[[nodiscard]] float getShadowClipNear() const;
	void setShadowClipNear(float shadowClipNear);

	[[nodiscard]] float getShadowClipFar() const;
	void setShadowClipFar(float shadowClipFar);

	[[nodiscard]] const glm::ivec2 &getShadowMapSize() const;
	void setShadowMapSize(const glm::ivec2 &shadowMapSize);

protected:
	bool _castShadow;
	float _shadowClipNear;
	float _shadowClipFar;
	glm::ivec2 _shadowMapSize;
	glm::mat4 _projectionMatrix;

	virtual void _updateProjectionMatrix() = 0;

	[[nodiscard]] const char *_termClassColor() const override;
};

class DirectionalLightNode : public CastingLightNode {
	STONE_NODE(DirectionalLightNode);

public:
	explicit DirectionalLightNode(const std::string &name = "directionallight");
	DirectionalLightNode(const DirectionalLightNode &other);

	~DirectionalLightNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] bool isInfinite() const;
	void setInfinite(bool infinite);

	[[nodiscard]] const glm::vec2 &getShadowOrthoSize() const;
	void setShadowOrthoSize(const glm::vec2 &shadowOrthoSize);

protected:
	bool _infinite;
	glm::vec2 _shadowOrthoSize;

	void _updateProjectionMatrix() override;
};

class SpotLightNode : public CastingLightNode {
	STONE_NODE(SpotLightNode);

public:
	explicit SpotLightNode(const std::string &name = "spotlight");
	SpotLightNode(const SpotLightNode &other);

	~SpotLightNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] float getConeAngle() const;
	void setConeAngle(float coneAngle);

	[[nodiscard]] float getConeAttenuation() const;
	void setConeAttenuation(float coneAttenuation);

protected:
	float _coneAngle;
	float _coneAttenuation;

	void _updateProjectionMatrix() override;
};

} // namespace Stone::Scene
