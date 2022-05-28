#pragma once

#include <xmmintrin.h>
#include <smmintrin.h>

namespace zen
{
    struct alignas(16) Vector4f final
    {
    public:
        using SimdType = __m128;

        [[nodiscard]]
        Vector4f() noexcept;

    private:
        union
        {
            SimdType _value;
            float _f32[4];
        };
    };

    inline Vector4f::Vector4f() noexcept
        : _value{ _mm_setzero_ps() }
    {
    }
}