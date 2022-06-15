#pragma once
#include <cstdint>

namespace zen::rhi
{
    template<typename T>
    struct HandleBase
    {
        static constexpr uint32_t invalidHandleId{ static_cast<uint32_t>(-1) };

        constexpr HandleBase() noexcept
            : _handle{ invalidHandleId }
        {
        }

        [[nodiscard]] bool isValid() const noexcept
        {
            return _handle != invalidHandleId;
        }

        void reset() noexcept
        {
            _handle = invalidHandleId;
        }


    private:
        uint32_t _handle;
    };

    template<typename T>
    struct Handle : public HandleBase<T>
    {
        Handle() noexcept = default;
    };
}