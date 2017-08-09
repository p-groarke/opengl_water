#include "engine/component.h"

namespace ts {
std::size_t global_component_index = 0;
size_t Entity::global_id_count = 0;
ComponentManager Entity::_component_manager;
} // namespace ts
