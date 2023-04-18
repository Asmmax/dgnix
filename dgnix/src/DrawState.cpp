#include "DrawState.hpp"
#include "resources/Shader.hpp"
#include <assert.h>

template<typename... Types>
DrawState<Types...>::DrawState(DrawState* parentState) :
	_parentState(parentState)
{
}

template<typename... Types>
template <typename Type>
void DrawState<Types...>::add(const StringId& name, const Type& value)
{
	add(name, value, _maps, std::index_sequence_for<Types...>());
}

template<typename... Types>
template<typename Type, std::size_t... Is>
void DrawState<Types...>::add(const StringId& name, const Type& value, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>)
{
	add(name, value, std::get<Is>(tuple)...);
}

template<typename... Types>
template<typename Type, typename CurrentType, typename... Remains>
void DrawState<Types...>::add(const StringId& name, const Type& value, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	add(name, value, remains...);
}

template<typename... Types>
template<typename Type, typename... Remains>
void DrawState<Types...>::add(const StringId& name, const Type& value, UnorderedMap<StringId, Type>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	currentMap.add(name, value);
}


template<typename... Types>
template <typename Type>
void DrawState<Types...>::addOrSet(const StringId& name, const Type& value)
{
	addOrSet(name, value, _maps, std::index_sequence_for<Types...>());
}

template<typename... Types>
template<typename Type, std::size_t... Is>
void DrawState<Types...>::addOrSet(const StringId& name, const Type& value, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>)
{
	addOrSet(name, value, std::get<Is>(tuple)...);
}

template<typename... Types>
template<typename Type, typename CurrentType, typename... Remains>
void DrawState<Types...>::addOrSet(const StringId& name, const Type& value, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	addOrSet(name, value, remains...);
}

template<typename... Types>
template<typename Type, typename... Remains>
void DrawState<Types...>::addOrSet(const StringId& name, const Type& value, UnorderedMap<StringId, Type>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	auto valuePtr = currentMap.getPtr(name);
	if (valuePtr) {
		*valuePtr = value;
		return;
	}

	add(name, value, currentMap, remains...);
}


template<typename... Types>
template <typename Type>
void DrawState<Types...>::set(const StringId& name, const Type& value)
{
	set(name, value, _maps, std::index_sequence_for<Types...>());
}

template<typename... Types>
template<typename Type, std::size_t... Is>
void DrawState<Types...>::set(const StringId& name, const Type& value, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>)
{
	set(name, value, std::get<Is>(tuple)...);
}

template<typename... Types>
template<typename Type, typename CurrentType, typename... Remains>
void DrawState<Types...>::set(const StringId& name, const Type& value, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	set(name, value, remains...);
}

template<typename... Types>
template<typename Type, typename... Remains>
void DrawState<Types...>::set(const StringId& name, const Type& value, UnorderedMap<StringId, Type>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	currentMap.get(name) = value;
}


template<typename... Types>
template <typename Type>
const Type& DrawState<Types...>::get(const StringId& name) const
{
	return get<Type>(name, _maps, std::index_sequence_for<Types...>());
}

template<typename... Types>
template<typename Type, std::size_t... Is>
const Type& DrawState<Types...>::get(const StringId& name, const std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>) const
{
	static Type dummy;
	return get(name, dummy, std::get<Is>(tuple)...);
}

template<typename... Types>
template<typename Type, typename CurrentType, typename... Remains>
const Type& DrawState<Types...>::get(const StringId& name, const Type& dummy, const UnorderedMap<StringId, CurrentType>& currentMap, const UnorderedMap<StringId, Remains>&... remains) const
{
	return get(name, dummy, remains...);
}

template<typename... Types>
template<typename Type, typename... Remains>
const Type& DrawState<Types...>::get(const StringId& name, const Type&, const UnorderedMap<StringId, Type>& currentMap, const UnorderedMap<StringId, Remains>&... remains) const
{
	const Type* valuePtr = currentMap.getPtr(name);
	if (!valuePtr && _parentState) {
		return _parentState->get<Type>(name);
	}
	assert(valuePtr);
	return *valuePtr;
}


template<typename... Types>
void DrawState<Types...>::remove(const StringId& name)
{
	remove(name, _maps, std::index_sequence_for<Types...>());
}

template<typename... Types>
template<std::size_t... Is>
void DrawState<Types...>::remove(const StringId& name, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>)
{
	remove(name, std::get<Is>(tuple)...);
}

template<typename... Types>
template<typename CurrentType, typename... Remains>
void DrawState<Types...>::remove(const StringId& name, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	if (currentMap.remove(name)) {
		return;
	}
	remove(name, remains...);
}

template<typename... Types>
template<typename LastType>
void DrawState<Types...>::remove(const StringId& name, UnorderedMap<StringId, LastType>& lastMap)
{
	lastMap.remove(name);
}


template<typename... Types>
void DrawState<Types...>::clear()
{
	clear(_maps, std::index_sequence_for<Types...>());
}

template<typename... Types>
template<std::size_t... Is>
void DrawState<Types...>::clear(std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>)
{
	clear(std::get<Is>(tuple)...);
}

template<typename... Types>
template<typename CurrentType, typename... Remains>
void DrawState<Types...>::clear(UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	currentMap.clear();
	clear(remains...);
}

template<typename... Types>
template<typename LastType>
void DrawState<Types...>::clear(UnorderedMap<StringId, LastType>& lastMap)
{
	lastMap.clear();
}


template<typename... Types>
bool DrawState<Types...>::has(const StringId& name) const
{
	return has(name, _maps, std::index_sequence_for<Types...>());
}

template<typename... Types>
template<std::size_t... Is>
bool DrawState<Types...>::has(const StringId& name, const std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>) const
{
	return has(name, std::get<Is>(tuple)...);
}

template<typename... Types>
template<typename CurrentType, typename... Remains>
bool DrawState<Types...>::has(const StringId& name, const UnorderedMap<StringId, CurrentType>& currentMap, const UnorderedMap<StringId, Remains>&... remains) const
{
	if (currentMap.has(name)) {
		return true;
	}
	return has(name, remains...);
}

template<typename... Types>
template<typename LastType>
bool DrawState<Types...>::has(const StringId& name, const UnorderedMap<StringId, LastType>& lastMap) const
{
	if (lastMap.has(name)) {
		return true;
	}
	return _parentState && _parentState->has(name);
}


template<typename... Types>
void DrawState<Types...>::apply(Shader& shader)
{
	if (_parentState) {
		_parentState->apply(shader);
	}
	apply(shader, _maps, std::index_sequence_for<Types...>());
}

template<typename... Types>
template<std::size_t... Is>
void DrawState<Types...>::apply(Shader& shader, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>)
{
	apply(shader, std::get<Is>(tuple)...);
}

template<typename... Types>
template<typename CurrentType, typename... Remains>
void DrawState<Types...>::apply(Shader& shader, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	apply(shader, currentMap);
	apply(shader, remains...);
}

template<typename... Types>
template<typename Type>
void DrawState<Types...>::apply(Shader& shader, UnorderedMap<StringId, Type>& map)
{
	for (size_t i = 0; i < map.size(); i++) {
		auto location = shader.getLocation(map.keys()[i]);
		if (location != -1) {
			shader.setUniform(location, map.values()[i]);
		}
	}
}


template<typename... Types>
void DrawState<Types...>::apply(DrawState& otherState)
{
	if (_parentState) {
		_parentState->apply(otherState);
	}
	apply(otherState, _maps, std::index_sequence_for<Types...>());
}

template<typename... Types>
template<std::size_t... Is>
void DrawState<Types...>::apply(DrawState& otherState, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>)
{
	apply(otherState, std::get<Is>(tuple)...);
}

template<typename... Types>
template<typename CurrentType, typename... Remains>
void DrawState<Types...>::apply(DrawState& otherState, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains)
{
	apply(otherState, currentMap);
	apply(otherState, remains...);
}

template<typename... Types>
template<typename Type>
void DrawState<Types...>::apply(DrawState& otherState, UnorderedMap<StringId, Type>& map)
{
	for (size_t i = 0; i < map.size(); i++) {
		otherState.add(map.keys()[i], map.values()[i]);
	}
}




template<typename... Types>
DrawStatePool<Types...>::DrawStatePool(size_t size) :
	_cursor(-1)
{
	assert(size > 1);
	_pool.reserve(size);
	DrawState<Types...>* parentState = nullptr;
	for (size_t i = 0; i < size; i++) {
		parentState = &_pool.emplace_back(parentState);
	}
}

template<typename... Types>
DrawState<Types...>& DrawStatePool<Types...>::get()
{
	assert(_cursor != -1);
	return _pool[_cursor];
}

template<typename... Types>
void DrawStatePool<Types...>::push()
{
	assert(_cursor == -1 || _cursor < _pool.size() - 1);
	_cursor++;
}

template<typename... Types>
void DrawStatePool<Types...>::pop()
{
	assert(_cursor != -1);
	_pool[_cursor].clear();
	_cursor--;
}

template class DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>;
template class DrawStatePool<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>;


template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const glm::mat4& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const glm::mat3& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const glm::vec4& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const glm::vec3& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const float& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const int& value);

template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const glm::mat4& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const glm::mat3& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const glm::vec4& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const glm::vec3& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const float& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const int& value);

template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const glm::mat4& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const glm::mat3& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const glm::vec4& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const glm::vec3& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const float& value);
template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const int& value);

template const glm::mat4& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<glm::mat4>(const StringId& name) const;
template const glm::mat3& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<glm::mat3>(const StringId& name) const;
template const glm::vec4& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<glm::vec4>(const StringId& name) const;
template const glm::vec3& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<glm::vec3>(const StringId& name) const;
template const float& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<float>(const StringId& name) const;
template const int& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<int>(const StringId& name) const;
