#pragma once
#include "globals.h"
#include "component.h"

#include <memory>
#include <vector>

struct Entity {
	Entity();

//	void init();
	void update(float dt);
	void render(float dt);
	void destroy();

	template <class T>
	T* add_component();

	template <class T>
	T* get_component();

private:
	static size_t id_count;
	size_t _id;
	std::vector<std::unique_ptr<Component>> _components;
};

template <class T>
T* Entity::add_component() {
	static_assert(std::is_base_of<Component, T>::value
			, "Your component needs to inherit Component.");

	if (T* ret = get_component<T>()) {
		OUTPUT_ERROR("Can't add duplicate components in entity.");
		return ret;
	}

	_components.emplace_back(std::make_unique<T>());
	Component* c = _components.back().get();
	c->entity = this;
	c->init();
	return dynamic_cast<T*>(c);
}

template <class T>
T* Entity::get_component() {
	for (const auto& x : _components) {
		T* ret = dynamic_cast<T*>(x.get());
		if (ret != nullptr)
			return ret;
	}
	return nullptr;
}
