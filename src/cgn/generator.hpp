#pragma once

#include "cgn/x64.hpp"
#include "hir/basic_block.hpp"

#include <vector>

namespace cgn
{
    class generator
    {
    public:
        [[nodiscard]]
        std::vector<std::uint8_t> process(const std::vector<hir::basic_block> &blocks);

    private:
        void emit(const hir::basic_block &block);
        void emit(const hir::instruction &instruction);

        void emit(const hir::return_instruction &ret);

        void emit_mov(x64::reg reg, std::uint32_t immediate);

    private:
        std::vector<std::uint8_t> bytes_;
    };

    inline std::vector<std::uint8_t> generate(const std::vector<hir::basic_block> &blocks)
    {
        generator generator;
        return generator.process(blocks);
    }
}
