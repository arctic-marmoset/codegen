#include "ast.hpp"
#include "hexdump.hpp"
#include "ast/print.hpp"
#include "cgn/generator.hpp"
#include "dyn/function.hpp"
#include "hir/generator.hpp"
#include "hir/printer.hpp"

#include <fmt/core.h>

const ast::statement tree = []
{
    std::vector<ast::statement> statements;

    // NOTE: Both push_back and emplace_back have the same effect here, but emplace_back satisfies the
    //  modernize-use-emplace clang-tidy lint.
    statements.emplace_back(
        ast::return_statement {
            ast::additive_expression {
                ast::additive_op::add,
                std::make_unique<ast::expression>(ast::unsigned_integer_literal { 0xDEADBEE5 }),
                std::make_unique<ast::expression>(ast::unsigned_integer_literal { 0x0000000A }),
            }
        }
    );

    statements.emplace_back(ast::return_statement { });
    statements.emplace_back(ast::return_statement { ast::unsigned_integer_literal { 0 } });

    return ast::compound_statement { std::move(statements) };
}();

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
