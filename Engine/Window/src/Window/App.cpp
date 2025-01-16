// Copyright 2024 Stone-Engine

#include "Window/App.hpp"

#include "Window/GlfwWindow.hpp"
#include "Window/Window.hpp"

#include <algorithm>
#include <iostream>

namespace Stone::Window {

App::App() : std::enable_shared_from_this<App>(), _windows() {
	if (glfwInit() == GLFW_FALSE) {
		throw std::runtime_error("Failed to initialize GLFW");
	}
}

App::~App() {
	glfwTerminate();
}

std::shared_ptr<Window> App::createWindow(const WindowSettings &settings) {
	std::shared_ptr<Window> win = std::make_shared<GlfwWindow>(shared_from_this(), settings);
	_windows.push_back(win);
	return win;
}

void App::destroyWindow(const std::shared_ptr<Window> &window) {
	auto it = std::find(_windows.begin(), _windows.end(), window);
	if (it != _windows.end()) {
		*it = nullptr;
	}
}

int App::run() {
	try {
		while (_windows.empty() == false) {
			for (int i = static_cast<int>(_windows.size()) - 1; i >= 0; --i) {
				if (_windows[i] == nullptr || _windows[i]->shouldClose()) {
					_windows.erase(_windows.begin() + i);
					continue;
				}

				_windows[i]->loopOnce();
			}
		}
	} catch (const std::exception &e) {
		std::cerr << "Stone Application ends with Exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

} // namespace Stone::Window
