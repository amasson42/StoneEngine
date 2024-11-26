// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/Renderable/IRenderable.hpp"

#include <functional>
#include <glm/vec3.hpp>

namespace Stone::Scene {

class Texture;
class Shader;

/**
 * @brief The Material class represents a material used for rendering objects in the scene.
 *
 * It contains various parameters such as textures, vectors, and scalars that can be used
 * to configure the appearance of rendered objects.
 */
class Material : public Core::Object, public IRenderable {
	STONE_OBJECT(Material)

public:
	Material() = default;
	Material(const Material &other) = default;

	~Material() override = default;

	/**
	 * @brief Write the Material object to an output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write a closing bracer after writing the Material object.
	 * @return The output stream after writing the Material object.
	 */
	void writeToJson(Json::Object &json) const override;

	/**
	 * @brief Set a texture parameter for the Material.
	 *
	 * @param name The name of the texture parameter.
	 * @param texture The texture to set.
	 */
	void setTextureParameter(const std::string &name, std::shared_ptr<Texture> texture);

	/**
	 * @brief Get a texture parameter from the Material.
	 *
	 * @param name The name of the texture parameter.
	 * @return The texture parameter as a shared pointer to Texture.
	 */
	[[nodiscard]] std::shared_ptr<Texture> getTextureParameter(const std::string &name) const;

	/**
	 * @brief Set a vector parameter for the Material.
	 *
	 * @param name The name of the vector parameter.
	 * @param vector The vector to set.
	 */
	void setVectorParameter(const std::string &name, const glm::vec3 &vector);

	/**
	 * @brief Get a vector parameter from the Material.
	 *
	 * @param name The name of the vector parameter.
	 * @return The vector parameter as a glm::vec3.
	 */
	[[nodiscard]] glm::vec3 getVectorParameter(const std::string &name) const;

	/**
	 * @brief Set a scalar parameter for the Material.
	 *
	 * @param name The name of the scalar parameter.
	 * @param scalar The scalar value to set.
	 */
	void setScalarParameter(const std::string &name, float scalar);

	/**
	 * @brief Get a scalar parameter from the Material.
	 *
	 * @param name The name of the scalar parameter.
	 * @return The scalar parameter as a float.
	 */
	[[nodiscard]] float getScalarParameter(const std::string &name) const;

	/**
	 * @brief Iterate over all texture parameters in the Material.
	 *
	 * @param lambda The lambda function to call for each texture parameter.
	 */
	void forEachTextures(const std::function<void(std::pair<const std::string, std::shared_ptr<Texture>> &)> &lambda);

	/**
	 * @brief Iterate over all vector parameters in the Material.
	 *
	 * @param lambda The lambda function to call for each vector parameter.
	 */
	void forEachVectors(const std::function<void(std::pair<const std::string, glm::vec3> &)> &lambda);

	/**
	 * @brief Iterate over all scalar parameters in the Material.
	 *
	 * @param lambda The lambda function to call for each scalar parameter.
	 */
	void forEachScalars(const std::function<void(std::pair<const std::string, float> &)> &lambda);

	/**
	 * @brief Set the vertex shader used by the Material.
	 *
	 * @param vertexShader The vertex shader to set.
	 */
	void setVertexShader(std::shared_ptr<Shader> vertexShader);

	/**
	 * @brief Get the vertex shader used by the Material.
	 *
	 * @return The vertex shader as a shared pointer to Shader.
	 */
	[[nodiscard]] std::shared_ptr<Shader> getVertexShader() const;

	/**
	 * @brief Set the fragment shader used by the Material.
	 *
	 * @param fragmentShader The fragment shader to set.
	 */
	void setFragmentShader(std::shared_ptr<Shader> fragmentShader);

	/**
	 * @brief Get the fragment shader used by the Material.
	 *
	 * @return The fragment shader as a shared pointer to Shader.
	 */
	[[nodiscard]] std::shared_ptr<Shader> getFragmentShader() const;

protected:
	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures; /**< Map of texture parameters. */
	std::unordered_map<std::string, glm::vec3> _vectors;				 /**< Map of vector parameters. */
	std::unordered_map<std::string, float> _scalars;					 /**< Map of scalar parameters. */

	std::shared_ptr<Shader>
		_vertexShader; /**< The vertex shader used by the material. nullptr means using the standard shader. */
	std::shared_ptr<Shader>
		_fragmentShader; /**< The fragment shader used by the material. nullptr means using the standard shader. */
};

} // namespace Stone::Scene
