//#include "engine/entity.h"
//
// size_t Entity::id_count = 0;
//
// Entity::Entity()
//	: _id(++id_count)
//{}
//
////void Entity::init() {
////	// No ranged for, in case user adds components.
////	for (size_t i = 0; i < _components.size(); ++i) {
////		_components[i]->init();
////	}
////}
//
// void Entity::update(float dt) {
//	for (size_t i = 0; i < _components.size(); ++i) {
//		_components[i]->update(dt);
//	}
//}
//
// void Entity::render(float dt) {
//	for (size_t i = 0; i < _components.size(); ++i) {
//		_components[i]->render(dt);
//	}
//}
//
// void Entity::destroy() {
//	for (size_t i = 0; i < _components.size(); ++i) {
//		_components[i]->destroy();
//	}
//}
