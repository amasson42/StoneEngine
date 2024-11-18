// Copyright 2024 Stone-Engine

#include "Core/Object.hpp"

namespace Stone::Core {

Object::Object() : std::enable_shared_from_this<Object>() {
	static uint32_t id = 0;
	_id = id++;
}

Object::Object(const Object &other) : std::enable_shared_from_this<Object>(other) {
	_id = other._id;
}

uint32_t Object::getId() const {
	return _id;
}

void Object::writeToJson(Json::Object &json) const {
	json["class"] = Json::string(getClassName());
	json["id"] = Json::number(getId());
}

} // namespace Stone::Core

std::ostream &operator<<(std::ostream &os, const Stone::Core::Object &obj) {
	Stone::Json::Value json = Stone::Json::object();
	obj.writeToJson(json.get<Stone::Json::Object>());
	return os << json;
}
