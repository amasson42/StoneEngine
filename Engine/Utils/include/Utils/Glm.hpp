// Copyright 2024 Stone-Engine

#pragma once

#include <glm/glm.hpp>

namespace glm {
float &value_at(mat3 &m, int r, int c);
float &value_at(mat4 &m, int r, int c);
} // namespace glm

namespace Json {
struct Value;
} // namespace Json

namespace Stone {

Json::Value to_json(const glm::vec2 &v);
Json::Value to_json(const glm::vec3 &v);
Json::Value to_json(const glm::ivec2 &v);
Json::Value to_json(const glm::ivec3 &v);
Json::Value to_json(const glm::mat3 &m);
Json::Value to_json(const glm::mat4 &m);
Json::Value to_json(const glm::quat &q);

} // namespace Stone
