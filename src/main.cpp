#include "engine/engine.h"
#include "components/editor_camera.h"
#include "components/water.h"

static const char* project_name = "Ripples";
Engine engine{ project_name };

int main(int, char**) {
	Entity camera_e = Entity::add_entity();
	camera_e.add_component<EditorCamera>();

	Entity water_e = Entity::add_entity();
	water_e.add_component<Water>();

	engine.doit();

	return 0;
}
