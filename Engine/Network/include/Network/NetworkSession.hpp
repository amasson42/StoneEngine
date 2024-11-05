// Copyright 2024 Stone-Engine

#pragma once

#include "Network/NetworkObject.hpp"
#include "Network/ObjectPool.hpp"

namespace Stone::Network {

class NetworkSession {
public:
	NetworkSession() = default;

	ObjectPool<NetworkObject> &getObjectPool() {
		return _objectPool;
	}

private:
	// TODO: Add CommunicationDevice class, which will be used to send and receive data

	// TODO: Add CommandExecutor class, which will be used to execute commands

	ObjectPool<NetworkObject> _objectPool;
};

} // namespace Stone::Network
