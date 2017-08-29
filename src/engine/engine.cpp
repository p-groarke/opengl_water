#include "engine/engine.hpp"
#include "components/renderer.hpp"

#include <chrono>

#if defined(__APPLE__)
	#include <mach-o/dyld.h>
#elif defined(_WIN32)
	#include <windows.h>
#endif

namespace {
std::chrono::high_resolution_clock::time_point new_frame_time
		= std::chrono::high_resolution_clock::now();

inline float get_dt() {
	auto last_frame_t = new_frame_time;
	new_frame_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> dt_duration
			= new_frame_time - last_frame_t;
	return dt_duration.count();
}

inline std::string init_folder_path() {
	std::string ret;
	char platform_slash = '/';
#if defined(__APPLE__)
	uint32_t size = 512;
	char app_path[size];
	int err = _NSGetExecutablePath(app_path, &size);
	assert(err == 0 && "Problem getting application path.");
#elif defined(_WIN32)
	TCHAR app_path[MAX_PATH];
	int err = GetModuleFileName(NULL, app_path, MAX_PATH);
	platform_slash = '\\';
	//assert(err == 0 && "Problem getting application path."); // FIXME
#endif
	ret = std::string(app_path);
	auto slash_index = ret.find_last_of(platform_slash);
	if (slash_index != std::string::npos) {
		ret = ret.substr(0, slash_index + 1);
	}
	return ret;
}
} // namespace anonymous

Engine::Engine(const char* project_name)
	: window(project_name)
{}

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

std::string Engine::folder_path = init_folder_path();

//std::vector<std::function<void()>> Engine::_components_init = {};
std::vector<std::function<void(float)>> Engine::_components_update = {};
std::vector<std::function<void(float)>> Engine::_components_render = {};
std::vector<std::function<void()>> Engine::_components_destroy = {};
