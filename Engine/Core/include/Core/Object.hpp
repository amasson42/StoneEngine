// Copyright 2024 Stone-Engine

#pragma once

#include "Utils/Json.hpp"

#include <memory>

namespace Stone::Core {

class Object : public std::enable_shared_from_this<Object> {
public:
	Object();
	Object(const Object &other);

	virtual ~Object() = default;

	uint32_t getId() const;

	const static char *StaticClassName() {
		return "Object";
	}

	static std::intptr_t StaticHashCode() {
		return reinterpret_cast<std::intptr_t>(StaticClassName());
	}

	virtual const char *getClassName() const = 0;

	std::intptr_t getClassHashCode() const {
		return reinterpret_cast<std::intptr_t>(getClassName());
	}

	virtual void writeToJson(Json::Object &json) const;

protected:
	uint32_t _id;
};

} // namespace Stone::Core

#define STONE_ABSTRACT_OBJECT(ClassName)                                                                               \
                                                                                                                       \
public:                                                                                                                \
	static const char *StaticClassName() {                                                                             \
		return #ClassName;                                                                                             \
	}                                                                                                                  \
	static std::intptr_t StaticHashCode() {                                                                            \
		return reinterpret_cast<std::intptr_t>(StaticClassName());                                                     \
	}                                                                                                                  \
                                                                                                                       \
private:

#define STONE_OBJECT(ClassName)                                                                                        \
	STONE_ABSTRACT_OBJECT(ClassName)                                                                                   \
                                                                                                                       \
public:                                                                                                                \
	const char *getClassName() const override {                                                                        \
		return StaticClassName();                                                                                      \
	}                                                                                                                  \
                                                                                                                       \
private:


std::ostream &operator<<(std::ostream &os, const Stone::Core::Object &obj);
