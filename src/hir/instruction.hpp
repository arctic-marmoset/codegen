#pragma once

#include "hir/builtin_type.hpp"

#include <variant>

namespace hir
{
    enum class operation
    {
        add,
        ret,
    };

    struct variable
    {
        std::uint32_t id;
    };

    using expression = std::variant<builtin_type, variable>;

    struct instruction
    {
        operation  op;
        variable   destination;
        expression lhs;
        expression rhs;
    };

    constexpr const char *to_string(operation op)
    {
        switch (op)
        {
        case operation::add:
            return "add";
        case operation::ret:
            return "return";
        default:
            return "<undefined>";
        }
    }
}

template<>
struct fmt::formatter<hir::operation> : fmt::formatter<const char *>
{
    template<typename FormatContext>
    auto format(hir::operation op, FormatContext &context) const
    {
        return fmt::format_to(context.out(), "{}", hir::to_string(op));
    }
};

template<>
struct fmt::formatter<hir::variable>
{
    constexpr auto parse(format_parse_context &context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(const hir::variable &value, FormatContext &context) const
    {
        return fmt::format_to(context.out(), "${}", value.id);
    }
};

template<>
struct fmt::formatter<hir::expression>
{
    constexpr auto parse(format_parse_context &context)
    {
        return context.begin();
    }

    template<typename FormatContext>
    auto format(const hir::expression &value, FormatContext &context) const
    {
        return std::visit(
            match {
                [&](const hir::variable &variable)
                {
                    return fmt::format_to(context.out(), "{}", variable);
                },
                [&](const hir::builtin_type &value)
                {
                    return fmt::format_to(context.out(), "{}", value);
                },
            },
            value
        );
    }
};
