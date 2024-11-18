// Copyright 2024 Stone-Engine

#include "Core/Image/ImageSource.hpp"

#include "Core/Assets/Bundle.hpp"
#include "Core/Image/ImageData.hpp"

namespace Stone::Core::Image {

ImageSource::ImageSource(const std::shared_ptr<Assets::Bundle> &bundle, const std::string &filepath, Channel channels)
	: Assets::Resource(bundle, filepath), _channels(channels) {
}

void ImageSource::writeToJson(Json::Object &json) const {
	Assets::Resource::writeToJson(json);

	json["path"] = Json::string(_filepath);
	json["channels"] = Json::number(static_cast<int>(_channels));
}

const std::string &ImageSource::getFilePath() const {
	return _filepath;
}

Channel ImageSource::getChannels() const {
	return _channels;
}

Size ImageSource::getSize() const {
	return _size;
}

void ImageSource::loadData(bool force) {
	if (force || _loadedImage == nullptr) {
		_loadedImage = std::make_shared<ImageData>(getFullPath(), _channels);
		_loadedImage->_source = std::static_pointer_cast<ImageSource>(shared_from_this());
		_channels = _loadedImage->getChannels();
		_size = _loadedImage->getSize();
	}
}

void ImageSource::unloadData() {
	_loadedImage = nullptr;
}

bool ImageSource::isLoaded() const {
	return _loadedImage != nullptr;
}

std::shared_ptr<ImageData> ImageSource::getLoadedImage() const {
	return _loadedImage;
}

std::shared_ptr<ImageData> ImageSource::getLoadedImage(bool loadIfNeeded) {
	if (loadIfNeeded && !isLoaded()) {
		loadData(false);
	}
	return _loadedImage;
}

} // namespace Stone::Core::Image
