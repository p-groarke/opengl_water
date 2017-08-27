#pragma once
#include "engine/globals.h"
#include "engine/entity.h"
#include "engine/meta.h"
#include "engine/engine.h"

#include <cstdlib>
#include <algorithm>
#include <map>
#include <type_traits>
#include <vector>

namespace detail {
template <class U>
using has_init = decltype(std::declval<U>().init());
template <class U>
using has_update = decltype(std::declval<U>().update(std::declval<float>()));
template <class U>
using has_render = decltype(std::declval<U>().render(std::declval<float>()));
template <class U>
using has_destroy = decltype(std::declval<U>().destroy());
} // namespace detail

template<class T>
struct Component {
	Component(Entity e)
		: entity(e)
	{
		if (!Component<T>::callbacks_set) {
//			if constexpr (is_detected_v<detail::has_init, T>) {
//				Engine::component_init(&Component<T>::init);
//			}
			if constexpr (is_detected_v<detail::has_update, T>) {
				Engine::component_update(&Component<T>::update);
			}
			if constexpr (is_detected_v<detail::has_render, T>) {
				Engine::component_render(&Component<T>::render);
			}
			if constexpr (is_detected_v<detail::has_destroy, T>) {
				Engine::component_destroy(&Component<T>::destroy);
			}
			Entity::component_kill(
					static_cast<void(*)(Entity)>(&Component<T>::kill_component));

			Component<T>::callbacks_set = true;
		}
	}

	static void* operator new(std::size_t) = delete;
	static void* operator new[](std::size_t) = delete;

	inline operator bool() const;
	inline T* operator->() const; /* Get this component shortcut. */

	template <class U> Component<U> add_component();
	template <class U> Component<U> get_component();
	template <class U> void kill_component();

	static Component<T> add_component(Entity e);
	static void kill_component(Entity e);

protected:
	Entity entity;

private:
	static void init();
	static void update(float dt);
	static void render(float dt);
	static void destroy();

	static std::map<Entity, size_t> _lut;
	static std::vector<T> _components;
	static bool callbacks_set;
};

template <class T>
inline Component<T>::operator bool() const {
	return _lut.count(entity) != 0;
}

template <class T>
inline T* Component<T>::operator->() const {
	if (!*this)
		return nullptr;
	return &_components[_lut[entity]];
}

template <class T>
template <class U>
Component<U> Component<T>::add_component() {
	static_assert(std::is_base_of<Component<U>, U>::value,
			"Components must inherit Component<>.");
	return entity.add_component<U>();
}

template <class T>
template <class U>
Component<U> Component<T>::get_component() {
	static_assert(std::is_base_of<Component<U>, U>::value,
			"Components must inherit Component<>.");
	return Component<U>{entity};
}

template <class T>
template <class U>
void Component<T>::kill_component() {
	static_assert(std::is_base_of<Component<U>, U>::value,
			"Components must inherit Component<>.");
	entity.kill_component<U>();
}

template <class T>
Component<T> Component<T>::add_component(Entity e) {
	_components.emplace_back(T{e});
	_lut[e] = _components.size() - 1;

	if constexpr (is_detected_v<detail::has_init, T>) {
		_components.back().init();
	}

	return Component<T>{e};
}

template <class T>
void Component<T>::kill_component(Entity e) {
	if (_lut.count(e) == 0)
		return;

	_lut[_components.back().entity] = _lut[e];
	if constexpr (is_detected_v<detail::has_destroy, T>) {
		_components[_lut[e]].destroy();
	}
	std::swap(_components[_lut[e]], _components.back());

	_components.pop_back();
	_lut.erase(e);
}

template <class T>
void Component<T>::init() {
	if constexpr (is_detected_v<detail::has_init, T>) {
		for (size_t i = 0; i < _components.size(); ++i) {
			_components[i].init();
		}
	}
}

template <class T>
void Component<T>::update(float dt) {
	if constexpr (is_detected_v<detail::has_update, T>) {
		for (size_t i = 0; i < _components.size(); ++i) {
			_components[i].update(dt);
		}
	}
}

template <class T>
void Component<T>::render(float dt) {
	if constexpr (is_detected_v<detail::has_render, T>) {
		for (size_t i = 0; i < _components.size(); ++i) {
			_components[i].render(dt);
		}
	}
}

template <class T>
void Component<T>::destroy() {
	if constexpr (is_detected_v<detail::has_destroy, T>) {
		for (size_t i = 0; i < _components.size(); ++i) {
			_components[i].destroy();
		}
	}
}

template <class T> std::map<Entity, size_t> Component<T>::_lut = {};
template <class T> std::vector<T> Component<T>::_components = {};
template <class T> bool Component<T>::callbacks_set = false;
