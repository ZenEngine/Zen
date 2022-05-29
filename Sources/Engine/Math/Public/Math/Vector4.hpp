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

        /**
        * @brief すべての成分をひとつの値で初期化するコンストラクタ。
        *
        * @param[in] value すべての値をvalueで初期化
        */
        explicit Vector4f(float value) noexcept;

        /**
        * @brief 各成分をそれぞれの値で初期化するコンストラクタ。
        *
        * @param[in] x X成分
        * @param[in] y Y成分
        * @param[in] z Z成分
        * @param[in] w W成分
        */
        Vector4f(float x, float y, float z, float w) noexcept;


        Vector4f(const Vector4f& other) noexcept = default;
        Vector4f& operator=(const Vector4f& other) noexcept = default;
        Vector4f(Vector4f&& other) noexcept = default;
        Vector4f& operator=(Vector4f&& other) noexcept = default;
        ~Vector4f() noexcept = default;

        [[nodiscard]] Vector4f operator-() const noexcept;
        [[nodiscard]] Vector4f operator+(const Vector4f& v) const noexcept;
        [[nodiscard]] Vector4f operator-(const Vector4f& v) const noexcept;
        [[nodiscard]] Vector4f operator*(const Vector4f& v) const noexcept;
        [[nodiscard]] Vector4f operator*(float scale) const noexcept;
        [[nodiscard]] Vector4f operator/(const Vector4f& v) const noexcept;
        [[nodiscard]] Vector4f operator/(float scale) const noexcept;

        [[nodiscard]] bool operator==(const Vector4f& v) const noexcept;
        [[nodiscard]] bool operator!=(const Vector4f& v) const noexcept;
        float& operator[](int32_t index) noexcept;
        float operator[](int32_t index) const noexcept;

        Vector4f& operator+=(const Vector4f& v) noexcept;
        Vector4f& operator-=(const Vector4f& v) noexcept;
        Vector4f& operator*=(const Vector4f& v) noexcept;
        Vector4f& operator/=(const Vector4f& v) noexcept;

        friend Vector4f operator*(float scale, const Vector4f& v) noexcept;

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

    ZEN_FORCEINLINE Vector4f::Vector4f(const float value) noexcept
        : Vector4f{ value, value, value, value }
    {
    }

    ZEN_FORCEINLINE Vector4f::Vector4f(const float x, const float y, const float z, const float w) noexcept
        : _value{ _mm_set_ps(w, z, y, x) }
    {
    }

    ZEN_FORCEINLINE Vector4f Vector4f::operator-() const noexcept
    {
        return Vector4f{ _mm_sub_ps(_mm_setzero_ps(), _value) };
    }

    ZEN_FORCEINLINE Vector4f Vector4f::operator+(const Vector4f& v) const noexcept
    {
        return Vector4f{ _mm_add_ps(_value, v._value) };
    }

    ZEN_FORCEINLINE Vector4f Vector4f::operator-(const Vector4f& v) const noexcept
    {
        return Vector4f{ _mm_sub_ps(_value, v._value) };
    }

    ZEN_FORCEINLINE Vector4f Vector4f::operator*(const Vector4f& v) const noexcept
    {
        return Vector4f{ _mm_mul_ps(_value, v._value) };
    }

    ZEN_FORCEINLINE Vector4f Vector4f::operator*(const float scale) const noexcept
    {
        return Vector4f{ _mm_mul_ps(_value, _mm_set1_ps(scale)) };
    }

    ZEN_FORCEINLINE Vector4f Vector4f::operator/(const Vector4f& v) const noexcept
    {
        return Vector4f{ _mm_div_ps(_value, v._value) };
    }

    ZEN_FORCEINLINE Vector4f Vector4f::operator/(const float scale) const noexcept
    {
        return Vector4f{ _mm_div_ps(_value, _mm_set1_ps(scale)) };
    }

    ZEN_FORCEINLINE bool Vector4f::operator==(const Vector4f& v) const noexcept
    {
        return (_mm_movemask_ps(_mm_cmpeq_ps(_value, v._value)) == 0);
    }

    ZEN_FORCEINLINE bool Vector4f::operator!=(const Vector4f& v) const noexcept
    {
        return (_mm_movemask_ps(_mm_cmpneq_ps(_value, v._value)) == 0);
    }

    ZEN_FORCEINLINE float& Vector4f::operator[](const int32_t index) noexcept
    {
        ZEN_EXPECTS_MSG(0 <= index && index <= 3, u"IndexOutOfRange");
        return _f32[index];
    }

    ZEN_FORCEINLINE float Vector4f::operator[](const int32_t index) const noexcept
    {
        ZEN_EXPECTS_MSG(0 <= index && index <= 3, u"IndexOutOfRange");
        return _f32[index];
    }

    ZEN_FORCEINLINE Vector4f& Vector4f::operator+=(const Vector4f& v) noexcept
    {
        _value = _mm_add_ps(_value, v._value);
        return *this;
    }

    ZEN_FORCEINLINE Vector4f& Vector4f::operator-=(const Vector4f& v) noexcept
    {
        _value = _mm_sub_ps(_value, v._value);
        return *this;
    }

    ZEN_FORCEINLINE Vector4f& Vector4f::operator*=(const Vector4f& v) noexcept
    {
        _value = _mm_mul_ps(_value, v._value);
        return *this;
    }

    ZEN_FORCEINLINE Vector4f& Vector4f::operator/=(const Vector4f& v) noexcept
    {
        _value = _mm_div_ps(_value, v._value);
        return *this;
    }

    ZEN_FORCEINLINE Vector4f operator*(const float scale, const Vector4f& v) noexcept
    {
        return{ v * scale };
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