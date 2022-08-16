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

    constexpr std::uint8_t modr(reg destination, reg source)
    {
        return 0b11000000 | to_index(source) << 3 | to_index(destination);
    }

    constexpr std::uint8_t ret = 0xC3;

    constexpr std::uint8_t mov([[maybe_unused]] reg destination, [[maybe_unused]] reg source)
    {
        return 0x89;
    }

    constexpr std::uint8_t mov(reg reg, [[maybe_unused]] std::uint32_t immediate)
    {
        return 0xB8 + to_index(reg);
    }

    constexpr std::uint8_t add([[maybe_unused]] reg destination, [[maybe_unused]] std::uint32_t immediate)
    {
        return 0x05;
    }
}
