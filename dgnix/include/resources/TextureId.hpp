#pragma once
#include <type_traits>
#include <cstdint>

using TextureId = std::conditional<sizeof(void*) == sizeof(std::int32_t), std::int32_t, std::int64_t>::type;
