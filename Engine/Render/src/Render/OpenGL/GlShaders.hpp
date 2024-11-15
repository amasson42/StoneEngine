// Copyright 2024 Stone-Engine

#pragma once

#include "Renderable/Texture.hpp"
#include "Scene/Renderable/Material.hpp"
#include "Scene/Renderable/Shader.hpp"
#include "Scene/Shader/ShaderParameters.hpp"
#include "Utils/FileSystem.hpp"

#include <GL/glew.h>

namespace Stone::Render::OpenGL {

GLuint compileSource(const std::string &source, GLenum type);

GLuint loadSpirv(const char *spirv_content, GLsizei spirv_length, GLenum type);

class GlShaderBase {
public:
	GlShaderBase(const Scene::AShader &shader, GLenum type) {

		auto [contentType, content] = shader.getContent();

		using ContentType = Scene::AShader::ContentType;
		switch (contentType) {
		case ContentType::SourceCode: _gl_shader = compileSource(content, type); break;
		case ContentType::SourceFile: _gl_shader = compileSource(Utils::readTextFile(content), type); break;
		case ContentType::CompiledCode: _gl_shader = loadSpirv(content.data(), content.size(), type); break;
		case ContentType::CompiledFile:
			auto fileContent = Utils::readBinaryFile(content);
			_gl_shader = loadSpirv(fileContent.data(), fileContent.size(), type);
			break;
		}
	}

	GlShaderBase(const char *source, GLenum type) {
		_gl_shader = compileSource(source, type);
	}

	virtual ~GlShaderBase() {
		if (_gl_shader != 0)
			glDeleteShader(_gl_shader);
	}

	GLuint getGLShader() const {
		return _gl_shader;
	}

protected:
	GLuint _gl_shader = 0;
};

class GlVertexShader : public GlShaderBase {
public:
	GlVertexShader(const char *source) : GlShaderBase(source, GL_VERTEX_SHADER) {
	}

	static std::unique_ptr<GlVertexShader> makeStandardMeshShader();
	static std::unique_ptr<GlVertexShader> makeStandardSkinMeshShader();
	static std::unique_ptr<GlVertexShader> makeStandardInstancedMeshShader();
};

class GlFragmentShader : public GlShaderBase {
public:
	GlFragmentShader(const Scene::AShader &shader) : GlShaderBase(shader, GL_FRAGMENT_SHADER) {
	}

	GlFragmentShader(const char *source) : GlShaderBase(source, GL_FRAGMENT_SHADER) {
	}

	static std::unique_ptr<GlFragmentShader> makeStandardShader(const Scene::ShaderParameters &params);
};

class GlShaderProgram {
public:
	GlShaderProgram(const GlVertexShader &vertexShader, const GlFragmentShader &fragmentShader);

	virtual ~GlShaderProgram();

	GLuint getGlProgram() const;

	void use() const;

	GLint getUniformLocation(const std::string &name) const;
	GLint getUniformLocation(const Scene::Material::Location &location) const;

	void setUniform(const Scene::Material::Location &location, float scalar) const;
	void setUniform(const Scene::Material::Location &location, const glm::vec3 &vec3) const;
	void setUniform(const Scene::Material::Location &location, const glm::mat4 &mat4) const;
	void setUniformTexture(const Scene::Material::Location &location, const Texture &texture, int textureIndex) const;

private:
	GLuint _gl_program;
};

} // namespace Stone::Render::OpenGL
