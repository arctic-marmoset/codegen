#pragma once

#include "ast/expression.hpp"

#include <optional>
#include <variant>
#include <vector>

namespace ast
{
    struct return_statement
    {
        std::optional<ast::expression> expression;
    };

    struct compound_statement;

    using statement = std::variant<compound_statement, return_statement>;

    struct compound_statement
    {
        std::vector<statement> statements;
    };
}
