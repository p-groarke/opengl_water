#include "engine/globals.h"
#include "engine/window.h"
#include "engine/entity.h"
#include "components/camera.h"
#include "components/rotation_camera.h"
#include "components/editor_camera.h"
#include "components/water.h"
#include "components/renderer.h"

#include <cstdlib>
#include <chrono>
#include <memory>
#include <vector>

#include <unistd.h>
static const char* project_name = "Ripples";
Window window{ project_name };

int main(int, char**) {
	app::init_executable_path();

	Entity* camera_e = Entity::add_entity();
	camera_e->add_component<EditorCamera>();

	Entity* water_e = Entity::add_entity();
	water_e->add_component<Water>();

	auto new_frame_t = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window.window)) {
		const auto last_frame_t = new_frame_t;
		new_frame_t = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> dt_duration
				= new_frame_t - last_frame_t;
		const float dt = dt_duration.count();

		Entity::update_entities(dt);
		Entity::render_entities(dt);

		window.post_render();
		GL_CHECK_ERROR();
	}
	Entity::destroy_entities();
	return 0;
}
