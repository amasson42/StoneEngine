// Copyright 2024 Stone-Engine

#include "Scene/Renderer/RendererObjectManager.hpp"

#include "Scene.hpp"

namespace Stone::Scene {

using CastFunction = std::function<void(RendererObjectManager &, const std::shared_ptr<Core::Object> &)>;

#define CASTED_FUNCTION_MAP_ENTRY(ClassName)                                                                           \
	{ClassName::StaticHashCode(),                                                                                      \
	 [](RendererObjectManager &manager, const std::shared_ptr<Core::Object> &renderable) {                             \
		 manager.update##ClassName(std::static_pointer_cast<ClassName>(renderable));                                   \
	 }}

const std::unordered_map<std::intptr_t, CastFunction> updateCastedFunctions = {
	CASTED_FUNCTION_MAP_ENTRY(MeshNode),		CASTED_FUNCTION_MAP_ENTRY(InstancedMeshNode),
	CASTED_FUNCTION_MAP_ENTRY(SkinMeshNode),	CASTED_FUNCTION_MAP_ENTRY(Material),
	CASTED_FUNCTION_MAP_ENTRY(DynamicMesh),		CASTED_FUNCTION_MAP_ENTRY(StaticMesh),
	CASTED_FUNCTION_MAP_ENTRY(DynamicSkinMesh), CASTED_FUNCTION_MAP_ENTRY(StaticSkinMesh),
	CASTED_FUNCTION_MAP_ENTRY(WireframeShape),	CASTED_FUNCTION_MAP_ENTRY(Texture),
	CASTED_FUNCTION_MAP_ENTRY(FragmentShader),
};

void RendererObjectManager::updateRenderable(const std::shared_ptr<Core::Object> &renderable) {
	auto it = updateCastedFunctions.find(renderable->getClassHashCode());
	if (it != updateCastedFunctions.end()) {
		it->second(*this, renderable);
	}
}

void RendererObjectManager::updateMeshNode(const std::shared_ptr<MeshNode> &meshNode) {
	if (meshNode->getMaterial() && meshNode->getMaterial()->isDirty())
		updateMaterial(meshNode->getMaterial());
	if (meshNode->getMesh() && meshNode->getMesh()->isDirty())
		updateRenderable(meshNode->getMesh());
	meshNode->markUndirty();
}

void RendererObjectManager::updateInstancedMeshNode(const std::shared_ptr<InstancedMeshNode> &instancedMeshNode) {
	if (instancedMeshNode->getMaterial() && instancedMeshNode->getMaterial()->isDirty())
		updateMaterial(instancedMeshNode->getMaterial());
	if (instancedMeshNode->getMesh() && instancedMeshNode->getMesh()->isDirty())
		updateRenderable(instancedMeshNode->getMesh());
	instancedMeshNode->markUndirty();
}

void RendererObjectManager::updateSkinMeshNode(const std::shared_ptr<SkinMeshNode> &skinMeshNode) {
	if (skinMeshNode->getMaterial() && skinMeshNode->getMaterial()->isDirty())
		updateMaterial(skinMeshNode->getMaterial());
	if (skinMeshNode->getSkinMesh() && skinMeshNode->getSkinMesh()->isDirty())
		updateRenderable(skinMeshNode->getSkinMesh());
	skinMeshNode->markUndirty();
}

void RendererObjectManager::updateMaterial(const std::shared_ptr<Material> &material) {
	auto fragmentShader = material->getFragmentShader();
	if (fragmentShader && fragmentShader->isDirty()) {
		updateFragmentShader(fragmentShader);
	}
	material->forEachTextures([this](const std::pair<const Material::Location &, std::shared_ptr<Texture>> &it) {
		if (it.second->isDirty())
			updateTexture(it.second);
	});
	material->markUndirty();
}

void RendererObjectManager::updateDynamicMesh(const std::shared_ptr<DynamicMesh> &mesh) {
	if (mesh->getDefaultMaterial() && mesh->getDefaultMaterial()->isDirty())
		updateMaterial(mesh->getDefaultMaterial());
	mesh->markUndirty();
}

void RendererObjectManager::updateStaticMesh(const std::shared_ptr<StaticMesh> &mesh) {
	if (mesh->getDefaultMaterial() && mesh->getDefaultMaterial()->isDirty())
		updateMaterial(mesh->getDefaultMaterial());
	mesh->markUndirty();
}

void RendererObjectManager::updateDynamicSkinMesh(const std::shared_ptr<DynamicSkinMesh> &skinmesh) {
	if (skinmesh->getDefaultMaterial() && skinmesh->getDefaultMaterial()->isDirty())
		updateMaterial(skinmesh->getDefaultMaterial());
	skinmesh->markUndirty();
}

void RendererObjectManager::updateStaticSkinMesh(const std::shared_ptr<StaticSkinMesh> &skinmesh) {
	if (skinmesh->getDefaultMaterial() && skinmesh->getDefaultMaterial()->isDirty())
		updateMaterial(skinmesh->getDefaultMaterial());
	skinmesh->markUndirty();
}

void RendererObjectManager::updateWireframeShape(const std::shared_ptr<WireframeShape> &shape) {
	shape->markUndirty();
}

void RendererObjectManager::updateTexture(const std::shared_ptr<Texture> &texture) {
	texture->markUndirty();
}

void RendererObjectManager::updateFragmentShader(const std::shared_ptr<FragmentShader> &shader) {
	shader->markUndirty();
}

void RendererObjectManager::setRendererObjectTo(IRenderable *element,
												const std::shared_ptr<IRendererObject> &rendererObject) {
	element->setRendererObject(rendererObject);
}

void RendererObjectManager::markElementUndirty(IRenderable *element) {
	element->markUndirty();
}

} // namespace Stone::Scene
