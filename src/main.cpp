#include "ast.hpp"
#include "hexdump.hpp"
#include "ast/print.hpp"
#include "cgn/generator.hpp"
#include "dyn/function.hpp"
#include "hir/generator.hpp"
#include "hir/printer.hpp"

#include <fmt/core.h>

// clang-format off
ast::statement tree = ast::compound_statement {
    {
        ast::return_statement { ast::unsigned_integer_literal { 0xDEADBEEF } },
        ast::return_statement { },
        ast::return_statement { ast::unsigned_integer_literal { 0 } },
    },
};
// clang-format on

// This is just for making sure CRTP is working.
template<typename T>
constexpr void do_visit(ast::visitor<T> &visitor, const ast::statement &statement)
{
    visitor.visit(statement);
}

int main()
{
    fmt::print("{:=^79}\n", "AST");
    ast::print(tree);

    fmt::print("{:=^79}\n", "IR");
    const std::vector<hir::basic_block> blocks = hir::generate(tree);
    hir::print(blocks);

    fmt::print("{:=^79}\n", "CODE GENERATION");
    const std::vector<std::uint8_t> bytes = cgn::generate(blocks);
    hex::dump(bytes);

    fmt::print("{:=^79}\n", "EXECUTION");
    dyn::function<std::uint32_t()> function(bytes);
    fmt::print("function() = 0x{:X}\n", function());
}
