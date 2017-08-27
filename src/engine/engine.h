#pragma once
#include "engine/window.h"

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
	Window window;

//	static std::vector<std::function<void()>> _components_init;
	static std::vector<std::function<void(float)>> _components_update;
	static std::vector<std::function<void(float)>> _components_render;
	static std::vector<std::function<void()>> _components_destroy;
};
