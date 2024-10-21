// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Geometry.hpp"
#include "Scene/Node/RenderableNode.hpp"

namespace Stone::Scene {

/**
 * @class WireframeShape
 * @brief Represents a wireframe shape that can be rendered in a scene.
 *
 * The WireframeShape class is a subclass of RenderableNode and provides functionality for creating and manipulating
 * wireframe shapes. Wireframe shapes are represented by a collection of vertices and can be rendered as lines or dots.
 * The color, line thickness, and lifespan of the wireframe shape can be customized.
 */
class WireframeShape : public RenderableNode {
	STONE_NODE(WireframeShape);

public:
	/**
	 * @brief Constructs a WireframeShape object.
	 *
	 * @param name The name of the WireframeShape (optional).
	 */
	explicit WireframeShape(const std::string &name = "debug_shape");
	/**
	 * @brief Constructs a new WireframeShape by copying another WireframeShape object.
	 *
	 * @param other The WireframeShape object to be copied.
	 */
	WireframeShape(const WireframeShape &other) = default;

	/**
	 * @brief Destructor for the WireframeShape class.
	 */
	~WireframeShape() override = default;

	/**
	 * Writes the wireframe shape to the output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write a closing bracer.
	 * @return The output stream after writing the wireframe shape.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Retrieves the color
	 *
	 * @return The color of the wireframe shape as a glm::vec3.
	 */
	[[nodiscard]] glm::vec3 getColor() const;
	/**
	 * Sets the color of the wireframe shape.
	 *
	 * @param color The color to set, specified as a glm::vec3.
	 */
	void setColor(const glm::vec3 &color);

	/**
	 * @brief Retrieves the thickness of the wireframe shape.
	 *
	 * @return The thickness of the wireframe shape.
	 */
	[[nodiscard]] float getThickness() const;
	/**
	 * @brief Sets the thickness of the wireframe shape.
	 *
	 * @param thickness The thickness of the wireframe shape.
	 */
	void setThickness(float thickness);

	/**
	 * @brief Retrieves the points of the wireframe shape.
	 *
	 * @return A constant reference to a vector of vectors of glm::vec3 representing the points of the wireframe shape.
	 */
	[[nodiscard]] const std::vector<std::vector<glm::vec3>> &getPoints() const;
	/**
	 * @brief Returns a reference to the vector of vector of glm::vec3 points.
	 *
	 * @return std::vector<std::vector<glm::vec3>>& A reference to the vector of vector of glm::vec3 points.
	 */
	std::vector<std::vector<glm::vec3>> &pointsRef();

	/**
	 * @brief Checks if the wireframe shape should be drawn as a line.
	 *
	 * @return true if the wireframe shape should be drawn as a line, false otherwise.
	 */
	[[nodiscard]] bool isDrawingLine() const;
	/**
	 * @brief Sets whether to draw the wireframe shape as a line.
	 *
	 * @param drawLine True to draw the wireframe shape as a line, false otherwise.
	 */
	void setDrawingLine(bool drawLine);

	/**
	 * @brief Checks if the wireframe shape should ignore the depth buffer.
	 *
	 * @return true if the wireframe shape should ignore the depth buffer, false otherwise.
	 */
	[[nodiscard]] bool isIgnoringDepth() const;

	/**
	 * @brief Sets whether to ignore the depth buffer when rendering the wireframe shape.
	 *
	 * @param ignoreDepth True to ignore the depth buffer, false otherwise.
	 */
	void setIgnoringDepth(bool ignoreDepth);

	/**
	 * @brief Creates a new instance of WireframeShape with the specified shape and arguments.
	 *
	 * @tparam ShapeType The type of shape to generate.
	 * @tparam Args The types of arguments to pass to the shape generator.
	 * @param shape The shape to generate.
	 * @param args The arguments to pass to the shape generator.
	 * @return A shared pointer to the newly created WireframeShape.
	 */
	template <typename ShapeType, typename... Args>
	static std::shared_ptr<WireframeShape> create(const ShapeType &shape, Args &&...args) {
		auto wireframeShape = std::make_shared<WireframeShape>();

		auto [indices, vertices] = generateGeometryMesh(shape, std::forward<Args>(args)...);

		auto &points = wireframeShape->pointsRef().emplace_back();
		points.reserve(indices.size());
		for (auto &index : indices) {
			points.push_back(vertices[index]);
		}

		return wireframeShape;
	}

protected:
	glm::vec3 _color; /** The color of the debug shape. */
	float _thickness; /** The line thickness of the debug shape. */

	std::vector<std::vector<glm::vec3>> _points; /** The vertices of the debug shape. */

	bool _drawLine;	   /** Whether to draw the debug shape as a line or as dots. */
	bool _ignoreDepth; /** Whether to ignore the depth buffer when rendering the debug shape. */
};

} // namespace Stone::Scene
