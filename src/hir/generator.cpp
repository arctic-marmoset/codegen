#include "generator.hpp"

void hir::generator::visit(const ast::return_statement &ret)
{
    expression expression = unit;

    if (ret.expression)
    {
        visit(ret.expression.value());
        expression = current_expression_;
    }

    // NOLINTNEXTLINE(modernize-use-emplace): `return_instruction` has to be explicitly constructed anyway.
    current_block().instructions.push_back(
        {
            .op = operation::ret,
            .lhs = expression,
        }
    );
}

void hir::generator::visit(const ast::additive_expression &additive)
{
    variable *parent = std::get_if<variable>(&current_expression_);

    visit(*additive.lhs);
    expression lhs = current_expression_;
    visit(*additive.rhs);
    expression rhs = current_expression_;

    variable destination = parent ? *parent : make_variable();

    current_block().instructions.push_back(
        {
            .op = operation::add,
            .destination = destination,
            .lhs = lhs,
            .rhs = rhs,
        }
    );

    current_expression_ = destination;
}

ast::visitor::action hir::generator::before(const ast::unsigned_integer_literal &integer)
{
    current_expression_ = integer.value;
    return action::proceed;
}
