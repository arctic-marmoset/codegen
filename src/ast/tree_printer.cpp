#include "tree_printer.hpp"

#include <fmt/core.h>

#include <iterator>

void ast::tree_printer::visit(const compound_statement &compound)
{
    fmt::print("compound_statement\n");
    visit_with_indent(compound.statements.begin(), compound.statements.end());
}

void ast::tree_printer::visit(const return_statement &ret)
{
    fmt::print("return_statement\n");
    if (ret.expression)
    {
        visit_with_indent(ret.expression.value());
    }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ast::tree_printer::visit(const unsigned_integer_literal &integer)
{
    fmt::print("integer_literal {}U\n", integer.value);
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

template<typename... Args>
void ast::tree_printer::visit_with_indent(Args &&...args)
{
    constexpr std::size_t count = sizeof...(args);
    add_indent(count);
    (visit(args), ...);
    remove_indent();
}

template<typename InputIterator>
void ast::tree_printer::visit_with_indent(InputIterator begin, InputIterator end)
{
    const auto count = static_cast<std::size_t>(end - begin);
    add_indent(count);
    for (auto it = begin; it != end; ++it)
    {
        visit(*it);
    }
    remove_indent();
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
