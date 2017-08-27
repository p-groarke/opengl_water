#include "engine/engine.h"
#include "engine/globals.h"
#include "components/renderer.h"

#include <chrono>

Engine::Engine(const char* project_name)
	: window(project_name)
{
	app::init_executable_path();
}

void Engine::doit() {
	auto new_frame_t = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window.window)) {
		const auto last_frame_t = new_frame_t;
		new_frame_t = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> dt_duration
				= new_frame_t - last_frame_t;
		const float dt = dt_duration.count();

		for (size_t i = 0; i < _components_update.size(); ++i) {
			_components_update[i](dt);
		}

		for (size_t i = 0; i < _components_render.size(); ++i) {
			_components_render[i](dt);
		}

		window.post_render();
		GL_CHECK_ERROR();
	}
	for (size_t i = 0; i < _components_destroy.size(); ++i) {
			_components_destroy[i]();
	}
}

//void Engine::component_init(std::function<void()>&& f) {
//	Engine::_components_init.emplace_back(f);
//}

void Engine::component_update(std::function<void(float)>&& f) {
	Engine::_components_update.emplace_back(f);
}

void Engine::component_render(std::function<void(float)>&& f) {
	Engine::_components_render.emplace_back(f);
}

void Engine::component_destroy(std::function<void()>&& f) {
	Engine::_components_destroy.emplace_back(f);
}

//std::vector<std::function<void()>> Engine::_components_init = {};
std::vector<std::function<void(float)>> Engine::_components_update = {};
std::vector<std::function<void(float)>> Engine::_components_render = {};
std::vector<std::function<void()>> Engine::_components_destroy = {};
