#include "engine/entity.h"

namespace ts {

std::size_t global_component_index = 0;

size_t Entity::global_id_count = 0;

ComponentManager Entity::_component_manager;

std::size_t ComponentManager::_actual_component_count = 0;

} // namespace ts
