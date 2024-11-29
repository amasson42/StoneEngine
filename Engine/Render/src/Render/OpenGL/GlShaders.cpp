// Copyright 2024 Stone-Engine

#include "GlShaders.hpp"

#include "Scene/Shader/ShaderGenerator.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <sstream>

namespace Stone::Render::OpenGL {

GLuint compileSource(const std::string &source, GLenum type) {
	GLuint shaderId = glCreateShader(type);
	if (shaderId == 0) {
		throw std::runtime_error("Failed to create shader.");
	}

	const char *source_c = source.c_str();
	glShaderSource(shaderId, 1, &source_c, nullptr);
	glCompileShader(shaderId);

	GLint compiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint log_length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log(log_length);
		glGetShaderInfoLog(shaderId, log_length, &log_length, log.data());

		glDeleteShader(shaderId);

		throw std::runtime_error("Failed to compile shader: " + std::string(log.data()));
	}

	return shaderId;
}

GLuint loadSpirv(const char *spirv_content, GLsizei spirv_length, GLenum type) {
	(void)type;

	GLuint shaderId = glCreateShader(GL_VERTEX_SHADER);
	if (shaderId == 0) {
		throw std::runtime_error("Failed to create shader.");
	}

	glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, spirv_content, spirv_length);
	glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);

	int compiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint log_length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log(log_length);
		glGetShaderInfoLog(shaderId, log_length, &log_length, log.data());

		glDeleteShader(shaderId);

		throw std::runtime_error("Failed to compile shader: " + std::string(log.data()));
	}

	return shaderId;
}

const char *basicVertexShaderSource = R"shader(
#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 uv;

uniform mat3 u_mat_normal;
uniform mat4 u_mat_projection;
uniform mat4 u_mat_view;
uniform mat4 u_mat_model;
uniform vec3 u_camera_position;

out FRAG_DATA {
    vec3 wposition;
    vec2 uv;
    vec3 wnormal;
    vec3 wtangent;
    vec3 wbitangent;
} vs_out;

void main()
{
    vs_out.wposition = vec3(u_mat_model * vec4(position, 1.0));
    gl_Position = u_mat_projection * u_mat_view * vec4(vs_out.wposition, 1.0);
    vs_out.wnormal = u_mat_normal * normal;
    vs_out.wtangent = u_mat_normal * tangent;
    vs_out.wbitangent = u_mat_normal * bitangent;
    vs_out.uv = uv;
}

)shader";

const char *basicSkinVertexShaderSource = R"shader(
#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 uv;
layout(location = 5) in vec4 boneWeights;
layout(location = 6) in ivec4 boneIDs;

out vec3 fragNormal;
out vec2 fragUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

void main() {
    fragNormal = normal;
    fragUV = uv;

    mat4 boneTransform = mat4(0.0);
    for (int i = 0; i < 4; i++) {
        boneTransform += boneWeights[i] * bones[boneIDs[i]];
    }

    gl_Position = projection * view * model * boneTransform * vec4(position, 1.0);
}

)shader";

std::unique_ptr<GlVertexShader> GlVertexShader::makeStandardMeshShader() {
	auto glShader = std::make_unique<GlVertexShader>(basicVertexShaderSource);
	return glShader;
}

std::unique_ptr<GlVertexShader> GlVertexShader::makeStandardSkinMeshShader() {
	auto glShader = std::make_unique<GlVertexShader>(basicSkinVertexShaderSource);
	return glShader;
}

std::unique_ptr<GlVertexShader> GlVertexShader::makeStandardInstancedMeshShader() {
	throw std::runtime_error("Not implemented.");
}

std::unique_ptr<GlFragmentShader> GlFragmentShader::makeStandardShader(const Scene::ShaderParameters &params) {
	std::stringstream source;
	Scene::ShaderGenerator generator;
	generator.generateFragmentShader(params, source);
	return std::make_unique<GlFragmentShader>(source.str().c_str());
}


GlShaderProgram::GlShaderProgram(const GlVertexShader &vertexShader, const GlFragmentShader &fragmentShader)
	: _gl_program(0) {
	_gl_program = glCreateProgram();
	glAttachShader(_gl_program, vertexShader.getGLShader());
	glAttachShader(_gl_program, fragmentShader.getGLShader());
	glLinkProgram(_gl_program);

	GLint success;
	glGetProgramiv(_gl_program, GL_LINK_STATUS, &success);
	if (!success) {
		GLint log_length = 0;
		glGetProgramiv(_gl_program, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log(log_length);
		glGetProgramInfoLog(_gl_program, log_length, &log_length, log.data());

		glDeleteProgram(_gl_program);

		throw std::runtime_error("Failed to link program: " + std::string(log.data()));
	}
}

GlShaderProgram::~GlShaderProgram() {
	if (_gl_program != 0)
		glDeleteProgram(_gl_program);
}

GLuint GlShaderProgram::getGlProgram() const {
	return _gl_program;
}

void GlShaderProgram::use() const {
	glUseProgram(_gl_program);
}

GLint GlShaderProgram::getUniformLocation(const std::string &name) const {
	return glGetUniformLocation(_gl_program, name.c_str());
}

GLint GlShaderProgram::getUniformLocation(const Scene::Material::Location &location) const {
	if (std::holds_alternative<std::string>(location)) {
		const std::string &name(std::get<std::string>(location));
		return getUniformLocation(name);
	} else if (std::holds_alternative<int>(location)) {
		return std::get<int>(location);
	}
	return -1;
}

void GlShaderProgram::setUniform(const Scene::Material::Location &location, float scalar) const {
	glUniform1f(getUniformLocation(location), scalar);
}

void GlShaderProgram::setUniform(const Scene::Material::Location &location, const glm::vec3 &vec3) const {
	glUniform3fv(getUniformLocation(location), 1, glm::value_ptr(vec3));
}

void GlShaderProgram::setUniform(const Scene::Material::Location &location, const glm::mat4 &mat4) const {
	glUniformMatrix4fv(getUniformLocation(location), 1, GL_FALSE, glm::value_ptr(mat4));
}

void GlShaderProgram::setUniformTexture(const Scene::Material::Location &location, const Texture &texture,
										int textureIndex) const {
	assert(textureIndex >= 0 && textureIndex < 32);
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, texture.getGlTexture());
	glUniform1i(getUniformLocation(location), texture.getGlTexture());
}


} // namespace Stone::Render::OpenGL
