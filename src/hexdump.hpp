#pragma once

#include "common.hpp"

#include <fmt/core.h>

#include <cstddef>
#include <cstdint>
#include <ranges>
#include <vector>

namespace hex
{
    // Simple function to dump hexadecimal bytes with address and ASCII representation.
    inline void dump(const std::vector<std::uint8_t> &bytes, std::uint32_t columns = 16)
    {
        constexpr std::size_t address_characters = 8;

        // Print column addresses, left padded with space equal in length to address label.
        fmt::print("{:<{}}", "", address_characters + 2 + 1);
        for (std::uint32_t i = 0; i < columns; ++i)
        {
            fmt::print(" {:02X}", i);
        }
        fmt::print("\n");

        // TODO (2022-08-13): Replace with C++23 std::views::chunk(columns) once available.
        for (std::size_t offset = 0; offset < bytes.size(); offset += columns)
        {
            fmt::print("0x{:0{}X}: ", offset, address_characters);

            // TODO (2022-08-13): Replace with fmt::print("{:02X}", fmt::join(chunk, ", ")) once
            //  std::views::chunk(columns) is being used.
            const char *separator = "";
            std::size_t byte_index;
            for (byte_index = 0; byte_index < columns && offset + byte_index < bytes.size(); ++byte_index)
            {
                fmt::print("{}{:02X}", separator, bytes[offset + byte_index]);
                separator = " ";
            }

            const std::size_t missing_count = columns - byte_index;
            for (std::size_t i = 0; i < missing_count; ++i)
            {
                fmt::print("   ");
            }

            fmt::print("  ");

            // TODO (2022-08-13): Replace with fmt::print("{}", fmt::join(chunk | std::views::transform(to_ascii), ""))
            //  once std::views::chunk(columns) is being used.
            for (byte_index = 0; byte_index < columns && offset + byte_index < bytes.size(); ++byte_index)
            {
                fmt::print("{}", to_ascii(bytes[offset + byte_index]));
            }

            fmt::print("\n");
        }
    }
}
