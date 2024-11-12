// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/Renderable/IRenderable.hpp"

#include <functional>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <variant>

namespace Stone::Scene {

class Texture;
class FragmentShader;

/**
 * @brief The Material class represents a material used for rendering objects in the scene.
 *
 * It contains various parameters such as textures, vectors, and scalars that can be used
 * to configure the appearance of rendered objects.
 */
class Material : public Core::Object, public IRenderable {
	STONE_OBJECT(Material)

public:
	using Location = std::variant<std::string, int>;

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
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Set a texture parameter for the Material.
	 *
	 * @param location The location of the texture parameter.
	 * @param texture The texture to set.
	 */
	void setTextureParameter(const Location &location, std::shared_ptr<Texture> texture);

	/**
	 * @brief Get a texture parameter from the Material.
	 *
	 * @param location The location of the texture parameter.
	 * @return The texture parameter as a shared pointer to Texture.
	 */
	[[nodiscard]] std::shared_ptr<Texture> getTextureParameter(const Location &location) const;

	/**
	 * @brief Set a vector parameter for the Material.
	 *
	 * @param location The location of the vector parameter.
	 * @param vector The vector to set.
	 */
	void setVectorParameter(const Location &location, const glm::vec3 &vector);

	/**
	 * @brief Get a vector parameter from the Material.
	 *
	 * @param location The location of the vector parameter.
	 * @return The vector parameter as a glm::vec3.
	 */
	[[nodiscard]] glm::vec3 getVectorParameter(const Location &location) const;

	/**
	 * @brief Set a scalar parameter for the Material.
	 *
	 * @param location The location of the scalar parameter.
	 * @param scalar The scalar value to set.
	 */
	void setScalarParameter(const Location &location, float scalar);

	/**
	 * @brief Get a scalar parameter from the Material.
	 *
	 * @param location The location of the scalar parameter.
	 * @return The scalar parameter as a float.
	 */
	[[nodiscard]] float getScalarParameter(const Location &location) const;

	/**
	 * @brief Iterate over all texture parameters in the Material.
	 *
	 * @param lambda The lambda function to call for each texture parameter.
	 */
	void forEachTextures(const std::function<void(const Location &, const std::shared_ptr<Texture> &)> &lambda) const;

	/**
	 * @brief Iterate over all vector parameters in the Material.
	 *
	 * @param lambda The lambda function to call for each vector parameter.
	 */
	void forEachVectors(const std::function<void(const Location &, glm::vec3)> &lambda) const;

	/**
	 * @brief Iterate over all scalar parameters in the Material.
	 *
	 * @param lambda The lambda function to call for each scalar parameter.
	 */
	void forEachScalars(const std::function<void(const Location &, float)> &lambda) const;

	/**
	 * @brief Set the fragment shader used by the Material.
	 *
	 * @param fragmentShader The fragment shader to set.
	 */
	void setFragmentShader(std::shared_ptr<FragmentShader> fragmentShader);

	/**
	 * @brief Get the fragment shader used by the Material.
	 *
	 * @return The fragment shader as a shared pointer to Shader.
	 */
	[[nodiscard]] const std::shared_ptr<FragmentShader> &getFragmentShader() const;

protected:
	std::unordered_map<Location, std::shared_ptr<Texture>> _textures; /**< Map of texture parameters. */
	std::unordered_map<Location, glm::vec3> _vectors;				  /**< Map of vector parameters. */
	std::unordered_map<Location, float> _scalars;					  /**< Map of scalar parameters. */

	std::shared_ptr<FragmentShader>
		_fragmentShader; /**< The fragment shader used by the material. nullptr means using the standard shader. */
};

std::ostream &operator<<(std::ostream &stream, const Material::Location &location);

} // namespace Stone::Scene
