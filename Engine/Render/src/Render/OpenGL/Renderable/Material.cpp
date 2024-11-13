// Copyright 2024 Stone-Engine

#include "Material.hpp"

#include "FragmentShader.hpp"

namespace Stone::Render::OpenGL {

Material::Material(Scene::Material &material, const std::shared_ptr<OpenGLRenderer> &renderer) : _material(material) {
	if (_material.getFragmentShader() == nullptr) {
		_shaderCollection = std::make_shared<ShaderCollection>(material, renderer->getOpenGLResources());
	} else {
		_shaderCollection = _material.getFragmentShader()->getRendererObject<FragmentShader>()->getShaderCollection();
	}
}

void Material::render(Scene::RenderContext &context) {
	(void)context;
}

void Material::setUniforms(Scene::MeshType meshType) {
	GlShaderProgram *program = _shaderCollection->getProgram(meshType);

	_material.forEachVectors([program](const auto &loc, glm::vec3 vec) { program->setUniform(loc, vec); });
	_material.forEachScalars([program](const auto &loc, float scalar) { program->setUniform(loc, scalar); });
}

const std::shared_ptr<ShaderCollection> &Material::getShaderCollection() const {
	return _shaderCollection;
}


} // namespace Stone::Render::OpenGL
