#pragma once
#include "engine/window.hpp"

#include <cstdio>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

#define OUTPUT_ERROR(format, ...) \
	printf("%s(%d) : %s() :\n  " format "\n" \
	, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define OUTPUT_MSG(format, ...) \
	printf("\n" format "\n", ##__VA_ARGS__)

struct Engine {
	Engine(const char* project_name);
	void doit();

//	static void component_init(std::function<void()>&& f);
	static void component_update(std::function<void(float)>&& f);
	static void component_render(std::function<void(float)>&& f);
	static void component_destroy(std::function<void()>&& f);

	static std::string folder_path;

private:
	Window window;

//	static std::vector<std::function<void()>> _components_init;
	static std::vector<std::function<void(float)>> _components_update;
	static std::vector<std::function<void(float)>> _components_render;
	static std::vector<std::function<void()>> _components_destroy;
};
