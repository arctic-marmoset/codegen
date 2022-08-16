#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <memory>
#include <variant>

namespace ast
{
    struct unsigned_integer_literal
    {
        std::uint32_t value = 0;
    };

    using constant_expression = std::variant<unsigned_integer_literal>;

    using primary_expression = std::variant<constant_expression>;

    struct additive_expression;

    using expression = std::variant<primary_expression, additive_expression>;

    enum class additive_op
    {
        add,
    };

    struct additive_expression
    {
        additive_op                 op;
        std::unique_ptr<expression> lhs;
        std::unique_ptr<expression> rhs;
    };

    constexpr const char *to_string(additive_op op)
    {
        switch (op)
        {
        case additive_op::add:
            return "+";
        default:
            return "<undefined>";
        }
    }
}

template<>
struct fmt::formatter<ast::additive_op> : fmt::formatter<const char *>
{
    template<typename FormatContext>
    auto format(ast::additive_op op, FormatContext &context) const
    {
        return fmt::format_to(context.out(), "{}", ast::to_string(op));
    }
};
