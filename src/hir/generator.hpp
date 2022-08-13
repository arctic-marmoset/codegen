#pragma once

#include "ast/visitor.hpp"
#include "hir/basic_block.hpp"

namespace hir
{
    class generator : public ast::visitor<generator>
    {
    public:
        using visitor::visit;

        // NOTE: See ast::tree_printer about the following "hides non-virtual function from base class" warnings.

        void visit(const ast::compound_statement &compound);
        void visit(const ast::return_statement &ret);
        void visit(const ast::unsigned_integer_literal &integer);

        std::vector<basic_block> &result()
        {
            return blocks_;
        }

    private:
        [[nodiscard]]
        basic_block &current_block()
        {
            return blocks_.back();
        }

        [[nodiscard]]
        basic_block make_basic_block()
        {
            return {
                .id = blocks_.size(),
                .instructions = { },
            };
        }

    private:
        std::vector<basic_block> blocks_;
        builtin_type current_value_;
    };

    inline std::vector<hir::basic_block> generate(const ast::root &ast)
    {
        generator generator;
        generator.visit(ast);
        return std::move(generator.result());
    }
}
