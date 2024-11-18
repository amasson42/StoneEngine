// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/Renderable/IRenderable.hpp"

namespace Stone::Scene {

/**
 * @brief The Shader class represents a shader used in rendering.
 */
class Shader : public Core::Object, public IRenderable {
	STONE_OBJECT(Shader);

public:
	enum class ContentType {
		SourceCode,	  /** The content is the full shader code in a readable shading language (cf. `.glsl`, `.metal`) */
		SourceFile,	  /** The content is a link to a file containing the readable code file. */
		CompiledCode, /** The content is a compiled executable bytes buffer (cf. spir-v) */
		CompiledFile, /** The content is a link to a file containing the compiled bytes. */
	};

	Shader() = default;
	explicit Shader(const std::string &content);
	Shader(ContentType contentType, std::string content);
	Shader(const Shader &other) = default;

	~Shader() override = default;

	/**
	 * @brief Write the Shader object to an output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write a closing brace after the object.
	 * @return The output stream after writing the Shader object.
	 */
	void writeToJson(Json::Object &json) const override;

	/**
	 * @brief Get the content of the shader paired with its type. See `Stone::Scene::Shader::ContentType` for more
	 * information.
	 */
	[[nodiscard]] std::pair<ContentType, const std::string &> getContent() const;

	/**
	 * @brief Get the function to call in the shader.
	 */
	[[nodiscard]] const std::string &getFunction() const;

	/**
	 * @brief Set the function to call in the shader.
	 *
	 * @param function The function to call in the shader.
	 */
	void setFunction(const std::string &function);

	/**
	 * @brief Get the maximum location used in the shader.
	 */
	[[nodiscard]] int getMaxLocation() const;

	/**
	 * @brief Get the location of a variable in the shader.
	 *
	 * @param name The name of the variable.
	 * @return The location of the variable.
	 */
	[[nodiscard]] int getLocation(const std::string &name) const;

	/**
	 * @brief Set the content of the shader paired with its type. See `Stone::Scene::Shader::ContentType` for more
	 * information.
	 *
	 * @param contentType The type of the content.
	 * @param content The content of the shader.
	 */
	void setContent(ContentType contentType, std::string content);

	/**
	 * @brief Set the location of a variable in the shader.
	 *
	 * @param name The name of the variable.
	 * @param location The location of the variable.
	 */
	void setLocation(const std::string &name, int location);

private:
	ContentType _contentType = ContentType::SourceCode; /** The type of the content. */
	std::string _content = "#version 450 core\n";		/** The content of the shader. */
	std::string _function = "main";						/** The function to call in the shader. */

	std::unordered_map<std::string, int> _locations = {}; /** The binding locations of the variables in the shader. */
	int _maxLocation = -1;								  /** The cached maximum value from the locations. */
};

} // namespace Stone::Scene
