#pragma once
#include "engine/globals.h"
#include "engine/entity.h"
#include "engine/meta.h"
#include "engine/engine.h"

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <type_traits>
#include <unordered_map>
#include <utility>
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

/* Force unused static object compilation hack. */
template<typename T, T> struct value { };
} // namespace detail

template<class T>
struct Component {
	/* Static constructor */
//	struct _static_ctor {
//		_static_ctor()
//		{
//			printf("component static constructor\n");
//			if constexpr (is_detected_v<detail::has_update, T>) {
//				Engine::component_update(&Component<T>::update);
//			}
//			if constexpr (is_detected_v<detail::has_render, T>) {
//				Engine::component_render(&Component<T>::render);
//			}
//			if constexpr (is_detected_v<detail::has_destroy, T>) {
//				Engine::component_destroy(&Component<T>::destroy);
//			}
//			Entity::component_kill(
//					static_cast<void(*)(Entity)>(&Component<T>::kill_component));
//		}
//		static constexpr unsigned char force_compilation = 42;
//		typedef detail::value<unsigned char&, force_compilation> value_user;
//	};

	Component(Entity e = Entity::dummy)
		: entity(e)
	{
		if (!Component<T>::callbacks_set && entity != Entity::dummy) {
//			if constexpr (is_detected_v<detail::has_init, T>) {
//				Engine::component_init(&Component<T>::init);
//			}
			if constexpr (is_detected_v<detail::has_update, T>) {
				Engine::component_update(&Component<T>::update_components);
			}
			if constexpr (is_detected_v<detail::has_render, T>) {
				Engine::component_render(&Component<T>::render_components);
			}
			if constexpr (is_detected_v<detail::has_destroy, T>) {
				Engine::component_destroy(&Component<T>::destroy_components);
			}
			Entity::on_component_kill(
					static_cast<void(*)(Entity)>(&Component<T>::kill_component));

			Component<T>::callbacks_set = true;
		}
	}

	static void* operator new(size_t) = delete;
	static void* operator new[](size_t) = delete;

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
	static void init_components();
	static void update_components(float dt);
	static void render_components(float dt);
	static void destroy_components();

	static std::unordered_map<size_t, size_t> _lut;
	static std::vector<T> _components;
//	static _static_ctor _s_ctor;
	static bool callbacks_set;
};

template <class T>
inline Component<T>::operator bool() const {
	return _lut.count(entity.id()) != 0;
}

template <class T>
inline T* Component<T>::operator->() const {
	assert(*this == true && "Component doesn't exist.");
	return &_components[_lut[entity.id()]];
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
	T t;
	t.entity = e;
	printf("Constructing %s Component. Entity : ", typeid(T).name());
	t.entity.debug_print();

	_components.emplace_back(std::move(t));
//	_components.emplace_back(T{e});
	_lut[e.id()] = _components.size() - 1;

	if constexpr (is_detected_v<detail::has_init, T>) {
		_components.back().init();
	}

	return Component<T>{e};
}

template <class T>
void Component<T>::kill_component(Entity e) {
	if (_lut.count(e.id()) == 0)
		return;

	printf("Killing %s Component. Entity : ", typeid(T).name());
	e.debug_print();

	size_t pos = _lut[e.id()];
	_lut[_components.back().entity.id()] = pos;
	if constexpr (is_detected_v<detail::has_destroy, T>) {
		_components[pos].destroy();
	}
	if (_components[pos].entity.id() != _components.back().entity.id()) {
		std::swap(_components[pos], _components.back());
	}
	_components.pop_back();
	_lut.erase(e.id());
}

template <class T>
void Component<T>::init_components() {
	if constexpr (is_detected_v<detail::has_init, T>) {
		for (size_t i = 0; i < _components.size(); ++i) {
			_components[i].init();
		}
	}
}

template <class T>
void Component<T>::update_components(float dt) {
	if constexpr (is_detected_v<detail::has_update, T>) {
		for (size_t i = 0; i < _components.size(); ++i) {
			_components[i].update(dt);
		}
	}
}

template <class T>
void Component<T>::render_components(float dt) {
	if constexpr (is_detected_v<detail::has_render, T>) {
		for (size_t i = 0; i < _components.size(); ++i) {
			_components[i].render(dt);
		}
	}
}

template <class T>
void Component<T>::destroy_components() {
	if constexpr (is_detected_v<detail::has_destroy, T>) {
		for (size_t i = 0; i < _components.size(); ++i) {
			_components[i].destroy();
		}
	}
}

template <class T> std::unordered_map<size_t, size_t> Component<T>::_lut = {};
template <class T> std::vector<T> Component<T>::_components = {};
template <class T> bool Component<T>::callbacks_set = false;
//template <class T> typename Component<T>::_static_ctor Component<T>::_s_ctor{};
//template <class T> unsigned char Component<T>::_static_ctor::force_compilation = 42;
