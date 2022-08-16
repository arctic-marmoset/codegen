#include "printer.hpp"

#include <fmt/core.h>

void hir::printer::print(const std::vector<basic_block> &blocks)
{
    for (const basic_block &block : blocks)
    {
        print(block);
    }
}

void hir::printer::print(const basic_block &block)
{
    fmt::print("{:04X} basic_block {{\n", block.id);
    for (const instruction &instruction : block.instructions)
    {
        print(instruction);
    }
    fmt::print("}}\n");
}

void hir::printer::print(const instruction &instruction)
{
    print_indent();

    switch (instruction.op)
    {
    case operation::add:
        fmt::print("{} = {} {}, {}\n", instruction.destination, instruction.op, instruction.lhs, instruction.rhs);
        break;
    case operation::ret:
        fmt::print("return {}\n", instruction.lhs);
        break;
    }
}

void hir::printer::print_indent()
{
    fmt::print("{:>{}}", "", indent_width_);
}
