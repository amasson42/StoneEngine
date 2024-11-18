// Copyright 2024 Stone-Engine

#include "Utils/Glm.hpp"

#include "Utils/Json.hpp"

#include <glm/ext.hpp>

namespace glm {

float &value_at(mat3 &m, int r, int c) {
	return value_ptr(m)[3 * c + r];
}

float &value_at(mat4 &m, int r, int c) {
	return value_ptr(m)[4 * c + r];
}

} // namespace glm

namespace Stone {

Json::Value to_json(const glm::vec2 &v) {
	return Json::array({
		Json::number(v.x),
		Json::number(v.y),
	});
}

Json::Value to_json(const glm::vec3 &v) {
	return Json::array({
		Json::number(v.x),
		Json::number(v.y),
		Json::number(v.z),
	});
}

Json::Value to_json(const glm::ivec2 &v) {
	return Json::array({
		Json::number(v.x),
		Json::number(v.y),
	});
}

Json::Value to_json(const glm::ivec3 &v) {
	return Json::array({
		Json::number(v.x),
		Json::number(v.y),
		Json::number(v.z),
	});
}

Json::Value to_json(const glm::mat3 &m) {
	Json::Value json = Json::array();
	Json::Array &values(json.get<Json::Array>());
	for (int i = 0; i < 9; ++i) {
		values.push_back(Json::number(glm::value_ptr(m)[i]));
	}
	return json;
}

Json::Value to_json(const glm::mat4 &m) {
	Json::Value json = Json::array();
	Json::Array &values(json.get<Json::Array>());
	for (int i = 0; i < 16; ++i) {
		values.push_back(Json::number(glm::value_ptr(m)[i]));
	}
	return json;
}

Json::Value to_json(const glm::quat &q) {
	return Json::array({
		Json::number(q.x),
		Json::number(q.y),
		Json::number(q.z),
		Json::number(q.w),
	});
}

} // namespace Stone
