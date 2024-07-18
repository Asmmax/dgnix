#pragma once
#include <type_traits>
#include <cstdint>

struct BufferId
{
	unsigned int value;

	BufferId() : value(0) {}
	explicit BufferId(unsigned int id) : value(id) {}
	operator unsigned int() const { return value; }
	operator bool() const { return value != 0; }
};
