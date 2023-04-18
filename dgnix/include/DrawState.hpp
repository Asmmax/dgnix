#pragma once
#include "UnorderedMap.hpp"
#include "StringId.hpp"
#include "glm/glm.hpp"
#include <tuple>
#include <utility>

class Shader;

template<typename... Types>
class DrawState
{
private:
	std::tuple<UnorderedMap<StringId,Types>...> _maps;
	DrawState* _parentState;

public:
	DrawState(DrawState* parentState = nullptr);

	template <typename Type>
	void add(const StringId& name, const Type& value);

	template <typename Type>
	void addOrSet(const StringId& name, const Type& value);

	template <typename Type>
	void set(const StringId& name, const Type& value);

	template <typename Type>
	const Type& get(const StringId& name) const;

	void remove(const StringId& name);
	void clear();

	bool has(const StringId& name) const;

	void apply(Shader& shader);
	void apply(DrawState& otherState);

private:
	template<typename Type, std::size_t... Is>
	void add(const StringId& name, const Type& value, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename Type, typename CurrentType, typename... Remains>
	void add(const StringId& name, const Type& value, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename Type, typename... Remains>
	void add(const StringId& name, const Type& value, UnorderedMap<StringId, Type>& currentMap, UnorderedMap<StringId, Remains>&... remains);


	template<typename Type, std::size_t... Is>
	void addOrSet(const StringId& name, const Type& value, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename Type, typename CurrentType, typename... Remains>
	void addOrSet(const StringId& name, const Type& value, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename Type, typename... Remains>
	void addOrSet(const StringId& name, const Type& value, UnorderedMap<StringId, Type>& currentMap, UnorderedMap<StringId, Remains>&... remains);


	template<typename Type, std::size_t... Is>
	void set(const StringId& name, const Type& value, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename Type, typename CurrentType, typename... Remains>
	void set(const StringId& name, const Type& value, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename Type, typename... Remains>
	void set(const StringId& name, const Type& value, UnorderedMap<StringId, Type>& currentMap, UnorderedMap<StringId, Remains>&... remains);


	template<typename Type, std::size_t... Is>
	const Type& get(const StringId& name, const std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>) const;

	template<typename Type, typename CurrentType, typename... Remains>
	const Type& get(const StringId& name, const Type& dummy, const UnorderedMap<StringId, CurrentType>& currentMap, const UnorderedMap<StringId, Remains>&... remains) const;

	template<typename Type, typename... Remains>
	const Type& get(const StringId& name, const Type& dummy, const UnorderedMap<StringId, Type>& currentMap, const UnorderedMap<StringId, Remains>&... remains) const;


	template<std::size_t... Is>
	void remove(const StringId& name, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename CurrentType, typename... Remains>
	void remove(const StringId& name, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename LastType>
	void remove(const StringId& name, UnorderedMap<StringId, LastType>& lastMap);


	template<std::size_t... Is>
	void clear(std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename CurrentType, typename... Remains>
	void clear(UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename LastType>
	void clear(UnorderedMap<StringId, LastType>& lastMap);


	template<std::size_t... Is>
	bool has(const StringId& name, const std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>) const;

	template<typename CurrentType, typename... Remains>
	bool has(const StringId& name, const UnorderedMap<StringId, CurrentType>& currentMap, const UnorderedMap<StringId, Remains>&... remains) const;

	template<typename LastType>
	bool has(const StringId& name, const UnorderedMap<StringId, LastType>& lastMap) const;


	template<std::size_t... Is>
	void apply(Shader& shader, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename CurrentType, typename... Remains>
	void apply(Shader& shader, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename Type>
	void apply(Shader& shader, UnorderedMap<StringId, Type>& map);


	template<std::size_t... Is>
	void apply(DrawState& otherState, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename CurrentType, typename... Remains>
	void apply(DrawState& otherState, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename Type>
	void apply(DrawState& otherState, UnorderedMap<StringId, Type>& map);
};


template<typename... Types>
class DrawStatePool
{
private:
	std::vector<DrawState<Types...>> _pool;
	size_t _cursor;
public:
	DrawStatePool(size_t size);

	DrawState<Types...>& get();

	void push();
	void pop();
};


using DrawStateDef = DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>;
using DrawStatePoolDef = DrawStatePool<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>;
