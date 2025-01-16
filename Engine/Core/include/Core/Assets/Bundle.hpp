// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Assets/Resource.hpp"
#include "Core/Object.hpp"

namespace Stone::Core::Assets {

class Bundle : public Object {
	STONE_OBJECT(Bundle)

public:
	Bundle(const Bundle &other) = delete;

	explicit Bundle(std::string rootDirectory = "./");

	~Bundle() override = default;

	void writeToJson(Json::Object &json) const override;

	template <typename ResourceType, typename... Args>
	std::shared_ptr<ResourceType> loadResource(const std::string &filepath, Args... args) {
		const std::string &reducedPath = reducePath(filepath);
		auto it = _resources.find(reducedPath);
		if (it != _resources.end()) {
			return std::static_pointer_cast<ResourceType>(it->second);
		}
		auto thisBundle = std::static_pointer_cast<Bundle>(shared_from_this());
		auto resource = std::make_shared<ResourceType>(thisBundle, reducedPath, std::forward<Args>(args)...);
		_resources[reducedPath] = resource;
		return resource;
	}

	std::shared_ptr<Resource> getResource(const std::string &filepath) const;

	const std::string &getRootDirectory() const;

	static std::string reducePath(const std::string &path);

protected:
	/**
	 * @brief The root directory of the bundle
	 */
	std::string _rootDirectory;

	/**
	 * @brief The map of resources indexed by their filename shortned path
	 */
	std::unordered_map<std::string, std::shared_ptr<Resource>> _resources;
};

} // namespace Stone::Core::Assets
