cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(CodeGeneration)

set(COMPILE_FLAGS)
if(MSVC)
    set(COMPILE_FLAGS
        /EHsc
        /Zc:__cplusplus
        /Zc:preprocessor
        /utf-8
    )
endif()

option(USE_EXTENSIVE_WARNINGS "Enable additional compiler warnings" ON)

if(MSVC)
    string(REGEX REPLACE "[-/]W[1-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(WARN_FLAGS /W4 /permissive-)
else()
    set(WARN_FLAGS -Wall -Wextra -Wpedantic)
endif()

if(USE_EXTENSIVE_WARNINGS)
    if(MSVC)
        list(APPEND WARN_FLAGS
            /w14242 # possible loss of data: conversion from `type1` to `type2`
            /w14254 # possible loss of data: conversion from `type1:field_bits`
                    # to `type2:field_bits`
            /w14263 # member function does not override any base class virtual
                    # member function
            /w14265 # class has virtual functions, but destructor is not
                    # virtual; instances of this class may not be destructed
                    # correctly
            /w14287 # unsigned/negative constant mismatch
            /we4289 # loop control variable declared in the for-loop is used
                    # outside the for-loop scope
            /w14296 # expression is always [true | false]
            /w14311 # pointer truncation from `type1` to `type2`
            /w14545 # expression before comma evaluates to a function which is
                    # missing an argument list
            /w14546 # function call before comma missing argument list
            /w14547 # operator before comma has no effect; expected operator
                    # with side-effect
            /w14549 # operator before comma has no effect; did you intend
                    # 'operator'?
            /w14555 # expression has no effect; expected expression with
                    # side-effect
            /w14619 # pragma warning: there is no warning number `number`
            /w14640 # thread un-safe static member initialization
            /w14826 # conversion from 'type1' to 'type_2' is sign-extended which
                    # may cause unexpected runtime behavior
            /w14928 # illegal copy-initialization; more than one user-defined
                    # conversion has been implicitly applied
        )
    else()
        list(APPEND WARN_FLAGS
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Wformat=2
            -Wfloat-equal
        )
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list(APPEND WARN_FLAGS
            -Wshadow=compatible-local
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wuseless-cast
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        list(APPEND WARN_FLAGS
            -Wshadow-uncaptured-local
            -Wshorten-64-to-32
        )
    endif()
endif()

if(WIN32)
    set(ADDITIONAL_COMPILE_DEFINITIONS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()

option(USE_ADDRESS_SANITIZER "Enable AddressSanitizer runtime checks" ON)
if(CMAKE_BUILD_TYPE_UPPER STREQUAL "RELEASE")
    set(USE_ADDRESS_SANITIZER OFF)
endif()

if(WIN32 AND NOT MSVC AND USE_ADDRESS_SANITIZER)
    message(WARNING "USE_ADDRESS_SANITIZER set but not yet supported on Windows non-MSVC compilers. Disabling.")
    set(USE_ADDRESS_SANITIZER OFF)
endif()

if(USE_ADDRESS_SANITIZER)
    if(MSVC)
        # Shared CRT (/MDd) requires clang_rt.asan_* libraries to be in PATH. Setting static CRT as a workaround.
        set(MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
        set(ADDRESS_SANITIZER_FLAGS
            /fsanitize=address
        )
    else()
        set(ADDRESS_SANITIZER_FLAGS
            -fsanitize=address
        )
        set(ADDRESS_SANITIZER_LINK_FLAGS ${ADDRESS_SANITIZER_FLAGS})
    endif()
endif()

find_package(fmt CONFIG REQUIRED)

add_executable(codegen
    src/main.cpp

    src/ast/tree_printer.cpp
    src/ast/visitor.cpp
    src/cgn/generator.cpp
    src/hir/generator.cpp
    src/hir/printer.cpp
)

set_target_properties(codegen PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE
    CXX_EXTENSIONS FALSE
)

set(VS_STARTUP_PROJECT codegen)

target_include_directories(codegen
    PRIVATE
        src
)

target_compile_definitions(codegen
    PRIVATE
        ${ADDITIONAL_COMPILE_DEFINITIONS}
)

target_link_libraries(codegen
    PRIVATE
        fmt::fmt-header-only
)

target_compile_options(codegen
    PRIVATE
        ${COMPILE_FLAGS}
        ${WARN_FLAGS}
        $<$<NOT:$<CONFIG:Release>>:${ADDRESS_SANITIZER_FLAGS}>
)

target_link_options(codegen
    PRIVATE
        $<$<NOT:$<CONFIG:Release>>:${ADDRESS_SANITIZER_LINK_FLAGS}>
)

if(MSVC AND USE_ADDRESS_SANITIZER)
    set_target_properties(codegen PROPERTIES
        MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY}
    )
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
if(IPO_SUPPORTED)
    set_target_properties(codegen PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
    )
endif()
