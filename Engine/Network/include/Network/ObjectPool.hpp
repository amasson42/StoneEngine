// Copyright 2024 Stone-Engine

#pragma once

#include <memory>
#include <vector>

namespace Stone::Network {

template <typename ObjectType>
class ObjectPool {
public:
	using Id = unsigned int;

	ObjectPool() : _lastUsableId(1), _objects() {
		_incrementLastUsableId();
	};

	ObjectPool(const ObjectPool &) = delete;

	virtual ~ObjectPool() = default;

	Id add(const std::shared_ptr<ObjectType> &object) {
		Id id = _makeUsableId();
		_objects[id] = object;
		return id;
	}

	void set(Id id, const std::shared_ptr<ObjectType> &object) {
		if (id >= _objects.size()) {
			_objects.resize(id + 1);
		}
		_objects[id] = object;
		_incrementLastUsableId();
	}

	std::shared_ptr<ObjectType> get(Id id) const {
		return _objects[id].lock();
	}

	const std::weak_ptr<ObjectType> &weak_get(Id id) const {
		return _objects[id];
	}

	void refreshId() {
		_lastUsableId = 1;
		_incrementLastUsableId();
	}

private:
	Id _makeUsableId() {
		Id id = _lastUsableId++;
		_incrementLastUsableId();
		return id;
	}

	void _incrementLastUsableId() {
		while (_lastUsableId < _objects.size() && !_objects[_lastUsableId].expired()) {
			++_lastUsableId;
		}
		if (_lastUsableId >= _objects.size()) {
			_objects.resize(_lastUsableId + 1);
		}
	}

	Id _lastUsableId; // The last id that is directly usable for the next object

	std::vector<std::weak_ptr<ObjectType>> _objects; // The objects in the pool
};

} // namespace Stone::Network
