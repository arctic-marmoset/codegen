#include "tree_printer.hpp"

#include <fmt/core.h>

#include <iterator>

ast::visitor::action ast::tree_printer::before([[maybe_unused]] const ast::statement &statement)
{
    print_indent();
    return action::proceed;
}

ast::visitor::action ast::tree_printer::before(const ast::compound_statement &compound)
{
    fmt::print("compound_statement\n");
    add_indent(compound.statements.size());
    return action::proceed;
}

void ast::tree_printer::after([[maybe_unused]] const ast::compound_statement &compound)
{
    remove_indent();
}

ast::visitor::action ast::tree_printer::before(const ast::return_statement &ret)
{
    fmt::print("return_statement\n");

    if (ret.expression)
    {
        add_indent();
    }

    return action::proceed;
}

void ast::tree_printer::after(const ast::return_statement &ret)
{
    if (ret.expression)
    {
        remove_indent();
    }
}

ast::visitor::action ast::tree_printer::before([[maybe_unused]] const ast::expression &expression)
{
    print_indent();
    return action::proceed;
}

ast::visitor::action ast::tree_printer::before(const ast::additive_expression &additive)
{
    fmt::print("additive_expression '{}'\n", additive.op);
    add_indent(2);
    return action::proceed;
}

void ast::tree_printer::after([[maybe_unused]] const ast::additive_expression &additive)
{
    remove_indent();
}

ast::visitor::action ast::tree_printer::before(const ast::unsigned_integer_literal &integer)
{
    fmt::print("integer_literal {0}U # 0x{0:X}\n", integer.value);
    return action::proceed;
}

void ast::tree_printer::add_indent(std::size_t count)
{
    if (!root_)
    {
        fmt::format_to(
            std::back_inserter(indent_),
            "{}{:<{}}",
            indent_start(),
            "",
            indent_width_ - 1
        );
    }

    root_ = false;
    child_counts_.push_back(count);
}

void ast::tree_printer::remove_indent()
{
    if (!indent_.empty())
    {
        indent_.resize(indent_.size() - indent_width_);
    }

    child_counts_.pop_back();
}

void ast::tree_printer::print_indent()
{
    fmt::print("{}", indent_);
    if (!root_)
    {
        --child_counts_.back();
        fmt::print(
            "{}{:-<{}}",
            branch_start(),
            "",
            indent_width_ - 1
        );
    }
}
