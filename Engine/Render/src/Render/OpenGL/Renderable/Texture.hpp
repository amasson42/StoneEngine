// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Image/ImageData.hpp"
#include "Core/Image/ImageSource.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"
#include "Scene/Renderable/Texture.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

GLuint convert(Core::Image::Channel channel);

GLuint convert(Scene::TextureFilter filter, bool mipmap);

GLuint convert(Scene::TextureWrap wrap);


class Texture : public Scene::IRendererObject {
public:
	Texture(Scene::Texture &texture, const std::shared_ptr<OpenGLRenderer> &renderer) : _gl_texture(0) {
		(void)renderer;

		std::shared_ptr<Core::Image::ImageSource> imageSource = texture.getImage();
		if (imageSource == nullptr) {
			return;
		}

		imageSource->loadData();
		std::shared_ptr<Core::Image::ImageData> imageData = imageSource->getLoadedImage();

		GLuint format = convert(imageData->getChannels());
		glGenTextures(1, &_gl_texture);
		if (_gl_texture == 0) {
			std::runtime_error("Failed to create texture buffer.");
		}
		glBindTexture(GL_TEXTURE_2D, _gl_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, imageData->getSize().x, imageData->getSize().y, 0, format,
					 GL_UNSIGNED_BYTE, imageData->getData());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convert(texture.getWrap()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, convert(texture.getWrap()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convert(texture.getMinFilter(), true));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convert(texture.getMagFilter(), false));
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture(GLsizei width, GLsizei height, GLint internalFormat, GLenum format, GLenum type, GLint wrap, GLint filter) {
		glGenTextures(1, &_gl_texture);
		if (_gl_texture == 0) {
			std::runtime_error("Failed to create texture buffer.");
		}
		glBindTexture(GL_TEXTURE_2D, _gl_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	}

	~Texture() override {
		if (_gl_texture != 0) {
			glDeleteTextures(1, &_gl_texture);
		}
	}

	void render(Scene::RenderContext &context) override {
		(void)context;
	}

	GLuint getGlTexture() const {
		return _gl_texture;
	}

private:
	GLuint _gl_texture = 0;
};

} // namespace Stone::Render::OpenGL
