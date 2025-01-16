// Copyright 2024 Stone-Engine

#pragma once

#include "Render/OpenGL/RendererSettings.hpp"
#include "Render/Renderer.hpp"

namespace Stone::Scene {
class WorldNode;
}

namespace Stone::Render::OpenGL {

class OpenGLResources;
struct GBuffer;

class OpenGLRenderer : public Renderer {
public:
	OpenGLRenderer() = delete;
	explicit OpenGLRenderer(RendererSettings &settings);
	OpenGLRenderer(const OpenGLRenderer &) = delete;

	~OpenGLRenderer() override;

	/** Renderer */

	void updateDataForWorld(const std::shared_ptr<Scene::WorldNode> &world) override;
	void renderWorld(const std::shared_ptr<Scene::WorldNode> &world) override;

	void updateFrameSize(std::pair<uint32_t, uint32_t> size) override;

	void initialize();
	const std::shared_ptr<OpenGLResources> &getOpenGLResources() const;

private:
	std::pair<uint32_t, uint32_t> _frameSize;
	std::shared_ptr<OpenGLResources> _resources;
	std::unique_ptr<GBuffer> _gBuffer;
};

} // namespace Stone::Render::OpenGL
