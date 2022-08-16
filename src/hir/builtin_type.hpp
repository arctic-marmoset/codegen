#pragma once

#include "common.hpp"

#include <fmt/core.h>

#include <string>
#include <variant>

namespace hir
{
    // The "unit" type. Also known as `void`.
    using unit_type = std::monostate;

    using uint32_type = std::uint32_t;

    using builtin_type = std::variant<unit_type, uint32_type>;

    constexpr unit_type unit;
}

template<>
struct fmt::formatter<hir::builtin_type>
{
    constexpr auto parse(format_parse_context &context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(const hir::builtin_type &value, FormatContext &context) const
    {
        return std::visit(
            match {
                [&](hir::unit_type)
                {
                    return fmt::format_to(context.out(), "()");
                },
                [&](hir::uint32_type uint32)
                {
                    return fmt::format_to(context.out(), "{}U", uint32);
                },
            },
            value
        );
    }
};
