#pragma once

#include <cstdint>
#include <variant>

namespace ast
{
    struct unsigned_integer_literal
    {
        std::uint32_t value = 0;
    };

    using constant_expression = std::variant<unsigned_integer_literal>;

    using primary_expression = std::variant<constant_expression>;

    using expression = std::variant<primary_expression>;
}
