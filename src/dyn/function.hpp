#pragma once

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/mman.h>
#else
#error Unsupported platform.
#endif

#include <cassert>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dyn
{
    namespace detail
    {
        inline void *allocate(std::size_t size)
        {
#if defined(_WIN32)
            return VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
#elif defined(__linux__)
            const long sc_pagesize = sysconf(_SC_PAGESIZE);
            assert(sc_pagesize > 0);
            const auto page_size = static_cast<std::size_t>(sc_pagesize);
            assert("TODO: Handle sizes larger than _SC_PAGESIZE" && size < page_size);
            return mmap(
                nullptr,
                page_size,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1,
                0
            );
#endif
        }

        inline void deallocate(void *address)
        {
#if defined(_WIN32)
            VirtualFree(address, 0, MEM_RELEASE);
#elif defined(__linux__)
            munmap(address, static_cast<std::size_t>(sysconf(_SC_PAGESIZE)));
#endif
        }

        inline bool try_make_executable(void *address, std::size_t size)
        {
#if defined(_WIN32)
            DWORD protect;
            return VirtualProtect(address, size, PAGE_EXECUTE_READ, &protect);
#elif defined(__linux__)
            return mprotect(address, static_cast<std::size_t>(sysconf(_SC_PAGESIZE)), PROT_READ | PROT_EXEC) == 0;
#endif
        }
    }

    template<typename>
    struct function;

    template<typename Result, typename... Args>
    struct function<Result(Args...)>
    {
    public:
        using type = Result(Args...);

        explicit function(const std::vector<std::uint8_t> &bytes)
            :
            buffer_(detail::allocate(bytes.size()))
        {
            if (!buffer_)
            {
                throw std::runtime_error("failed to allocate memory for dynamic function");
            }

            std::memcpy(buffer_, bytes.data(), bytes.size());

            if (!detail::try_make_executable(buffer_, bytes.size()))
            {
                throw std::runtime_error("failed to change protection of dynamic function memory from rw to rx");
            }
        }

        ~function()
        {
            detail::deallocate(buffer_);
        }

        Result operator()(Args... args)
        {
            return pointer()(std::forward<Args>(args)...);
        }

        type *pointer() const
        {
            return reinterpret_cast<type *>(buffer_);
        }

    private:
        void *buffer_;
    };
}
