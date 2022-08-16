#pragma once

#include "ast/visitor.hpp"
#include "hir/basic_block.hpp"

namespace hir
{
    class generator final : public ast::visitor
    {
    public:
        std::vector<basic_block> &result()
        {
            return blocks_;
        }

        using visitor::visit;

        void visit(const ast::return_statement &ret) override;
        void visit(const ast::additive_expression &additive) override;

    protected:
        action before(const ast::unsigned_integer_literal &integer) override;

    private:
        [[nodiscard]]
        basic_block &current_block()
        {
            return blocks_.back();
        }

        [[nodiscard]]
        basic_block make_basic_block() const
        {
            return {
                .id = blocks_.size(),
                .instructions = { },
            };
        }

        [[nodiscard]]
        variable make_variable()
        {
            return {
                .id = variable_count_++,
            };
        }

    private:
        std::vector<basic_block> blocks_ = { basic_block { .id = 0 } };
        expression current_expression_;
        std::uint32_t variable_count_ = 0;
    };

    inline std::vector<hir::basic_block> generate(const ast::root &ast)
    {
        generator generator;
        generator.visit(ast);
        return std::move(generator.result());
    }
}
