// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "ImageTypes.hpp"

namespace Stone::Core::Image {

class ImageSource;

class ImageData : public Object {
	STONE_OBJECT(ImageData)

public:
	ImageData() = delete;
	ImageData(const ImageData &other) = delete;

	~ImageData() override;

	void writeToJson(Json::Object &json) const override;

	[[nodiscard]] const Size &getSize() const;
	[[nodiscard]] Channel getChannels() const;
	[[nodiscard]] const uint8_t *getData() const;

	std::shared_ptr<ImageSource> getSource() const;

	ImageData(const std::string &filepath, Channel channels);

protected:
	Size _size = Size(0);
	int _channels = 0;
	uint8_t *_data = nullptr;

	std::weak_ptr<ImageSource> _source;

	friend ImageSource;
};

} // namespace Stone::Core::Image
