// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Assets/Resource.hpp"
#include "ImageTypes.hpp"

namespace Stone::Core::Image {

class ImageData;

/**
 * @brief ImageSource class. This class does not contain any image data,
 * it is used to hold reference to the image file and the channels of the image.
 * It is used to load the image data when needed and hold the reference to the loaded image data.
 */
class ImageSource : public Assets::Resource {
	STONE_OBJECT(ImageSource)

public:
	ImageSource(const std::shared_ptr<Assets::Bundle> &bundle, const std::string &filepath,
				Channel channels = Channel::RGBA);

	ImageSource() = delete;
	ImageSource(const ImageSource &other) = delete;

	~ImageSource() override = default;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] const std::string &getFilePath() const;
	[[nodiscard]] Channel getChannels() const;
	[[nodiscard]] Size getSize() const;

	void unloadData();
	void loadData(bool force);

	[[nodiscard]] bool isLoaded() const;
	[[nodiscard]] std::shared_ptr<ImageData> getLoadedImage() const;
	[[nodiscard]] std::shared_ptr<ImageData> getLoadedImage(bool loadIfNeeded = false);

protected:
	std::string _filepath;
	Channel _channels;
	Size _size = Size(0);

	std::shared_ptr<ImageData> _loadedImage = nullptr;
};

} // namespace Stone::Core::Image
