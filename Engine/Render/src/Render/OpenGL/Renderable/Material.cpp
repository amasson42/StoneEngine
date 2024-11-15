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

#ifndef NDEBUG
	int textureCount = 0;
	material.forEachTextures([&textureCount](auto, auto) { ++textureCount; });
	if (textureCount >= 32) {
		// TODO: Use log module
		std::cerr << "Material " << material.getId() << " has more than 32 textures" << std::endl;
	}
#endif
}

void Material::render(Scene::RenderContext &context) {
	(void)context;
}

void Material::setUniforms(Scene::MeshType meshType) {
	GlShaderProgram *program = _shaderCollection->getProgram(meshType);

	_material.forEachVectors([program](const auto &loc, glm::vec3 vec) { program->setUniform(loc, vec); });
	_material.forEachScalars([program](const auto &loc, float scalar) { program->setUniform(loc, scalar); });

	int textureIndex = 0;
	_material.forEachTextures(
		[program, &textureIndex](const auto &loc, const std::shared_ptr<Scene::Texture> &texture) {
			if (textureIndex >= 32)
				return;

			assert(texture->isDirty() == false);
			auto rendererTexture = texture->getRendererObject<Texture>();
			program->setUniformTexture(loc, *rendererTexture, textureIndex);
			++textureIndex;
		});
}

const std::shared_ptr<ShaderCollection> &Material::getShaderCollection() const {
	return _shaderCollection;
}


} // namespace Stone::Render::OpenGL
