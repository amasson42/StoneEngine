// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/Renderable/IRenderable.hpp"

namespace Stone::Core::Image {
class ImageSource;
}

namespace Stone::Scene {

enum class TextureFilter {
	Nearest,
	Linear,
	Cubic,
};

enum class TextureWrap {
	Repeat,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder,
};

/**
 * @brief The Texture class represents a texture used in rendering.
 */
class Texture : public Core::Object, public IRenderable {
	STONE_OBJECT(Texture);

public:
	Texture() = default;
	Texture(const Texture &other) = default;

	~Texture() override = default;

	/**
	 * @brief Write the Texture object to an output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write a closing brace after the object.
	 * @return The output stream after writing the Texture object.
	 */
	void writeToJson(Json::Object &json) const override;

	/**
	 * @brief Set the image associated with the Texture.
	 *
	 * @param image The image to set.
	 */
	void setImage(const std::shared_ptr<Core::Image::ImageSource> &image);

	/**
	 * @brief Get the image associated with the Texture.
	 */
	[[nodiscard]] const std::shared_ptr<Core::Image::ImageSource> &getImage() const;

	/**
	 * @brief Set the wrap mode for the Texture.
	 *
	 * @param wrap The wrap mode to set.
	 */
	void setWrap(TextureWrap wrap);

	/**
	 * @brief Get the wrap mode for the Texture.
	 */
	[[nodiscard]] TextureWrap getWrap() const;

	/**
	 * @brief Set the minification filter for the Texture.
	 *
	 * @param filter The minification filter to set.
	 */
	void setMinFilter(TextureFilter filter);

	/**
	 * @brief Get the minification filter for the Texture.
	 */
	[[nodiscard]] TextureFilter getMinFilter() const;

	/**
	 * @brief Set the magnification filter for the Texture.
	 *
	 * @param filter The magnification filter to set.
	 */
	void setMagFilter(TextureFilter filter);

	/**
	 * @brief Get the magnification filter for the Texture.
	 */
	[[nodiscard]] TextureFilter getMagFilter() const;

protected:
	std::shared_ptr<Core::Image::ImageSource> _image; /**< The image associated with the Texture. */

	TextureWrap _wrap = TextureWrap::Repeat;		  /**< The wrap mode for the Texture. */
	TextureFilter _minFilter = TextureFilter::Linear; /**< The minification filter for the Texture. */
	TextureFilter _magFilter = TextureFilter::Linear; /**< The magnification filter for the Texture. */
};

} // namespace Stone::Scene
