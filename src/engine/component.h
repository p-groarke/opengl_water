#pragma once

#include "engine/globals.h"

#include <memory>
#include <vector>

namespace ts {
class ComponentBase;
struct Entity;

class ComponentManager {
public:
	static constexpr std::size_t maximum_entity = TS_MAXIMUM_ENTITY;
	static constexpr std::size_t maximum_component = TS_MAXIMUM_COMPONENT;

	class ComponentVector {
	public:
		inline ~ComponentVector()
		{
		}

		inline ComponentVector()
			: _begin(nullptr)
			, _size(0)
		{
		}

		inline ComponentVector(ComponentVector&& cv)
			: _begin(std::move(cv._begin))
			, _size(std::move(cv._size))
		{
		}

		inline ComponentVector(const ComponentVector& cv) = delete;

		inline ComponentVector& operator=(const ComponentVector& cv) = delete;

		inline ComponentVector& operator=(ComponentVector&& cv) = delete;

		inline std::size_t size() const
		{
			return _size;
		}

		static constexpr inline std::size_t capacity()
		{
			return ComponentManager::maximum_component;
		}

		inline void push_back(const std::shared_ptr<ComponentBase>& component)
		{
			*(_begin + _size++) = component;
			assert_error(_size <= capacity(), "Maximum amount of component has been reached.");
		}

		inline void emplace_back(std::shared_ptr<ComponentBase>&& component)
		{
			*(_begin + _size++) = component;
			assert_error(_size <= capacity(), "Maximum amount of component has been reached.");
		}

		/// Undefined if size is zero.
		inline void pop_back()
		{
			assert_error(_size, "Can't call pop_back() on an empty vector.");
			(_begin + --_size)->reset();
		}

		/// Undefined if size is zero.
		inline std::shared_ptr<ComponentBase>& back()
		{
			assert_error(_size, "Can't call back() on an empty vector.");
			return *(_begin + _size - 1);
		}

		/// Undefined if size is zero.
		inline const std::shared_ptr<ComponentBase>& back() const
		{
			assert_error(_size, "Can't call back() on an empty vector.");
			return *(_begin + _size - 1);
		}

		inline std::shared_ptr<ComponentBase>& operator[](std::size_t i)
		{
			assert_error(i < _size, "Index exceed vector size.");
			return *(_begin + i);
		}

		inline const std::shared_ptr<ComponentBase>& operator[](std::size_t i) const
		{
			assert_error(i < _size, "Index exceed vector size.");
			return *(_begin + i);
		}

		//
		// Range loop.
		//

		inline std::shared_ptr<ComponentBase>* begin()
		{
			return _begin;
		}

		inline std::shared_ptr<ComponentBase>* end()
		{
			return _begin + size();
		}

		inline const std::shared_ptr<ComponentBase>* begin() const
		{
			return _begin;
		}

		inline const std::shared_ptr<ComponentBase>* end() const
		{
			return _begin + size();
		}

	private:
		friend ComponentManager;

		inline ComponentVector(std::shared_ptr<ComponentBase>* begin)
			: _begin(begin)
		{
		}

		std::shared_ptr<ComponentBase>* _begin;
		std::size_t _size = 0;
	};

	ComponentManager()
	{
		_components = new std::shared_ptr<ComponentBase>[maximum_entity * maximum_component];
		_component_vectors.reserve(maximum_entity);
		_entity_vector.reserve(maximum_entity);

		// Entities starts at 1.
		std::size_t index = ComponentManager::maximum_component;
		for (std::size_t i = 1; i < maximum_entity; i++, index += ComponentManager::maximum_component) {
			_component_vectors.push_back(ComponentVector(&_components[index]));
		}
	}

	~ComponentManager()
	{
		delete[] _components;
	}

	inline Entity create_entity();

	inline ComponentVector& get_component_vector(std::size_t entity)
	{
		return _component_vectors[entity];
	}

	inline ComponentVector& operator[](std::size_t entity)
	{
		return _component_vectors[entity];
	}

	//    inline std::vector<Entity>& get_entities() {
	//        return _entity_vector;
	//    }

	inline const std::vector<Entity>& get_entities() const
	{
		return _entity_vector;
	}

private:
	std::shared_ptr<ComponentBase>* _components;
	std::vector<ComponentVector> _component_vectors;
	std::vector<Entity> _entity_vector;
};

struct Entity {
	inline Entity();

	inline Entity(std::size_t id)
		: _id(id)
	{
	}

	inline Entity(const Entity& entity)
		: _id(entity._id)
	{
	}

	inline std::size_t id() const
	{
		return _id;
	}

	operator std::size_t() const
	{
		return _id;
	}

	inline void init() const;
	inline void update(float dt) const;
	inline void render(float dt) const;
	inline void destroy() const;

	template <typename T, typename... Args>
	T* add_component(Args... args);

	template <typename T>
	T* get_component();

	template <typename T>
	void kill_component();

	static Entity create()
	{
		return _component_manager.create_entity();
	}

	static const std::vector<Entity>& all()
	{
		return _component_manager.get_entities();
	}

private:
	static size_t global_id_count;
	size_t _id;
	static ComponentManager _component_manager;
};

inline Entity ComponentManager::create_entity()
{
	Entity entity;
	_entity_vector.push_back(entity);
	return entity;
}

template <typename T, typename = void>
struct _has_init : std::false_type {
};

template <typename T>
struct _has_init<T,
	typename std::enable_if<
		std::is_same<decltype(std::declval<T>().init(std::declval<Entity>())), void>::value>::type>
	: std::true_type {
};

template <typename T, typename = void>
struct _has_update : std::false_type {
};

template <typename T>
struct _has_update<T,
	typename std::enable_if<
		std::is_same<decltype(std::declval<T>().update(std::declval<Entity>(), std::declval<float>())),
			void>::value>::type> : std::true_type {
};

template <typename T, typename = void>
struct _has_render : std::false_type {
};

template <typename T>
struct _has_render<T,
	typename std::enable_if<
		std::is_same<decltype(std::declval<T>().render(std::declval<Entity>(), std::declval<float>())),
			void>::value>::type> : std::true_type {
};

template <typename T, typename = void>
struct _has_destroy : std::false_type {
};

template <typename T>
struct _has_destroy<T,
	typename std::enable_if<
		std::is_same<decltype(std::declval<T>().destroy(std::declval<Entity>())), void>::value>::type>
	: std::true_type {
};

template <typename T, typename = void>
struct _is_data : std::false_type {
};

template <typename T>
struct _is_data<T,
	typename std::enable_if<std::is_same<decltype(std::declval<T>().is_data()), bool>::value>::type>
	: std::true_type {
};

template <typename T>
struct ComponentTraits {
	typedef _has_init<T> has_init;
	typedef _has_update<T> has_update;
	typedef _has_render<T> has_render;
	typedef _has_destroy<T> has_destroy;
	typedef _is_data<T> is_data;
};

extern std::size_t global_component_index;

template <typename T>
struct ComponentIndex {
	static const std::size_t index;
};

template <typename T>
const std::size_t ComponentIndex<T>::index = ++global_component_index;

class ComponentBase {
public:
	inline ComponentBase(std::size_t i = 0)
		: cindex(i)
	{
	}

	template <typename T>
	T* get()
	{
		if (ComponentIndex<T>::index != cindex) {
			return nullptr;
		}

		return static_cast<T*>(data());
	}

	virtual void init(Entity entity) = 0;
	virtual void update(Entity entity, float dt) = 0;
	virtual void render(Entity entity, float dt) = 0;
	virtual void destroy(Entity entity) = 0;

protected:
	virtual void* data() = 0;
	std::size_t cindex;
};

template <typename T>
class Component : public ComponentBase {
public:
	inline Component()
		: ComponentBase(ComponentIndex<T>::index)
	{
	}

protected:
	virtual void init(Entity entity) override
	{
		if constexpr (ComponentTraits<T>::has_init::value) {
			_data.init(entity);
		}
	}
	virtual void update(Entity entity, float dt) override
	{
		if constexpr (ComponentTraits<T>::has_update::value) {
			_data.update(entity, dt);
		}
	}

	virtual void render(Entity entity, float dt) override
	{
		if constexpr (ComponentTraits<T>::has_render::value) {
			_data.render(entity, dt);
		}
	}

	virtual void destroy(Entity entity) override
	{
		if constexpr (ComponentTraits<T>::has_destroy::value) {
			_data.destroy(entity);
		}
	}

private:
	virtual void* data() override
	{
		return &_data;
	}

	T _data;
};

Entity::Entity()
	: _id(++global_id_count)
{
}
template <typename T, typename... Args>
T* Entity::add_component(Args... args)
{
	static_assert(ComponentTraits<T>::has_init::value || ComponentTraits<T>::has_update::value
			|| ComponentTraits<T>::has_render::value || ComponentTraits<T>::has_destroy::value
			|| ComponentTraits<T>::is_data::value,
		"Your component doesn't do shit.");

	if (T* ret = get_component<T>()) {
		OUTPUT_ERROR("Can't add duplicate components in entity.");
		return ret;
	}

	auto& components = _component_manager[_id];
	_component_manager[_id].emplace_back(std::make_shared<Component<T>>(std::forward<Args>(args)...));
	ComponentBase* c = components.back().get();
	c->init(_id);
	return c->get<T>();
}

template <typename T>
T* Entity::get_component()
{
	auto& components = _component_manager[_id];
	for (auto& x : components) {
		if (T* ret = x->get<T>()) {
			return ret;
		}
	}

	return nullptr;
}

template <typename T>
void Entity::kill_component()
{
	auto& components = _component_manager[_id];

	for (size_t i = 0; i < components.size(); ++i) {
		if (T* c = components[i]->get<T>()) {
			c->destroy(_id);

			std::swap(components[i], components.back());
			components.pop_back();
		}
	}
}

inline void Entity::init() const
{
	auto& components = _component_manager[_id];

	for (auto& n : components) {
		n->init(_id);
	}
}

inline void Entity::update(float dt) const
{
	auto& components = _component_manager[_id];

	for (auto& n : components) {
		n->update(_id, dt);
	}
}

inline void Entity::render(float dt) const
{
	auto& components = _component_manager[_id];

	for (auto& n : components) {
		n->render(_id, dt);
	}
}

inline void Entity::destroy() const
{
	auto& components = _component_manager[_id];

	for (auto& n : components) {
		n->destroy(_id);
	}
}
} // namespace ts
