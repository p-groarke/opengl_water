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

int main()
{
	ts::Entity camera_e = ts::Entity::create();
	camera_e.add_component<RotationCamera>();

	ts::Entity water_e = ts::Entity::create();
	water_e.add_component<Water>();

	auto new_frame_t = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window.window)) {
		const auto last_frame_t = new_frame_t;
		new_frame_t = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> dt_duration = new_frame_t - last_frame_t;
		const float dt = dt_duration.count();

		for (const auto& x : ts::Entity::all()) {
			x.update(dt);
		}

		for (const auto& x : ts::Entity::all()) {
			x.render(dt);
		}

		window.post_render();
		GL_CHECK_ERROR();
	}

	water_e.kill_component<Water>();

	for (const auto& x : ts::Entity::all()) {
		x.destroy();
	}

	return 0;
}
