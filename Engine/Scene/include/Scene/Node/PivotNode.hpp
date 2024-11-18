// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Node/Node.hpp"
#include "Scene/Transform.hpp"

namespace Stone::Scene {

/**
 * @class PivotNode
 * @brief Represents a node that can be transformed.
 *
 * The `PivotNode` class is a node that can be transformed in 3D space. It provides a Transform3D property that can be
 * used to move, rotate, and scale the node.
 *
 * @note Child nodes of a `PivotNode` are transformed relative to the pivot node's transform.
 * All transformations applied to the pivot node are also applied to its children but not to its parent.
 */
class PivotNode : public Node {
	STONE_NODE(PivotNode);

public:
	explicit PivotNode(const std::string &name = "pivot");
	PivotNode(const PivotNode &other) = default;

	~PivotNode() override = default;

	void writeToJson(Json::Object &json) const override;

	void render(RenderContext &context) override;

	void transformRelativeMatrix(glm::mat4 &relative) const override;

	Transform3D &getTransform();
	[[nodiscard]] const Transform3D &getTransform() const;
	void setTransform(const Transform3D &transform);

	const glm::mat4 &getTransformMatrix();
	[[nodiscard]] glm::mat4 getTransformMatrix() const;

protected:
	Transform3D _transform;

	[[nodiscard]] const char *_termClassColor() const override;
};

} // namespace Stone::Scene
