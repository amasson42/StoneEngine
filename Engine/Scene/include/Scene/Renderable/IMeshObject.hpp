// Copyright 2024 Stone-Engine

#pragma once

#include "Core/Object.hpp"
#include "Scene/Renderable/IRenderable.hpp"

namespace Stone::Scene {

class Material;

enum class MeshType : uint8_t {
	Standard = 0,
	Skin = 1,
	Instanced = 2,
};

/**
 * @brief Interface for a mesh object of any type.
 *
 * The `IMeshObject` is an interface that provides methods that the mesh object metadata must implement.
 *
 * @see StaticMesh, DynamicMesh, SkinMesh, SkinMeshSource
 */
class IMeshObject : public Core::Object, public IRenderable {
	STONE_ABSTRACT_OBJECT(IMeshObject)

public:
	/**
	 * @brief Destructor
	 *
	 * Virtual destructor for proper cleanup of derived classes.
	 */
	virtual ~IMeshObject() = default;

	/**
	 * @brief Get the material associated with the mesh object
	 *
	 * This function returns the material associated with the mesh object.
	 *
	 * @return The material associated with the mesh object
	 */
	const std::shared_ptr<Material> &getDefaultMaterial() const {
		return _defaultMaterial;
	}

	/**
	 * @brief Set the material associated with the mesh object
	 *
	 * This function sets the material associated with the mesh object.
	 *
	 * @param material The material to associate with the mesh object
	 */
	void setDefaultMaterial(const std::shared_ptr<Material> &material) {
		_defaultMaterial = material;
	}

protected:
	std::shared_ptr<Material> _defaultMaterial; /**< The material associated with the mesh object */
};

} // namespace Stone::Scene
