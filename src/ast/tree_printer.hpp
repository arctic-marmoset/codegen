#pragma once

#include "ast/visitor.hpp"

#include <string>
#include <vector>

namespace ast
{
    class tree_printer final : public visitor
    {
    public:
        constexpr explicit tree_printer(std::uint32_t indent_width = 2)
            :
            indent_width_(indent_width)
        {
        }

    protected:
        action before(const statement &statement) override;
        action before(const compound_statement &compound) override;
        void after(const ast::compound_statement &compound) override;
        action before(const return_statement &ret) override;
        void after(const ast::return_statement &ret) override;

        action before(const expression &expression) override;
        action before(const ast::additive_expression &additive) override;
        void after(const ast::additive_expression &additive) override;
        action before(const unsigned_integer_literal &integer) override;

    private:
        static constexpr char space_indent = ' ';
        static constexpr char intermediate_indent = '|';
        static constexpr char middle_branch = '|';
        static constexpr char last_branch = '`';

        void add_indent(std::size_t count = 1);
        void remove_indent();

        void print_indent();

        [[nodiscard]]
        constexpr bool last_child() const
        {
            return child_counts_.back() == 0;
        }

        [[nodiscard]]
        constexpr char indent_start()
        {
            return last_child() ? space_indent : intermediate_indent;
        }

        [[nodiscard]]
        constexpr char branch_start()
        {
            return last_child() ? last_branch : middle_branch;
        }

    private:
        std::string indent_;
        std::vector<std::size_t> child_counts_;
        std::uint32_t indent_width_;
        bool root_ = true;
    };
}
