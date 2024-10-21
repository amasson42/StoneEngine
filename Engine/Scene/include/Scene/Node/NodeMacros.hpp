// Copyright 2024 Stone-Engine

#pragma once

#include <string>

#define __STONE_NODE_BASE(NewClassName)                                                                                \
                                                                                                                       \
public:                                                                                                                \
	static const std::string nodeClassName;                                                                            \
	const char *getNodeClassName() const override;

/**
 * @brief Macro to use inside an abstract node class to make the engine reconize this class as a node.
 */
#define STONE_ABSTRACT_NODE(NewClassName)                                                                              \
	STONE_ABSTRACT_OBJECT(NewClassName)                                                                                \
	__STONE_NODE_BASE(NewClassName)                                                                                    \
                                                                                                                       \
private:


/**
 * @brief Macro to use inside a node class to make the engine reconize this class as a node.
 */
#define STONE_NODE(NewClassName)                                                                                       \
	STONE_OBJECT(NewClassName)                                                                                         \
	__STONE_NODE_BASE(NewClassName)                                                                                    \
                                                                                                                       \
private:

/* Cpp Implementations */

#include "Utils/DynamicObjectFactory.hpp"

#define __STONE_NODE_IMPLEMENTATION_BASE(NewClassName)                                                                 \
	const char *NewClassName::getNodeClassName() const {                                                               \
		return NewClassName::nodeClassName.c_str();                                                                    \
	}

/**
 * @brief Macro to use inside the cpp implementation of a node class that have used the STONE_NODE macro in its class.
 */
#define STONE_NODE_IMPLEMENTATION(NewClassName)                                                                        \
	__STONE_NODE_IMPLEMENTATION_BASE(NewClassName)                                                                     \
                                                                                                                       \
	const std::string NewClassName::nodeClassName = [] {                                                               \
		auto constructor = [](const std::string &nodeName) {                                                           \
			return std::make_shared<NewClassName>(nodeName);                                                           \
		};                                                                                                             \
		Stone::DynamicObjectFactory<Stone::Scene::Node, const std::string &>::getInstance().add(#NewClassName,         \
																								constructor);          \
		return #NewClassName;                                                                                          \
	}();

/**
 * @brief Macro to use inside the cpp implementation of an abstract node class that have used the STONE_ABSTRACT_NODE
 * macro in its class.
 */
#define STONE_ABSTRACT_NODE_IMPLEMENTATION(NewClassName)                                                               \
	__STONE_NODE_IMPLEMENTATION_BASE(NewClassName)                                                                     \
                                                                                                                       \
	const std::string NewClassName::nodeClassName = #NewClassName;
