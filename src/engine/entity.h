#pragma once
#include "globals.h"
#include "component.h"

#include <memory>
#include <vector>

struct Entity {
	Entity()
		: _id(++id_count)
	{}

	template <class T>
	T* add_component() {
		static_assert(std::is_base_of<Component, T>::value
				, "Your component needs to inherit Component.");

		for (const auto& x : _components) {
			if (dynamic_cast<T*>(x.get()) != nullptr) {
				OUTPUT_ERROR("Can't add duplicate components in entity.");
				return nullptr;
			}
		}

		_components.emplace_back(std::make_unique<T>());
		return dynamic_cast<T*>(_components.back().get());
	}

	template <class T>
	T* get_component() {
		for (const auto& x : _components) {
			T* ret = dynamic_cast<T*>(x.get());
			if (ret != nullptr)
				return ret;
		}
		return nullptr;
	}

private:
	static size_t id_count;
	size_t _id;
	std::vector<std::unique_ptr<Component>> _components;
};

size_t Entity::id_count = 0;
