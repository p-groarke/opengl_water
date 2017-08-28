#include "engine/entity.h"
#include "engine/engine.h"

#include <limits>

Entity::Entity()
	: _id(++_id_count)
{}

Entity::Entity(size_t id)
	: _id(id)
{}

Entity Entity::add_entity() {
	_entities.emplace_back(Entity{});
	return _entities.back();
}

void Entity::kill_entity(Entity e) {
	for (size_t i = 0; i < _components_kill.size(); ++i) {
		_components_kill[i](e);
	}
	_entities.back()._id = e._id;
	std::swap(_entities[e._id], _entities.back());
	_entities.pop_back();
}

void Entity::on_component_kill(std::function<void(Entity)>&& f) {
	Entity::_components_kill.emplace_back(f);
}

const Entity Entity::dummy{ std::numeric_limits<size_t>::max() };
size_t Entity::_id_count = 0;
std::vector<Entity> Entity::_entities = {};
std::vector<std::function<void(Entity)>> Entity::_components_kill = {};
