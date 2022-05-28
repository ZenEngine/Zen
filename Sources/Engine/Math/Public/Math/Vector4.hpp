#pragma once
#include <Core/Platform/PlatformDefine.hpp>
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

        [[nodiscard]]
        float getX() const noexcept;

        [[nodiscard]]
        float getY() const noexcept;

        [[nodiscard]]
        float getZ() const noexcept;

        [[nodiscard]]
        float getW() const noexcept;

        void setX(float x) noexcept;
        void setY(float y) noexcept;
        void setZ(float z) noexcept;
        void setW(float w) noexcept;

        /**
        * @brief 内積を計算します。
        *
        * @param [in] v1 一つ目のベクトル
        * @param [in] v2 二つ目のベクトル
        *
        * @return 内積
        */
        [[nodiscard]]
        static float dot(const Vector4f& v1, const Vector4f& v2) noexcept;

    private:
        union
        {
            SimdType _value;
            float _f32[4];
        };
    };

    ZEN_FORCEINLINE Vector4f::Vector4f() noexcept
        : _value{ _mm_setzero_ps() }
    {
    }

    ZEN_FORCEINLINE float Vector4f::getX() const noexcept
    {
        return _mm_cvtss_f32(_value);
    }

    ZEN_FORCEINLINE float Vector4f::getY() const noexcept
    {
        return _f32[1];
    }

    ZEN_FORCEINLINE float Vector4f::getZ() const noexcept
    {
        return _f32[2];
    }

    ZEN_FORCEINLINE float Vector4f::getW() const noexcept
    {
        return _f32[3];
    }

    ZEN_FORCEINLINE void Vector4f::setX(const float x) noexcept
    {
        _f32[0] = x;
    }

    ZEN_FORCEINLINE void Vector4f::setY(const float y) noexcept
    {
        _f32[1] = y;
    }

    ZEN_FORCEINLINE void Vector4f::setZ(const float z) noexcept
    {
        _f32[2] = z;
    }

    ZEN_FORCEINLINE void Vector4f::setW(const float w) noexcept
    {
        _f32[3] = w;
    }

    ZEN_FORCEINLINE float Vector4f::dot(const Vector4f& v1, const Vector4f& v2) noexcept
    {
        return _mm_cvtss_f32(_mm_dp_ps(v1._value, v2._value, 0xff));
    }
}