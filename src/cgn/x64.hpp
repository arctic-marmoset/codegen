#pragma once

#include <cstdint>
#include <utility>

namespace x64
{
    enum class reg : std::uint8_t
    {
        rax,
    };

    constexpr std::uint8_t to_index(reg reg)
    {
        return std::to_underlying(reg) % 8;
    }

    constexpr std::uint8_t ret = 0xC3;

    constexpr std::uint8_t mov(reg reg)
    {
        return 0xB8 + to_index(reg);
    }
}
