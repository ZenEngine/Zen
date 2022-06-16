#pragma once
#include <cstdint>

namespace zen::rhi
{
    template<typename T>
    struct HandleBase
    {
        static constexpr uint32_t invalidId{ static_cast<uint32_t>(-1) };

        constexpr HandleBase() noexcept
            : _id{ invalidId }
            , _generation{ 0 }
        {
        }

        explicit HandleBase(uint32_t handleId) noexcept
            : _id{ handleId }
        {
        }

        [[nodiscard]] bool isValid() const noexcept
        {
            return _id != invalidId;
        }

        void reset() noexcept
        {
            _id = invalidId;
        }


    private:
        uint32_t _id : 20; ///< オブジェクトへのID。
        uint32_t _generation : 10; ///< 再利用された際、前の参照と区別するための識別子。
    };

    template<typename T>
    struct Handle : public HandleBase<T>
    {
        Handle() noexcept = default;
        explicit Handle(uint32_t id) noexcept
            : HandleBase{ id }
        {
        }

        Handle(const Handle& other) noexcept = default;
        Handle& operator=(const Handle& other) noexcept = default;
        Handle(Handle&& other) noexcept = default;
        Handle& operator=(Handle&& other) noexcept = default;
        ~Handle() noexcept = default;
    };
}