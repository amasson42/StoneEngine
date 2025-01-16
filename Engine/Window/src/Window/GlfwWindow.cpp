// Copyright 2024 Stone-Engine

#include "Window/GlfwWindow.hpp"

#ifdef STONE_RENDERER_VULKAN
#include "Render/Vulkan/VulkanRenderer.hpp"
#elif defined(STONE_RENDERER_OPENGL)
#include "Render/OpenGL/OpenGLRenderer.hpp"
#else
#include "Render/OpenGL/OpenGLRenderer.hpp"
#endif

#include "Scene/Node/WorldNode.hpp"

#include <iostream>
#include <stdexcept>

namespace Stone::Window {

GlfwWindow::GlfwWindow(const std::shared_ptr<App> &app, const WindowSettings &settings)
	: Window(app, settings), _glfwWindow(nullptr) {
	glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);
#ifdef STONE_RENDERER_VULKAN
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#else // def STONE_RENDERER_OPENGL
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
#endif

	GLFWwindow *sharingContext = nullptr;
	if (!settings.shareContext.expired()) {
		auto sharingGlfwWindow = std::dynamic_pointer_cast<GlfwWindow>(settings.shareContext.lock());
		if (sharingGlfwWindow) {
			sharingContext = sharingGlfwWindow->_glfwWindow;
			_renderer = sharingGlfwWindow->_renderer;
		}
	}

	_glfwWindow = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(),
								   settings.fullScreen ? glfwGetPrimaryMonitor() : nullptr, sharingContext);
	if (!_glfwWindow) {
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSetWindowSizeLimits(_glfwWindow, 300, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwMakeContextCurrent(_glfwWindow);

	_initializeWindowCallbacks();

	glfwSwapInterval(1);

	_elapsedTime = glfwGetTime();

	if (!_renderer) {

		int frameBufferWidth, frameBufferHeight;
		glfwGetFramebufferSize(_glfwWindow, &frameBufferWidth, &frameBufferHeight);
#ifdef STONE_RENDERER_VULKAN
		Render::Vulkan::RendererSettings rendererSettings;
		rendererSettings.app_name = settings.title;
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		rendererSettings.instanceExt = std::vector<const char *>(glfwExtensions, glfwExtensions + glfwExtensionCount);
		rendererSettings.createSurface = [&](VkInstance instance, const VkAllocationCallbacks *allocator,
											 VkSurfaceKHR *surface) {
			return glfwCreateWindowSurface(instance, _glfwWindow, allocator, surface);
		};
		rendererSettings.frame_size = {
			static_cast<uint32_t>(frameBufferWidth),
			static_cast<uint32_t>(frameBufferHeight),
		};
		_renderer = std::make_shared<Render::Vulkan::VulkanRenderer>(rendererSettings);
#else
		Render::OpenGL::RendererSettings rendererSettings;
		rendererSettings.frame_size = {
			static_cast<uint32_t>(frameBufferWidth),
			static_cast<uint32_t>(frameBufferHeight),
		};
		auto renderer = std::make_shared<Render::OpenGL::OpenGLRenderer>(rendererSettings);
		renderer->initialize();
		_renderer = renderer;
#endif
	}

	_world->setRenderer(_renderer);
}

GlfwWindow::~GlfwWindow() {
	if (_glfwWindow != nullptr) {
		glfwDestroyWindow(_glfwWindow);
	}
}

void GlfwWindow::loopOnce() {
	if (_glfwWindow == nullptr)
		return;

	glfwPollEvents();

	if (glfwWindowShouldClose(_glfwWindow))
		return;

	double newTime = glfwGetTime();
	_deltaTime = newTime - _elapsedTime;
	_elapsedTime = newTime;

	Window::loopOnce();

	glfwSwapBuffers(_glfwWindow);
}

bool GlfwWindow::shouldClose() const {
	return _glfwWindow == nullptr || glfwWindowShouldClose(_glfwWindow);
}

void GlfwWindow::_initializeWindowCallbacks() {
	glfwSetWindowUserPointer(_glfwWindow, this);

	glfwSetCursorPosCallback(_glfwWindow, [](GLFWwindow *win, double x, double y) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onMouseMoveCallback(x, y);
	});

	glfwSetMouseButtonCallback(_glfwWindow, [](GLFWwindow *win, int button, int action, int modifiers) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onMouseButtonCallback(button, action, modifiers);
	});

	glfwSetKeyCallback(_glfwWindow, [](GLFWwindow *win, int key, int scancode, int action, int mods) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onKeyCallback(key, scancode, action, mods);
	});

	glfwSetCharCallback(_glfwWindow, [](GLFWwindow *win, unsigned int codepoint) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onCharCallback(codepoint);
	});

	glfwSetWindowCloseCallback(_glfwWindow, [](GLFWwindow *win) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onCloseCallback();
	});

	glfwSetWindowSizeCallback(_glfwWindow, [](GLFWwindow *win, int width, int height) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		if (window->_glfwWindow == nullptr)
			return;
		glfwGetWindowSize(window->_glfwWindow, &(window->_settings.width), &(window->_settings.height));
		window->_onResizeCallback(width, height);
	});

	glfwSetScrollCallback(_glfwWindow, [](GLFWwindow *win, double x, double y) {
		auto *window = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(win));
		window->_onScrollCallback(x, y);
	});
}

} // namespace Stone::Window
