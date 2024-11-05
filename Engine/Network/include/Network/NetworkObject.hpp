// Copyright 2024 Stone-Engine

#include "Core/Object.hpp"
#include "Network/ObjectPool.hpp"
#include "Utils/SigSlot.hpp"

namespace Stone::Network {

class NetworkObject : public Core::Object {
	STONE_OBJECT(NetworkObject)

public:
	NetworkObject() = default;
	NetworkObject(const NetworkObject &other) = default;

	~NetworkObject() override = default;

	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	ObjectPool<NetworkObject>::Id getPoolId() const;

	Signal<const NetworkObject &, const std::uint8_t *, std::size_t> onReceivingData;
	Signal<const NetworkObject &, const std::uint8_t *, std::size_t> onSendingData;

protected:
	virtual void receiveData(const std::uint8_t *data, std::size_t size);

	virtual void sendData(const std::uint8_t *data, std::size_t &size) const;

private:
	ObjectPool<NetworkObject>::Id _poolId = 0;

	friend class NetworkSession;
};

} // namespace Stone::Network
