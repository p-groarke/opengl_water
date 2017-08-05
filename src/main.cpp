#include "engine/globals.h"
#include "engine/window.h"
#include "engine/entity.h"
#include "components/camera.h"
#include "components/rotation_camera.h"
#include "components/water.h"
#include "components/renderer.h"

#include <cstdlib>
#include <chrono>
#include <memory>
#include <vector>

static const char* project_name = "Ripples";
Window window{ project_name };

int main(int, char**) {
	printf("%s\n", project_name);

	std::vector<std::unique_ptr<Entity>> entities;

	entities.emplace_back(std::make_unique<Entity>());
	Entity* camera_e = entities.back().get();
	camera_e->add_component<RotationCamera>();

	entities.emplace_back(std::make_unique<Entity>());
	Entity* water_e = entities.back().get();
	water_e->add_component<Water>();

	auto new_frame_t = std::chrono::high_resolution_clock::now();
//	for (const auto& x : entities) {
//		x->init();
//	}
	while (!glfwWindowShouldClose(window.window)) {
		const auto last_frame_t = new_frame_t;
		new_frame_t = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> dt_duration
				= new_frame_t - last_frame_t;
		const float dt = dt_duration.count();

		for (const auto& x : entities) {
			x->update(dt);
		}

		for (const auto& x : entities) {
			x->render(dt);
		}

		window.post_render();
		GL_CHECK_ERROR();
	}

	for (const auto& x : entities) {
		x->destroy();
	}
	return 0;
}
