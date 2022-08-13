#pragma once

#include <array>
#include <bit>
#include <climits>
#include <cstdint>
#include <type_traits>

// Utility class for making a callable object with overloaded operator() to be used in std::visit. Can create a
// pseudo match statement like Rust:
//     match {
//         [](const first_type &) { },
//         [](const second_type &) { },
//     }
template<typename... Ts>
struct match : Ts...
{
    using Ts::operator()...;
};

template<typename... Ts>
match(Ts...) -> match<Ts...>;

template<std::integral Integral>
std::array<std::uint8_t, sizeof(Integral)> to_le_bytes(Integral value)
{
    if constexpr (std::endian::native == std::endian::big)
    {
        value = std::byteswap(value);
    }

    constexpr std::size_t byte_count = sizeof(Integral);
    std::array<std::uint8_t, byte_count> bytes;

    for (std::size_t i = 0; i < byte_count; ++i)
    {
        bytes[i] = static_cast<std::uint8_t>((value >> (CHAR_BIT * i)) & 0xFF);
    }

    return bytes;
}

constexpr char to_ascii(std::uint8_t byte, char fallback = '.')
{
    if (31 < byte && byte < 128)
    {
        return static_cast<char>(byte);
    }
    else
    {
        return fallback;
    }
}
