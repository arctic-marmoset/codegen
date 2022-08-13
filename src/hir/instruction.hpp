#pragma once

#include "hir/builtin_type.hpp"

#include <variant>

namespace hir
{
    struct return_instruction
    {
        builtin_type value;
    };

    using instruction = std::variant<return_instruction>;
}
