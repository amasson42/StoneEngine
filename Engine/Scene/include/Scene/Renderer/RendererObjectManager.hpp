// Copyright 2024 Stone-Engine

#pragma once

#include "Scene/Renderable/IRenderable.hpp"
#include "SceneTypes.hpp"

namespace Stone::Core {
class Object;
}

namespace Stone::Scene {

/**
 * @class RendererObjectManager
 * @brief Provide basic overridable methods to manage the renderables objects elements in the scene.
 *
 * This class provides basic methods to update the renderer data of classes implementing the IRenderable interface.
 * It will store an instance of IRendererObject in the IRenderable::_rendererObject proprety.
 * Each IRenderable objects will use the appropriate method in this interface to update itself.
 */
class RendererObjectManager {
public:
	RendererObjectManager() = default;
	virtual ~RendererObjectManager() = default;

	/**
	 * Updates the renderable object with the given node.
	 *
	 * @param node The shared pointer to the renderable object.
	 */
	virtual void updateRenderable(const std::shared_ptr<Core::Object> &renderable);

	/**
	 * @brief Updates the renderer data for a given mesh node.
	 * @param meshNode The mesh node to be updated.
	 */
	virtual void updateMeshNode(const std::shared_ptr<MeshNode> &meshNode);

	/**
	 * @brief Updates the renderer data for a given instanced mesh node.
	 * @param instancedMeshNode The instanced mesh node to be updated.
	 */
	virtual void updateInstancedMeshNode(const std::shared_ptr<InstancedMeshNode> &instancedMeshNode);

	/**
	 * @brief Updates the renderer data for a given skin mesh node.
	 * @param skinMeshNode The skin mesh node to be updated.
	 */
	virtual void updateSkinMeshNode(const std::shared_ptr<SkinMeshNode> &skinMeshNode);

	/**
	 * @brief Updates the renderer data for a given material.
	 * @param material The material to be updated.
	 */
	virtual void updateMaterial(const std::shared_ptr<Material> &material);

	/**
	 * @brief Updates the renderer data for a given dynamic mesh.
	 * @param mesh The dynamic mesh to be updated.
	 */
	virtual void updateDynamicMesh(const std::shared_ptr<DynamicMesh> &mesh);

	/**
	 * @brief Updates the renderer data for a given static mesh.
	 * @param mesh The mesh to be updated.
	 */
	virtual void updateStaticMesh(const std::shared_ptr<StaticMesh> &mesh);

	/**
	 * @brief Updates the renderer data for a given dynamic skin mesh.
	 * @param skinmesh The skin mesh to be updated.
	 */
	virtual void updateDynamicSkinMesh(const std::shared_ptr<DynamicSkinMesh> &skinmesh);

	/**
	 * @brief Updates the renderer data for a given static skin mesh.
	 * @param skinmesh The skin mesh to be updated.
	 */
	virtual void updateStaticSkinMesh(const std::shared_ptr<StaticSkinMesh> &skinmesh);

	/**
	 * @brief Updates the renderer data for a given wireframe shape.
	 * @param wireframe shape The wireframe shape to be updated.
	 */
	virtual void updateWireframeShape(const std::shared_ptr<WireframeShape> &shape);

	/**
	 * @brief Updates the renderer data for a given texture.
	 * @param texture The texture to be updated.
	 */
	virtual void updateTexture(const std::shared_ptr<Texture> &texture);

	/**
	 * @brief Updates the renderer data for a given shader.
	 * @param shader The shader to be updated.
	 */
	virtual void updateFragmentShader(const std::shared_ptr<FragmentShader> &shader);

protected:
	/**
	 * @brief Sets the IRendererObject value to the given IRenderable.
	 *
	 * This class is friend with IRenderable, so it can access the protected method setRendererObject but its
	 * inheriting classes can't.
	 *
	 * @param element The render element to set the renderer object to.
	 * @param rendererObject The renderer object to set.
	 */
	static void updateRendererObject(IRenderable &element, const std::shared_ptr<IRendererObject> &rendererObject);
};

} // namespace Stone::Scene
