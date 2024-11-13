// Copyright 2024 StoneEngine

#include "OpenGLResources.hpp"

#include "Scene/Renderable/Material.hpp"
#include "ShaderCollection.hpp"

namespace Stone::Render::OpenGL {

OpenGLResources::OpenGLResources(const std::shared_ptr<OpenGLRenderer> &renderer) : _renderer(renderer) {
}

const std::unique_ptr<GlVertexShader> &OpenGLResources::getMeshVertexShader() {
	if (_meshVertexShader == nullptr) {
		_meshVertexShader = GlVertexShader::makeStandardMeshShader();
	}
	return _meshVertexShader;
}

const std::unique_ptr<GlVertexShader> &OpenGLResources::getSkinMeshVertexShader() {
	if (_skinMeshVertexShader == nullptr) {
		_skinMeshVertexShader = GlVertexShader::makeStandardSkinMeshShader();
	}
	return _skinMeshVertexShader;
}

const std::unique_ptr<GlVertexShader> &OpenGLResources::getInstancedMeshVertexShader() {
	if (_instancedMeshVertexShader == nullptr) {
		_instancedMeshVertexShader = GlVertexShader::makeStandardInstancedMeshShader();
	}
	return _instancedMeshVertexShader;
}

GlVertexShader *OpenGLResources::getVertexShader(Scene::MeshType meshType) {
	switch (meshType) {
	case Scene::MeshType::Standard: return getMeshVertexShader().get();
	case Scene::MeshType::Skin: return getSkinMeshVertexShader().get();
	case Scene::MeshType::Instanced: return getInstancedMeshVertexShader().get();
	default: return nullptr;
	}
}


const std::unique_ptr<GlFragmentShader> &OpenGLResources::getFragmentShader(Scene::ShaderParameters params) {
	auto it = _fragmentShaders.find(params);
	if (it == _fragmentShaders.end()) {
		return (_fragmentShaders[params] = GlFragmentShader::makeStandardShader(params));
	} else {
		return it->second;
	}
}

const std::unique_ptr<ShaderCollection> &OpenGLResources::getDefaultShaderCollection() {
	if (_defaultShaderCollection == nullptr) {
		_defaultShaderCollection = std::make_unique<ShaderCollection>(shared_from_this());
	}
	return _defaultShaderCollection;
}

} // namespace Stone::Render::OpenGL
