// Copyright 2024 Stone-Engine

#include "Texture.hpp"

namespace Stone::Render::OpenGL {

GLuint convert(Core::Image::Channel channel) {
	switch (channel) {
	case Core::Image::Channel::GREY: return GL_RED;
	case Core::Image::Channel::DUAL: return GL_RG;
	case Core::Image::Channel::RGB: return GL_RGB;
	case Core::Image::Channel::RGBA: return GL_RGBA;
	default: return GL_RGBA;
	}
}

GLuint convert(Scene::TextureFilter filter, bool mipmap) {
	switch (filter) {
	case Scene::TextureFilter::Nearest: return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
	case Scene::TextureFilter::Linear: return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	case Scene::TextureFilter::Cubic: return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	default: return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	}
}

GLuint convert(Scene::TextureWrap wrap) {
	switch (wrap) {
	case Scene::TextureWrap::Repeat: return GL_REPEAT;
	case Scene::TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
	case Scene::TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
	case Scene::TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
	default: return GL_REPEAT;
	}
}

} // namespace Stone::Render::OpenGL
