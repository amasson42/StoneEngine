// Copyright 2024 Stone-Engine

#include "Core/Image/ImageData.hpp"

#include "Utils/Glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>
#include <stb_image.h>

namespace Stone::Core::Image {

ImageData::~ImageData() {
	if (_data) {
		stbi_image_free(const_cast<uint8_t *>(_data));
	}
}

void ImageData::writeToJson(Json::Object &json) const {
	Object::writeToJson(json);

	json["size"] = to_json(_size);
	json["channels"] = Json::number(_channels);
}

const Size &ImageData::getSize() const {
	return _size;
}

Channel ImageData::getChannels() const {
	return static_cast<Channel>(_channels);
}

const uint8_t *ImageData::getData() const {
	return _data;
}

std::shared_ptr<ImageSource> ImageData::getSource() const {
	return _source.lock();
}

ImageData::ImageData(const std::string &filepath, Channel channels) {
	_data = stbi_load(filepath.c_str(), &_size.x, &_size.y, &_channels, static_cast<int>(channels));
	if (_data == nullptr) {
		throw std::runtime_error("Failed to load image: " + filepath +
								 " with channels: " + std::to_string(static_cast<int>(channels)));
	}
	assert(_channels >= 1 && _channels <= 4);
}

} // namespace Stone::Core::Image
