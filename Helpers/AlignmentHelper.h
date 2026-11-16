#pragma once

#include <cstdint>

inline uint32_t AlignUp(uint32_t value, uint32_t alignment)
{
    return (value + alignment - 1) / alignment * alignment;
}
