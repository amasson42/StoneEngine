// Copyright 2024 Stone-Engine

#include "Scene/Node/WireframeShape.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>

namespace Stone::Scene {

STONE_NODE_IMPLEMENTATION(WireframeShape)

WireframeShape::WireframeShape(const std::string &name)
	: RenderableNode(name), _color(glm::vec3(1.0f)), _thickness(1.0f), _points(), _drawLine(true), _ignoreDepth(false) {
}

std::ostream &WireframeShape::writeToStream(std::ostream &stream, bool closing_bracer) const {
	RenderableNode::writeToStream(stream, false);
	stream << ",color:" << _color;
	stream << ",thickness:" << _thickness;
	stream << ",points:[";
	for (std::size_t i = 0; i < _points.size(); i++) {
		if (i > 0)
			stream << ",";
		stream << "[";
		for (std::size_t j = 0; j < _points[i].size(); j++) {
			if (j > 0)
				stream << ",";
			stream << _points[i][j];
		}
		stream << "]";
	}
	stream << "],drawLine:" << _drawLine;
	stream << ",ignoreDepth:" << _ignoreDepth;
	if (closing_bracer)
		stream << "}";
	return stream;
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
