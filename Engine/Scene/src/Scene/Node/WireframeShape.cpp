// Copyright 2024 Stone-Engine

#include "Scene/Node/WireframeShape.hpp"

#include "Utils/Glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(WireframeShape)

WireframeShape::WireframeShape(const std::string &name)
	: RenderableNode(name), _color(glm::vec3(1.0f)), _thickness(1.0f), _points(), _drawLine(true), _ignoreDepth(false) {
}

void WireframeShape::writeToJson(Json::Object &json) const {
	RenderableNode::writeToJson(json);

	json["color"] = to_json(_color);
	json["thickness"] = Json::number(_thickness);

	auto &points_json((json["points"] = Json::array()).get<Json::Array>());
	for (const auto &line : _points) {
		auto &line_json((points_json.emplace_back() = Json::array()).get<Json::Array>());

		for (const auto &point : line) {
			line_json.push_back(to_json(point));
		}
	}

	json["draw_line"] = Json::boolean(_drawLine);
	json["ignore_depth"] = Json::boolean(_ignoreDepth);
}

glm::vec3 WireframeShape::getColor() const {
	return _color;
}

void WireframeShape::setColor(const glm::vec3 &color) {
	_color = color;
	markDirty();
}

float WireframeShape::getThickness() const {
	return _thickness;
}

void WireframeShape::setThickness(float thickness) {
	_thickness = thickness;
	markDirty();
}

const std::vector<std::vector<glm::vec3>> &WireframeShape::getPoints() const {
	return _points;
}

void WireframeShape::withPointsRef(const std::function<void(std::vector<std::vector<glm::vec3>> &)> &func) {
	func(_points);
	markDirty();
}

bool WireframeShape::isDrawingLine() const {
	return _drawLine;
}

void WireframeShape::setDrawingLine(bool drawLine) {
	_drawLine = drawLine;
	markDirty();
}

bool WireframeShape::isIgnoringDepth() const {
	return _ignoreDepth;
}

void WireframeShape::setIgnoringDepth(bool ignoreDepth) {
	_ignoreDepth = ignoreDepth;
	markDirty();
}

}; // namespace Stone::Scene
