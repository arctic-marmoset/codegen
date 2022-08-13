#pragma once

#include "ast.hpp"
#include "ast/tree_printer.hpp"

namespace ast
{
    enum class print_format
    {
        tree,
    };

    inline void print(const ast::root &ast, print_format format = print_format::tree)
    {
        switch (format)
        {
        case print_format::tree:
            {
                tree_printer printer;
                printer.visit(ast);
            }
            break;
        }
    }
}
