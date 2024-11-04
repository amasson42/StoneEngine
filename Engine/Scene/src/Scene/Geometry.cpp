// Copyright 2024 Stone-Engine

#include "Scene/Geometry.hpp"

namespace Stone::Scene {

std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Plane &plane, float size) {
	std::vector<uint32_t> indices = {0, 1, 2, 0, 2, 3};
	std::vector<glm::vec3> vertices = {
		plane.normal * size + glm::vec3(0.0f, 0.0f, plane.distance),
		plane.normal * size + glm::vec3(size, 0.0f, plane.distance),
		plane.normal * size + glm::vec3(size, size, plane.distance),
		plane.normal * size + glm::vec3(0.0f, size, plane.distance),
	};
	return {indices, vertices};
}

std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Sphere &sphere, int rings) {
	std::vector<uint32_t> indices;
	std::vector<glm::vec3> vertices;

	const float phiStep = (float)M_PI / (float)rings;
	const float thetaStep = 2.0f * (float)M_PI / (float)rings;

	for (int i = 0; i <= rings; i++) {
		const float phi = (float)i * phiStep;
		for (int j = 0; j <= rings; j++) {
			const float theta = (float)j * thetaStep;

			const float x = sphere.radius * std::sin(phi) * std::cos(theta);
			const float y = sphere.radius * std::cos(phi);
			const float z = sphere.radius * std::sin(phi) * std::sin(theta);

			vertices.push_back(sphere.center + glm::vec3(x, y, z));
		}
	}

	for (int i = 0; i < rings; i++) {
		for (int j = 0; j < rings; j++) {
			const int i0 = i * (rings + 1) + j;
			const int i1 = i0 + 1;
			const int i2 = i0 + rings + 1;
			const int i3 = i2 + 1;

			indices.push_back(i0);
			indices.push_back(i1);
			indices.push_back(i2);

			indices.push_back(i2);
			indices.push_back(i1);
			indices.push_back(i3);
		}
	}

	return {indices, vertices};
}

std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Box &box) {
	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0, // Front
		4, 5, 6, 6, 7, 4, // Back
		0, 4, 7, 7, 3, 0, // Left
		1, 5, 6, 6, 2, 1, // Right
		0, 1, 5, 5, 4, 0, // Top
		3, 2, 6, 6, 7, 3, // Bottom
	};
	std::vector<glm::vec3> vertices = {
		box.min,
		glm::vec3(box.max.x, box.min.y, box.min.z),
		glm::vec3(box.max.x, box.max.y, box.min.z),
		glm::vec3(box.min.x, box.max.y, box.min.z),
		glm::vec3(box.min.x, box.min.y, box.max.z),
		glm::vec3(box.max.x, box.min.y, box.max.z),
		box.max,
		glm::vec3(box.min.x, box.max.y, box.max.z),
	};
	return {indices, vertices};
}

std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Line &line) {
	std::vector<uint32_t> indices = {0, 1};
	std::vector<glm::vec3> vertices = {line.origin, line.origin + line.direction};
	return {indices, vertices};
}

std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Cone &cone, int segments) {
	std::vector<uint32_t> indices;
	std::vector<glm::vec3> vertices;

	const float angleStep = cone.angle / (float)segments;
	const float halfLength = cone.length / 2.0f;

	for (int i = 0; i <= segments; i++) {
		const float angle = (float)i * angleStep;
		const float x = cone.length * std::cos(angle);
		const float y = cone.length * std::sin(angle);

		vertices.push_back(cone.origin + glm::vec3(x, y, -halfLength));
		vertices.push_back(cone.origin + glm::vec3(x, y, halfLength));
	}

	for (int i = 0; i < segments; i++) {
		const int i0 = i * 2;
		const int i1 = i0 + 1;
		const int i2 = i0 + 2;
		const int i3 = i2 + 1;

		indices.push_back(i0);
		indices.push_back(i1);
		indices.push_back(i2);

		indices.push_back(i2);
		indices.push_back(i1);
		indices.push_back(i3);
	}

	return {indices, vertices};
}

std::pair<std::vector<uint32_t>, std::vector<glm::vec3>> generateGeometryMesh(const Frustum &frustum) {
	std::vector<uint32_t> indices;
	std::vector<glm::vec3> vertices;

	for (auto plane : frustum.planes) {
		auto [planeIndices, planeVertices] = generateGeometryMesh(plane, 1.0f);
		const auto offset = static_cast<uint32_t>(vertices.size());
		for (const auto &index : planeIndices) {
			indices.push_back(index + offset);
		}
		for (const auto &vertex : planeVertices) {
			vertices.push_back(vertex);
		}
	}

	return {indices, vertices};
}

} // namespace Stone::Scene
