#include "generator.hpp"

void hir::generator::visit(const ast::compound_statement &compound)
{
    blocks_.push_back(make_basic_block());
    for (const ast::statement &statement : compound.statements)
    {
        visit(statement);
    }
}

void hir::generator::visit(const ast::return_statement &ret)
{
    builtin_type value = unit;

    if (ret.expression)
    {
        visit(ret.expression.value());
        value = current_value_;
    }

    // NOLINTNEXTLINE(modernize-use-emplace): `return_instruction` has to be explicitly constructed anyway.
    current_block().instructions.push_back(return_instruction { value });
}

void hir::generator::visit(const ast::unsigned_integer_literal &integer)
{
    current_value_ = integer.value;
}
