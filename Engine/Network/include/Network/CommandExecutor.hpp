// Copyright 2024 Stone-Engine

#pragma once

#include <memory>

namespace Stone::Network {

template <typename ObjectType>
class CommandExecutor {
public:
	using Command = std::function<void(const std::shared_ptr<ObjectType> &)>;

	CommandExecutor() = default;
	CommandExecutor(const CommandExecutor &) = delete;

	virtual ~CommandExecutor() = default;

	void execute(const Command &command, const std::shared_ptr<ObjectType> &object) {
		command(object);
	}
};

} // namespace Stone::Network
