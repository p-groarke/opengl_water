#include "engine/entity.h"

Entity::Entity()
	: _id(++id_count)
{}

void Entity::init_components() {
	// No ranged for, in case user adds components.
	for (size_t i = 0; i < _components.size(); ++i) {
		_components[i]->init();
	}
}

void Entity::update_components(float dt) {
	for (size_t i = 0; i < _components.size(); ++i) {
		_components[i]->update(dt);
	}
}

void Entity::render_components(float dt) {
	for (size_t i = 0; i < _components.size(); ++i) {
		_components[i]->render(dt);
	}
}

void Entity::destroy_components() {
	for (size_t i = 0; i < _components.size(); ++i) {
		_components[i]->destroy();
	}
}

Entity* Entity::add_entity() {
	_entities.emplace_back(std::make_unique<Entity>());
	return _entities.back().get();
}

void Entity::kill_entity(Entity* e) {
	for (size_t i = 0; i < _entities.size(); ++i) {
		if (e == _entities[i].get()) {
			e->destroy_components();
			std::swap(_entities[i], _entities.back());
			_entities.pop_back();
			return;
		}
	}
}

void Entity::init_entities() {
	for (size_t i = 0; i < _entities.size(); ++i) {
		_entities[i]->init_components();
	}
}

void Entity::update_entities(float dt) {
	for (size_t i = 0; i < _entities.size(); ++i) {
		_entities[i]->update_components(dt);
	}
}

void Entity::render_entities(float dt) {
	for (size_t i = 0; i < _entities.size(); ++i) {
		_entities[i]->render_components(dt);
	}
}

void Entity::destroy_entities() {
	for (size_t i = 0; i < _entities.size(); ++i) {
		_entities[i]->destroy_components();
	}
}

size_t Entity::id_count = 0;
std::vector<std::unique_ptr<Entity>> Entity::_entities = {};

