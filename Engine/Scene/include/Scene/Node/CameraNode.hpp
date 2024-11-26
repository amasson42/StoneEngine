// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/PivotNode.hpp"

namespace Stone::Scene {

/**
 * @class CameraNode
 * @brief Represents a camera node.
 *
 * The `CameraNode` class represents a camera node in the scene graph.
 *
 * This class is abstract and cannot be instantiated directly. Use `PerspectiveCameraNode` or `OrthographicCameraNode`
 */
class CameraNode : public PivotNode {
	STONE_ABSTRACT_NODE(CameraNode);

public:
	explicit CameraNode(const std::string &name = "camera");
	CameraNode(const CameraNode &other);

	~CameraNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] virtual glm::mat4 getProjectionMatrix() const = 0;

	[[nodiscard]] float getNear() const;
	void setNear(float near);

	[[nodiscard]] float getFar() const;
	void setFar(float far);

protected:
	float _near;
	float _far;

	[[nodiscard]] const char *_termClassColor() const override;
};

class PerspectiveCameraNode : public CameraNode {
	STONE_NODE(PerspectiveCameraNode);

public:
	explicit PerspectiveCameraNode(const std::string &name = "perspective_camera");
	PerspectiveCameraNode(const PerspectiveCameraNode &other);

	~PerspectiveCameraNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] glm::mat4 getProjectionMatrix() const override;

	[[nodiscard]] float getFov() const;
	void setFov(float fov);

	[[nodiscard]] float getAspect() const;
	void setAspect(float aspect);

protected:
	float _fov;
	float _aspect;
};

class OrthographicCameraNode : public CameraNode {
	STONE_NODE(OrthographicCameraNode);

public:
	explicit OrthographicCameraNode(const std::string &name = "orthographic_camera");
	OrthographicCameraNode(const OrthographicCameraNode &other);

	~OrthographicCameraNode() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] glm::mat4 getProjectionMatrix() const override;

	[[nodiscard]] glm::vec2 getSize() const;
	void setSize(const glm::vec2 &size);

protected:
	glm::vec2 _size;
};

} // namespace Stone::Scene
