#pragma once
#include <cstdint>

namespace zen::rhi
{
    template<typename T>
    struct Handle 
    {
        constexpr Handle() noexcept = default;
        explicit Handle(uint32_t raw) noexcept
            : _raw{ raw }
        {
        }


        Handle(const Handle& other) noexcept = default;
        Handle& operator=(const Handle& other) noexcept = default;
        Handle(Handle&& other) noexcept = default;
        Handle& operator=(Handle&& other) noexcept = default;
        ~Handle() noexcept = default;

        bool operator==(const Handle& other) const noexcept
        {
            return _raw == other._raw;
        }

        bool operator!=(const Handle& other) const noexcept
        {
            return _raw != other._raw;
        }

        [[nodiscard]]
        uint32_t getRaw() const noexcept
        {
            return _raw;
        }

    private:
        uint32_t _raw = 0;
    };
}