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
	explicit DrawState(DrawState* parentState = nullptr);

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

	void apply(Shader& shader) const;
	void apply(DrawState& otherState) const;

private:
	template<typename Type, std::size_t... Is>
	static void add(const StringId& name, const Type& value, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename Type, typename CurrentType, typename... Remains>
	static void add(const StringId& name, const Type& value, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename Type, typename... Remains>
	static void add(const StringId& name, const Type& value, UnorderedMap<StringId, Type>& currentMap, UnorderedMap<StringId, Remains>&... remains);


	template<typename Type, std::size_t... Is>
	static void addOrSet(const StringId& name, const Type& value, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename Type, typename CurrentType, typename... Remains>
	static void addOrSet(const StringId& name, const Type& value, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename Type, typename... Remains>
	static void addOrSet(const StringId& name, const Type& value, UnorderedMap<StringId, Type>& currentMap, UnorderedMap<StringId, Remains>&... remains);


	template<typename Type, std::size_t... Is>
	static void set(const StringId& name, const Type& value, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename Type, typename CurrentType, typename... Remains>
	static void set(const StringId& name, const Type& value, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename Type, typename... Remains>
	static void set(const StringId& name, const Type& value, UnorderedMap<StringId, Type>& currentMap, UnorderedMap<StringId, Remains>&... remains);


	template<typename Type, std::size_t... Is>
	const Type& get(const StringId& name, const std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>) const;

	template<typename Type, typename CurrentType, typename... Remains>
	const Type& get(const StringId& name, const Type& dummy, const UnorderedMap<StringId, CurrentType>& currentMap, const UnorderedMap<StringId, Remains>&... remains) const;

	template<typename Type, typename... Remains>
	const Type& get(const StringId& name, const Type& dummy, const UnorderedMap<StringId, Type>& currentMap, const UnorderedMap<StringId, Remains>&... remains) const;


	template<std::size_t... Is>
	static void remove(const StringId& name, std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename CurrentType, typename... Remains>
	static void remove(const StringId& name, UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename LastType>
	static void remove(const StringId& name, UnorderedMap<StringId, LastType>& lastMap);


	template<std::size_t... Is>
	static void clear(std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename CurrentType, typename... Remains>
	static void clear(UnorderedMap<StringId, CurrentType>& currentMap, UnorderedMap<StringId, Remains>&... remains);

	template<typename LastType>
	static void clear(UnorderedMap<StringId, LastType>& lastMap);


	template<std::size_t... Is>
	bool has(const StringId& name, const std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>) const;

	template<typename CurrentType, typename... Remains>
	bool has(const StringId& name, const UnorderedMap<StringId, CurrentType>& currentMap, const UnorderedMap<StringId, Remains>&... remains) const;

	template<typename LastType>
	bool has(const StringId& name, const UnorderedMap<StringId, LastType>& lastMap) const;


	template<std::size_t... Is>
	static void apply(Shader& shader, const std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename CurrentType, typename... Remains>
	static void apply(Shader& shader, const UnorderedMap<StringId, CurrentType>& currentMap, const UnorderedMap<StringId, Remains>&... remains);

	template<typename Type>
	static void apply(Shader& shader, const UnorderedMap<StringId, Type>& map);


	template<std::size_t... Is>
	static void apply(DrawState& otherState, const std::tuple<UnorderedMap<StringId, Types>...>& tuple, std::index_sequence<Is...>);

	template<typename CurrentType, typename... Remains>
	static void apply(DrawState& otherState, const UnorderedMap<StringId, CurrentType>& currentMap, const UnorderedMap<StringId, Remains>&... remains);

	template<typename Type>
	static void apply(DrawState& otherState, const UnorderedMap<StringId, Type>& map);
};


template<typename... Types>
class DrawStatePool
{
private:
	std::vector<DrawState<Types...>> _pool;
	size_t _cursor;
public:
	explicit DrawStatePool(size_t size);

	DrawState<Types...>& get();

	void push();
	void pop();
};


using DrawStateDef = DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>;
using DrawStatePoolDef = DrawStatePool<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>;


extern template class DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>;
extern template class DrawStatePool<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>;

extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const glm::mat4& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const glm::mat3& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const glm::vec4& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const glm::vec3& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const float& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::add(const StringId& name, const int& value);

extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const glm::mat4& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const glm::mat3& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const glm::vec4& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const glm::vec3& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const float& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::addOrSet(const StringId& name, const int& value);

extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const glm::mat4& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const glm::mat3& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const glm::vec4& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const glm::vec3& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const float& value);
extern template void DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::set(const StringId& name, const int& value);

extern template const glm::mat4& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<glm::mat4>(const StringId& name) const;
extern template const glm::mat3& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<glm::mat3>(const StringId& name) const;
extern template const glm::vec4& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<glm::vec4>(const StringId& name) const;
extern template const glm::vec3& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<glm::vec3>(const StringId& name) const;
extern template const float& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<float>(const StringId& name) const;
extern template const int& DrawState<glm::mat4, glm::mat3, glm::vec4, glm::vec3, float, int>::get<int>(const StringId& name) const;
