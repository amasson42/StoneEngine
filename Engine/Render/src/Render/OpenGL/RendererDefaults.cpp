// Copyright 2024 StoneEngine

#include "RendererDefaults.hpp"

#include "Scene/Renderable/Material.hpp"

namespace Stone::Render::OpenGL {

RendererDefaults::RendererDefaults(const std::shared_ptr<OpenGLRenderer> &renderer) : Scene::RendererDefaults() {
	_meshFragmentShader = FragmentShader::makeStandardMeshShader(renderer);
	_meshVertexShader = VertexShader::makeStandardMeshShader(renderer);
	_skinMeshVertexShader = VertexShader::makeStandardSkinMeshShader(renderer);
	// _instancedMeshVertexShader = VertexShader::makeStandardInstancedMeshShader(renderer);
}

} // namespace Stone::Render::OpenGL
