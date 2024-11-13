// Copyright 2024 Stone-Engine


#include "Render/OpenGL/OpenGLRenderer.hpp"

#include "OpenGLResources.hpp"
#include "RenderContext.hpp"
#include "RendererObjectManager.hpp"
#include "Scene/Node/WorldNode.hpp"

#include <GL/glew.h>


namespace Stone::Render::OpenGL {

static void initializeOpenGL() {
	static bool initialized = false;
	if (!initialized) {
		glewExperimental = true;
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("Failed to initialize GLEW");
		}
		initialized = true;
	}
}

OpenGLRenderer::OpenGLRenderer(RendererSettings &settings)
	: Renderer(), _frameSize(settings.frame_size), _resources(nullptr) {
}

OpenGLRenderer::~OpenGLRenderer() {
	std::cout << "OpenGLRenderer destroyed" << std::endl;
}

void OpenGLRenderer::updateDataForWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	OpenGL::RendererObjectManager manager(std::static_pointer_cast<OpenGLRenderer>(shared_from_this()));
	world->traverseTopDown([&manager](const std::shared_ptr<Scene::Node> &node) {
		auto renderElement = std::dynamic_pointer_cast<Scene::IRenderable>(node);
		if (renderElement && renderElement->isDirty()) {
			manager.updateRenderable(node);
		}
	});
}

void OpenGLRenderer::renderWorld(const std::shared_ptr<Scene::WorldNode> &world) {
	glViewport(0, 0, static_cast<GLsizei>(_frameSize.first), static_cast<GLsizei>(_frameSize.second));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	OpenGL::RenderContext context;
	world->initializeRenderContext(context);
	world->render(context);
}

void OpenGLRenderer::updateFrameSize(std::pair<uint32_t, uint32_t> size) {
	_frameSize = size;
}

void OpenGLRenderer::initialize() {
	initializeOpenGL();
	std::cout << "OpenGLRenderer created" << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	_resources = std::make_shared<OpenGLResources>(std::static_pointer_cast<OpenGLRenderer>(shared_from_this()));
}

const OpenGLResources &OpenGLRenderer::getOpenGLResources() const {
	return *_resources;
}


} // namespace Stone::Render::OpenGL
