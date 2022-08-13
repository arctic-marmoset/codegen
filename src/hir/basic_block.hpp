#pragma once

#include "hir/instruction.hpp"

#include <vector>

namespace hir
{
    struct basic_block
    {
        std::size_t              id;
        std::vector<instruction> instructions;
    };
}
