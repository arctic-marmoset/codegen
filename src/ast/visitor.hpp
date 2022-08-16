#pragma once

#include "ast.hpp"

namespace ast
{
    struct visitor
    {
        virtual void visit(const statement &statement);
        virtual void visit(const compound_statement &compound);
        virtual void visit(const return_statement &ret);

        virtual void visit(const expression &expression);
        virtual void visit(const additive_expression &additive);
        virtual void visit(const primary_expression &primary);
        virtual void visit(const constant_expression &constant);
        virtual void visit(const unsigned_integer_literal &integer);

        virtual ~visitor() = 0;

    protected:
        enum class action
        {
            proceed,
            cancel,
        };

        virtual action before([[maybe_unused]] const statement &statement) { return action::proceed; }
        virtual void after([[maybe_unused]] const statement &statement) { }
        virtual action before([[maybe_unused]] const compound_statement &compound) { return action::proceed; }
        virtual void after([[maybe_unused]] const compound_statement &compound) { }
        virtual action before([[maybe_unused]] const return_statement &ret) { return action::proceed; }
        virtual void after([[maybe_unused]] const return_statement &ret) { }

        virtual action before([[maybe_unused]] const expression &expression) { return action::proceed; }
        virtual void after([[maybe_unused]] const expression &expression) { }
        virtual action before([[maybe_unused]] const additive_expression &additive) { return action::proceed; }
        virtual void after([[maybe_unused]] const additive_expression &additive) { }
        virtual action before([[maybe_unused]] const primary_expression &primary) { return action::proceed; }
        virtual void after([[maybe_unused]] const primary_expression &primary) { }
        virtual action before([[maybe_unused]] const constant_expression &constant) { return action::proceed; }
        virtual void after([[maybe_unused]] const constant_expression &constant) { }
        virtual action before([[maybe_unused]] const unsigned_integer_literal &integer) { return action::proceed; }
        virtual void after([[maybe_unused]] const unsigned_integer_literal &integer) { }
    };
}
