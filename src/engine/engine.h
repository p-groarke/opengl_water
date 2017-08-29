#pragma once
#include "engine/window.h"

#include <chrono>
#include <functional>
#include <vector>

struct Engine {
	Engine(const char* project_name);
	void doit();

//	static void component_init(std::function<void()>&& f);
	static void component_update(std::function<void(float)>&& f);
	static void component_render(std::function<void(float)>&& f);
	static void component_destroy(std::function<void()>&& f);

private:
	float get_dt();

	Window window;
	std::chrono::high_resolution_clock::time_point new_frame_time;

//	static std::vector<std::function<void()>> _components_init;
	static std::vector<std::function<void(float)>> _components_update;
	static std::vector<std::function<void(float)>> _components_render;
	static std::vector<std::function<void()>> _components_destroy;
};
