// Copyright 2024 Stone-Engine

#include "Network/NetworkObject.hpp"

namespace Stone::Network {

std::ostream &NetworkObject::writeToStream(std::ostream &stream, bool closing_bracer) const {
	Core::Object::writeToStream(stream, false);
	stream << ",poolId:" << _poolId;
	if (closing_bracer) {
		stream << "}";
	}
	return stream;
}

ObjectPool<NetworkObject>::Id NetworkObject::getPoolId() const {
	return _poolId;
}

void NetworkObject::receiveData(const std::uint8_t *data, std::size_t size) {
	onReceivingData.broadcast(*this, data, size);
}

void NetworkObject::sendData(const std::uint8_t *data, std::size_t &size) const {
	onSendingData.broadcast(*this, data, size);
}


} // namespace Stone::Network
