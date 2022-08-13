#pragma once

#include "ast/visitor.hpp"

#include <string>
#include <vector>

namespace ast
{
    class tree_printer : public visitor<tree_printer>
    {
    public:
        constexpr explicit tree_printer(std::uint32_t indent_width = 2)
            :
            indent_width_(indent_width)
        {
        }

        using visitor::visit;

        // The following public functions cause a "hides non-virtual function from base class" warning. This can be
        // ignored; CRTP is commonly implemented with the pattern of hiding base class functions. Renaming them would
        // also work, but would make the interface of "visitor"-type classes nonuniform.

        void visit(const compound_statement &compound);
        void visit(const return_statement &ret);
        void visit(const unsigned_integer_literal &integer);

        template<leaf T>
        void before([[maybe_unused]] const T &node)
        {
            print_indent();
        }

        // This has to be re-declared because `before<T: leaf>()` is overloaded above.
        template<typename T>
        constexpr void before([[maybe_unused]] const T &node)
        {
            visitor::before(node);
        }

    private:
        static constexpr char space_indent = ' ';
        static constexpr char intermediate_indent = '|';
        static constexpr char middle_branch = '|';
        static constexpr char last_branch = '`';

        template<typename... Args>
        void visit_with_indent(Args &&...args);

        template<typename InputIterator>
        void visit_with_indent(InputIterator begin, InputIterator end);

        void add_indent(std::size_t count);
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
