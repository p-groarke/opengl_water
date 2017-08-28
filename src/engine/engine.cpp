#include "engine/engine.h"
#include "engine/globals.h"
#include "components/renderer.h"


Engine::Engine(const char* project_name)
	: window(project_name)
	, new_frame_time(std::chrono::high_resolution_clock::now())
{
	app::init_executable_path();
}

void Engine::doit() {
	while (!glfwWindowShouldClose(window.window)) {
		float dt = get_dt();

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

float Engine::get_dt() {
	const auto last_frame_t = new_frame_time;
	new_frame_time = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> dt_duration
			= new_frame_time - last_frame_t;
	return dt_duration.count();
}

//std::vector<std::function<void()>> Engine::_components_init = {};
std::vector<std::function<void(float)>> Engine::_components_update = {};
std::vector<std::function<void(float)>> Engine::_components_render = {};
std::vector<std::function<void()>> Engine::_components_destroy = {};
