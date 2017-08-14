#pragma once
#include "engine/globals.h"

struct Entity;

struct Component {
	virtual void init() {}
	virtual void update(float) {}
	virtual void render(float) {}
	virtual void destroy() {}

//	template <class T> T* add_component();
//	template <class T> T* get_component();
//	template <class T> void kill_component();

	Entity* entity = nullptr;
};

// Header hell...
//template <class T>
//T* Component::add_component() {
//	return entity->add_component<T>();
//}
//
//template <class T>
//T* Component::get_component() {
//	return entity->get_component<T>();
//}
//
//template <class T>
//void Component::kill_component() {
//	entity->kill_component<T>();
//}
