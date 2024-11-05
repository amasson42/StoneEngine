// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderable/IMeshObject.hpp"
#include "Scene/Vertex.hpp"

#include <vector>

/**
 * @brief Represents a mesh used for rendering in the scene.
 *
 * It provides functionality for managing vertices and indices of the mesh.
 */
namespace Stone::Scene {

class IMeshInterface : public IMeshObject {};

/**
 * @brief Represents a dynamic mesh used for rendering in the scene.
 *
 * A dynamic mesh is a mesh that can be modified at runtime.
 * It provides functionality for managing vertices and indices of the mesh.
 */
class DynamicMesh : public IMeshInterface {
	STONE_OBJECT(DynamicMesh);

public:
	DynamicMesh() = default;
	DynamicMesh(const DynamicMesh &other) = default;

	~DynamicMesh() override = default;

	/**
	 * @brief Writes the mesh data to the given output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write a closing bracer after the mesh data.
	 * @return The modified output stream.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Retrieves the vertices of the mesh.
	 *
	 * @return A constant reference to the vector of vertices.
	 */
	[[nodiscard]] const std::vector<Vertex> &getVertices() const;

	/**
	 * @brief Retrieves the indices of the mesh.
	 *
	 * @return A constant reference to the vector of indices.
	 */
	[[nodiscard]] const std::vector<uint32_t> &getIndices() const;

	/**
	 * @brief Execute a lambda that receives a mutable reference to the vertices and indices.
	 *
	 * @note Using this method marks the mesh as dirty after the lambda is fully executed.
	 */
	void withElementsRef(const std::function<void (std::vector<Vertex> &, std::vector<uint32_t> &)>& func);

protected:
	std::vector<Vertex> _vertices;	/**< The vector of vertices. */
	std::vector<uint32_t> _indices; /**< The vector of indices. */
};


/**
 * @brief Represents a static mesh used for rendering in the scene.
 *
 * A static mesh is a mesh that cannot be modified at runtime.
 * It is generated from a dynamic mesh.
 */
class StaticMesh : public IMeshInterface {
	STONE_OBJECT(StaticMesh);

public:
	StaticMesh() = default;
	StaticMesh(const StaticMesh &other) = default;

	~StaticMesh() override = default;

	/**
	 * @brief Writes the mesh data to the given output stream.
	 *
	 * @param stream The output stream to write to.
	 * @param closing_bracer Flag indicating whether to write a closing bracer after the mesh data.
	 * @return The modified output stream.
	 */
	std::ostream &writeToStream(std::ostream &stream, bool closing_bracer) const override;

	/**
	 * @brief Retrieves the source mesh being used to generate the static mesh.
	 * It's expected to be a nullptr once the buffers are initialized.
	 *
	 * @return The source mesh used to generate the static mesh.
	 */
	[[nodiscard]] const std::shared_ptr<DynamicMesh> &getSourceMesh() const;

	/**
	 * @brief Sets the source mesh used to generate the static mesh.
	 *
	 * @param sourceMesh The source mesh used to generate the static mesh.
	 */
	void setSourceMesh(const std::shared_ptr<DynamicMesh> &sourceMesh);


protected:
	/**
	 * The dynamic mesh used for rendering.
	 * This pointer will be reset by the renderer once the buffers are initialized.
	 */
	std::shared_ptr<DynamicMesh> _dynamicMesh;
};


} // namespace Stone::Scene
