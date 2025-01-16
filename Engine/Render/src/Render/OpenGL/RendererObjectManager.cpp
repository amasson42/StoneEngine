// Copyright 2024 Stone-Engine

#include "RendererObjectManager.hpp"

#include "Renderable/FragmentShader.hpp"
#include "Renderable/InstancedMeshNode.hpp"
#include "Renderable/Material.hpp"
#include "Renderable/Mesh.hpp"
#include "Renderable/MeshNode.hpp"
#include "Renderable/SkinMesh.hpp"
#include "Renderable/SkinMeshNode.hpp"
#include "Renderable/Texture.hpp"


#define UPDATE_RENDERER_OBJECT(RenderableClass, object)                                                                \
	void RendererObjectManager::update##RenderableClass(const std::shared_ptr<Scene::RenderableClass> &(object)) {     \
		Scene::RendererObjectManager::update##RenderableClass((object));                                               \
                                                                                                                       \
		auto new##RenderableClass = std::make_shared<OpenGL::RenderableClass>(*(object), _renderer);                   \
		updateRendererObject(*(object), new##RenderableClass);                                                         \
	}


namespace Stone::Render::OpenGL {

RendererObjectManager::RendererObjectManager(std::shared_ptr<OpenGLRenderer> renderer)
	: _renderer(std::move(renderer)) {
}


UPDATE_RENDERER_OBJECT(MeshNode, meshNode)

UPDATE_RENDERER_OBJECT(InstancedMeshNode, instancedMeshNode)

UPDATE_RENDERER_OBJECT(SkinMeshNode, skinMeshNode)

UPDATE_RENDERER_OBJECT(Material, material)

UPDATE_RENDERER_OBJECT(DynamicMesh, dynamicMesh)

UPDATE_RENDERER_OBJECT(StaticMesh, staticMesh)

UPDATE_RENDERER_OBJECT(DynamicSkinMesh, dynamicSkinMesh)

UPDATE_RENDERER_OBJECT(StaticSkinMesh, staticSkinMesh)

UPDATE_RENDERER_OBJECT(Texture, texture)

UPDATE_RENDERER_OBJECT(FragmentShader, shader)

} // namespace Stone::Render::OpenGL
