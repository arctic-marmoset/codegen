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
    std::visit(
        match {
            [&](const hir::return_instruction &ret)
            {
                emit(ret);
            },
        },
        instruction
    );
}

void cgn::generator::emit(const hir::return_instruction &ret)
{
    std::visit(
        match {
            [](hir::unit_type) { },
            [&](hir::uint32_type value)
            {
                emit_mov(x64::reg::rax, value);
            },
        },
        ret.value
    );

    bytes_.push_back(x64::ret);
}

void cgn::generator::emit_mov(x64::reg reg, std::uint32_t immediate)
{
    bytes_.push_back(x64::mov(reg));
    const std::array immediate_bytes = to_le_bytes(immediate);
    bytes_.insert(bytes_.end(), immediate_bytes.begin(), immediate_bytes.end());
}
