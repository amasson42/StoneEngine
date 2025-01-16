// Copyright 2024 Stone-Engine

#include "ShaderCollection.hpp"

#include "GlShaders.hpp"
#include "OpenGLResources.hpp"
#include "Render/OpenGL/OpenGLRenderer.hpp"

namespace Stone::Render::OpenGL {

void ShaderCollection::makeMeshProgram() {
	if (_meshProgram != nullptr)
		return;

	assert(_resources.expired() == false);

	auto &vertexShader = _resources.lock()->getMeshVertexShader();
	assert(vertexShader != nullptr);

	_meshProgram = std::make_unique<GlShaderProgram>(*vertexShader, *_glFragmentShader);
}

void ShaderCollection::makeSkinMeshProgram() {
	if (_skinMeshProgram != nullptr)
		return;

	assert(_resources.expired() == false);

	auto &vertexShader = _resources.lock()->getSkinMeshVertexShader();
	assert(vertexShader != nullptr);

	_skinMeshProgram = std::make_unique<GlShaderProgram>(*vertexShader, *_glFragmentShader);
}

void ShaderCollection::makeInstancedMeshProgram() {
	if (_instancedMeshProgram != nullptr)
		return;

	assert(_resources.expired() == false);

	auto &vertexShader = _resources.lock()->getInstancedMeshVertexShader();
	assert(vertexShader != nullptr);

	_instancedMeshProgram = std::make_unique<GlShaderProgram>(*vertexShader, *_glFragmentShader);
}

void ShaderCollection::makeProgram(Scene::MeshType meshType) {
	switch (meshType) {
	case Scene::MeshType::Standard: makeMeshProgram(); break;
	case Scene::MeshType::Skin: makeSkinMeshProgram(); break;
	case Scene::MeshType::Instanced: makeInstancedMeshProgram(); break;
	}
}

const std::unique_ptr<GlShaderProgram> &ShaderCollection::getMeshProgram() const {
	return _meshProgram;
}

const std::unique_ptr<GlShaderProgram> &ShaderCollection::getSkinMeshProgram() const {
	return _skinMeshProgram;
}

const std::unique_ptr<GlShaderProgram> &ShaderCollection::getInstancedMeshProgram() const {
	return _instancedMeshProgram;
}

GlShaderProgram *ShaderCollection::getProgram(Scene::MeshType meshType) const {
	switch (meshType) {
	case Scene::MeshType::Standard: return _meshProgram.get();
	case Scene::MeshType::Skin: return _skinMeshProgram.get();
	case Scene::MeshType::Instanced: return _instancedMeshProgram.get();
	}
	return nullptr;
}

} // namespace Stone::Render::OpenGL
