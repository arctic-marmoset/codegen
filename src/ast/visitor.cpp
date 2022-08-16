#include "visitor.hpp"

#include "common.hpp"

#include <variant>

void ast::visitor::visit(const ast::statement &statement)
{
    if (before(statement) == action::cancel) return;

    std::visit(
        match {
            [&](const compound_statement &compound)
            {
                visit(compound);
            },
            [&](const return_statement &ret)
            {
                visit(ret);
            },
        },
        statement
    );

    after(statement);
}

void ast::visitor::visit(const ast::compound_statement &compound)
{
    if (before(compound) == action::cancel) return;

    for (const statement &statement : compound.statements)
    {
        visit(statement);
    }

    after(compound);
}

void ast::visitor::visit(const ast::return_statement &ret)
{
    if (before(ret) == action::cancel) return;

    if (ret.expression)
    {
        visit(ret.expression.value());
    }

    after(ret);
}

void ast::visitor::visit(const ast::expression &expression)
{
    if (before(expression) == action::cancel) return;

    std::visit(
        match {
            [&](const additive_expression &additive)
            {
                visit(additive);
            },
            [&](const primary_expression &primary)
            {
                visit(primary);
            },
        },
        expression
    );

    after(expression);
}

void ast::visitor::visit(const ast::additive_expression &additive)
{
    if (before(additive) == action::cancel) return;

    visit(*additive.lhs);
    visit(*additive.rhs);

    after(additive);
}

void ast::visitor::visit(const ast::primary_expression &primary)
{
    if (before(primary) == action::cancel) return;

    std::visit(
        match {
            [&](const constant_expression &constant)
            {
                visit(constant);
            },
        },
        primary
    );

    after(primary);
}

void ast::visitor::visit(const ast::constant_expression &constant)
{
    if (before(constant) == action::cancel) return;

    std::visit(
        match {
            [&](const unsigned_integer_literal &integer)
            {
                visit(integer);
            },
        },
        constant
    );

    after(constant);
}

void ast::visitor::visit(const ast::unsigned_integer_literal &integer)
{
    if (before(integer) == action::cancel) return;
    after(integer);
}

ast::visitor::~visitor() = default;
