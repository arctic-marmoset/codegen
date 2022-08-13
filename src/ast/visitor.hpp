#pragma once

#include "ast.hpp"
#include "common.hpp"

#include <type_traits>
#include <variant>

namespace ast
{
    template<typename T>
    struct is_variant : std::false_type { };

    template<typename... Args>
    struct is_variant<std::variant<Args...>> : std::true_type { };

    template<typename T>
    constexpr bool is_variant_v = is_variant<T>::value;

    template<typename T>
    concept branch = is_variant_v<T>;

    template<typename T>
    concept leaf = !is_variant_v<T>;

    template<typename Derived>
    struct visitor
    {
        constexpr void visit(const statement &statement)
        {
            static_cast<Derived *>(this)->before(statement);

            std::visit(
                match {
                    [&](const compound_statement &compound)
                    {
                        visit(compound);
                    },
                    [&](const return_statement &ret)
                    {
                        visit(ret);
                    },
                },
                statement
            );

            static_cast<Derived *>(this)->after(statement);
        }

        constexpr void visit(const compound_statement &compound)
        {
            static_cast<Derived *>(this)->before(compound);
            static_cast<Derived *>(this)->visit(compound);
            static_cast<Derived *>(this)->after(compound);
        }

        constexpr void visit(const return_statement &ret)
        {
            static_cast<Derived *>(this)->before(ret);
            static_cast<Derived *>(this)->visit(ret);
            static_cast<Derived *>(this)->after(ret);
        }

        constexpr void visit(const expression &expression)
        {
            static_cast<Derived *>(this)->before(expression);

            std::visit(
                match {
                    [&](const primary_expression &primary)
                    {
                        visit(primary);
                    },
                },
                expression
            );

            static_cast<Derived *>(this)->after(expression);
        }

        constexpr void visit(const primary_expression &primary)
        {
            static_cast<Derived *>(this)->before(primary);

            std::visit(
                match {
                    [&](const constant_expression &constant)
                    {
                        visit(constant);
                    },
                },
                primary
            );

            static_cast<Derived *>(this)->after(primary);
        }

        constexpr void visit(const constant_expression &constant)
        {
            static_cast<Derived *>(this)->before(constant);

            std::visit(
                match {
                    [&](const unsigned_integer_literal &integer)
                    {
                        visit(integer);
                    },
                },
                constant
            );

            static_cast<Derived *>(this)->after(constant);
        }

        constexpr void visit(const unsigned_integer_literal &integer)
        {
            static_cast<Derived *>(this)->before(integer);
            static_cast<Derived *>(this)->visit(integer);
            static_cast<Derived *>(this)->after(integer);
        }

        template<typename T>
        constexpr void before([[maybe_unused]] const T &node) { }

        template<typename T>
        constexpr void after([[maybe_unused]] const T &node) { }
    };
}
