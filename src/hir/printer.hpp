#pragma once

#include "hir/basic_block.hpp"

#include <cstdint>
#include <vector>

namespace hir
{
    // TODO (2022-08-13): This class and cgn::generator are very similar in structure. They could be derived from
    //  another `visitor` class for HIR.

    class printer
    {
    public:
        constexpr explicit printer(std::uint32_t indent_width = 4)
            :
            indent_width_(indent_width)
        {
        }

        void print(const std::vector<basic_block> &blocks);

    private:
        void print(const basic_block &block);
        void print(const instruction &instruction);

        void print_indent();

    private:
        std::uint32_t indent_width_;
    };

    inline void print(const std::vector<basic_block> &blocks)
    {
        printer printer;
        printer.print(blocks);
    }
}
