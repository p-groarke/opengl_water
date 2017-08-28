#include "engine/engine.h"
#include "components/editor_camera.h"
#include "components/water.h"
#include "components/rotation_camera.h"

static const char* project_name = "Ripples";
Engine engine{ project_name };

int main(int, char**) {
	Entity camera_e = Entity::new_entity();
	camera_e.add_component<EditorCamera>();

	Entity water_e = Entity::new_entity();
	water_e.add_component<Water>();

	Entity::kill_entity(water_e);

	water_e = Entity::new_entity();
	water_e.add_component<Water>();

	Entity::kill_entity(camera_e);

	Entity t = Entity::new_entity();
	t.add_component<RotationCamera>();
	t.kill();

	Entity camera_e2 = Entity::new_entity();
	camera_e2.add_component<EditorCamera>();
	camera_e2.kill_component<EditorCamera>();
	camera_e2.kill_component<Camera>();
	camera_e2.kill_component<Transform>();
	camera_e2.add_component<EditorCamera>();
	camera_e2.kill_component<Transform>();
	camera_e2.add_component<Transform>();
	camera_e2.kill();

	camera_e2 = Entity::new_entity();
	camera_e2.add_component<EditorCamera>();

	engine.doit();

	return 0;
}
