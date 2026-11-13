#pragma once

#include <cstdint>
#include <vector>
#include <type_traits>

template<typename T>
inline void WriteIntegerLittleEndian(std::vector<uint8_t>& bytes, T value)
{
    static_assert(std::is_integral_v<T>);

    using UnT = std::make_unsigned_t<T>;
    UnT unsignedValue = static_cast<UnT>(value);

    for (size_t i = 0; i < sizeof(T); ++i)
        bytes.push_back(static_cast<uint8_t>(unsignedValue >> (i * 8)));
}
