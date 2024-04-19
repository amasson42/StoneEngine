// Copyright 2024 Stone-Engine

#include "core/Object.hpp"

namespace Stone
{

    namespace Core
    {

        Object::Object()
            : std::enable_shared_from_this<Object>()
        {
            static uint32_t id = 0;
            _id = id++;
        }

        Object::Object(const Object &other)
            : std::enable_shared_from_this<Object>()
        {
            _id = other._id;
        }

        Object::~Object()
        {
        }

        uint32_t Object::getId() const
        {
            return _id;
        }

        const char *Object::getClassName() const
        {
            return "Object";
        }

        std::string Object::debugDescription() const
        {
            return "{id:" + std::to_string(_id) + "}";
        }

    }

}