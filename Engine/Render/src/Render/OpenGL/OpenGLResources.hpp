// Copyright 2024 Stone-Engine

#pragma once

#include "GlShaders.hpp"
#include "Scene/Shader/ShaderParameters.hpp"
#include "ShaderCollection.hpp"

namespace Stone::Render::OpenGL {

class OpenGLRenderer;

class OpenGLResources : std::enable_shared_from_this<OpenGLResources> {
public:
	OpenGLResources() = default;

	OpenGLResources(const std::shared_ptr<OpenGLRenderer> &renderer);

	virtual ~OpenGLResources() = default;

	const std::unique_ptr<GlVertexShader> &getMeshVertexShader();
	const std::unique_ptr<GlVertexShader> &getSkinMeshVertexShader();
	const std::unique_ptr<GlVertexShader> &getInstancedMeshVertexShader();
	GlVertexShader *getVertexShader(Scene::MeshType meshType);

	const std::unique_ptr<GlFragmentShader> &getFragmentShader(Scene::ShaderParameters params);

	const std::unique_ptr<ShaderCollection> &getDefaultShaderCollection();

private:
	std::weak_ptr<OpenGLRenderer> _renderer;

	std::unique_ptr<GlVertexShader> _meshVertexShader;
	std::unique_ptr<GlVertexShader> _skinMeshVertexShader;
	std::unique_ptr<GlVertexShader> _instancedMeshVertexShader;

	std::unordered_map<Scene::ShaderParameters, std::unique_ptr<GlFragmentShader>> _fragmentShaders;

	std::unique_ptr<ShaderCollection> _defaultShaderCollection;
};

} // namespace Stone::Render::OpenGL
