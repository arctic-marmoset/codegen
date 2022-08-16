#include "generator.hpp"

std::vector<std::uint8_t> cgn::generator::process(const std::vector<hir::basic_block> &blocks)
{
    for (const hir::basic_block &block : blocks)
    {
        emit(block);
    }

    return std::move(bytes_);
}

void cgn::generator::emit(const hir::basic_block &block)
{
    for (const hir::instruction &instruction : block.instructions)
    {
        emit(instruction);
    }
}

void cgn::generator::emit(const hir::instruction &instruction)
{
    switch (instruction.op)
    {
    case hir::operation::add:
        {
            auto lhs = std::get<hir::uint32_type>(std::get<hir::builtin_type>(instruction.lhs));
            auto rhs = std::get<hir::uint32_type>(std::get<hir::builtin_type>(instruction.rhs));

            emit_mov(x64::reg::rax, lhs);
            emit_add(x64::reg::rax, rhs);
        }
        break;
    case hir::operation::ret:
        {
            std::visit(
                match {
                    [&](const hir::variable &)
                    {
                    },
                    [&](const hir::builtin_type &value)
                    {
                        auto *immediate = std::get_if<hir::uint32_type>(&value);

                        if (immediate)
                        {
                            emit_mov(x64::reg::rax, *immediate);
                        }
                    },
                },
                instruction.lhs
            );

            bytes_.push_back(x64::ret);
        }
        break;
    }
}

void cgn::generator::emit_mov(x64::reg reg, std::uint32_t immediate)
{
    bytes_.push_back(x64::mov(reg, immediate));
    const auto immediate_bytes = to_le_bytes(immediate);
    bytes_.insert(bytes_.end(), immediate_bytes.begin(), immediate_bytes.end());
}

void cgn::generator::emit_add(x64::reg reg, std::uint32_t immediate)
{
    bytes_.push_back(x64::add(reg, immediate));
    const auto immediate_bytes = to_le_bytes(immediate);
    bytes_.insert(bytes_.end(), immediate_bytes.begin(), immediate_bytes.end());
}
