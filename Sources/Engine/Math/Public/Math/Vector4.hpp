#pragma once
#include <Core/Platform/PlatformDefine.hpp>
#include <xmmintrin.h>
#include <smmintrin.h>
#include <cstdint>
#include <cstddef>


namespace zen
{
    template <class ElementType, size_t Extent>
    class TSpan;

    struct Vector3;
    struct alignas(16) Vector4 final
    {
    public:
        using simd_type = __m128;

        [[nodiscard]]
        Vector4() noexcept;

        /**
        * @brief すべての成分をひとつの値で初期化するコンストラクタ。
        *
        * @param[in] value すべての値をvalueで初期化
        */
        explicit Vector4(float value) noexcept;

        explicit Vector4(simd_type value) noexcept;

        /**
        * @brief 各成分をそれぞれの値で初期化するコンストラクタ。
        *
        * @param[in] x X成分
        * @param[in] y Y成分
        * @param[in] z Z成分
        * @param[in] w W成分
        */
        Vector4(float x, float y, float z, float w) noexcept;

        Vector4(TSpan<const float, 4> span) noexcept;

        Vector4(const Vector4& other) noexcept = default;
        Vector4& operator=(const Vector4& other) noexcept = default;
        Vector4(Vector4&& other) noexcept = default;
        Vector4& operator=(Vector4&& other) noexcept = default;
        ~Vector4() noexcept = default;

        [[nodiscard]] Vector4 operator-() const noexcept;
        [[nodiscard]] Vector4 operator+(const Vector4& v) const noexcept;
        [[nodiscard]] Vector4 operator-(const Vector4& v) const noexcept;
        [[nodiscard]] Vector4 operator*(const Vector4& v) const noexcept;
        [[nodiscard]] Vector4 operator*(float scale) const noexcept;
        [[nodiscard]] Vector4 operator/(const Vector4& v) const noexcept;
        [[nodiscard]] Vector4 operator/(float scale) const noexcept;

        [[nodiscard]] bool operator==(const Vector4& v) const noexcept;
        [[nodiscard]] bool operator!=(const Vector4& v) const noexcept;
        float& operator[](int32_t index) noexcept;
        float operator[](int32_t index) const noexcept;

        Vector4& operator+=(const Vector4& v) noexcept;
        Vector4& operator-=(const Vector4& v) noexcept;
        Vector4& operator*=(const Vector4& v) noexcept;
        Vector4& operator/=(const Vector4& v) noexcept;

        friend Vector4 operator*(float scale, const Vector4& v) noexcept;

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
        static float dot(const Vector4& v1, const Vector4& v2) noexcept;

        static const Vector4 zero;
        static const Vector4 one;

    private:
        union
        {
            simd_type _value;
            float _f32[4];
        };
    };

    ZEN_FORCEINLINE Vector4::Vector4() noexcept
        : _value{ _mm_setzero_ps() }
    {
    }

    ZEN_FORCEINLINE Vector4::Vector4(const float value) noexcept
        : Vector4{ value, value, value, value }
    {
    }

    ZEN_FORCEINLINE Vector4::Vector4(Vector4::simd_type value) noexcept
        : _value{ value }
    {
    }

    ZEN_FORCEINLINE Vector4::Vector4(const float x, const float y, const float z, const float w) noexcept
        : _value{ _mm_set_ps(w, z, y, x) }
    {
    }

    ZEN_FORCEINLINE Vector4 Vector4::operator-() const noexcept
    {
        return Vector4{ _mm_sub_ps(_mm_setzero_ps(), _value) };
    }

    ZEN_FORCEINLINE Vector4 Vector4::operator+(const Vector4& v) const noexcept
    {
        return Vector4{ _mm_add_ps(_value, v._value) };
    }

    ZEN_FORCEINLINE Vector4 Vector4::operator-(const Vector4& v) const noexcept
    {
        return Vector4{ _mm_sub_ps(_value, v._value) };
    }

    ZEN_FORCEINLINE Vector4 Vector4::operator*(const Vector4& v) const noexcept
    {
        return Vector4{ _mm_mul_ps(_value, v._value) };
    }

    ZEN_FORCEINLINE Vector4 Vector4::operator*(const float scale) const noexcept
    {
        return Vector4{ _mm_mul_ps(_value, _mm_set1_ps(scale)) };
    }

    ZEN_FORCEINLINE Vector4 Vector4::operator/(const Vector4& v) const noexcept
    {
        return Vector4{ _mm_div_ps(_value, v._value) };
    }

    ZEN_FORCEINLINE Vector4 Vector4::operator/(const float scale) const noexcept
    {
        return Vector4{ _mm_div_ps(_value, _mm_set1_ps(scale)) };
    }

    ZEN_FORCEINLINE bool Vector4::operator==(const Vector4& v) const noexcept
    {
        return (_mm_movemask_ps(_mm_cmpeq_ps(_value, v._value)) == 0);
    }

    ZEN_FORCEINLINE bool Vector4::operator!=(const Vector4& v) const noexcept
    {
        return (_mm_movemask_ps(_mm_cmpneq_ps(_value, v._value)) == 0);
    }

    ZEN_FORCEINLINE float& Vector4::operator[](const int32_t index) noexcept
    {
        ZEN_EXPECTS_MSG(0 <= index && index <= 3, u"IndexOutOfRange");
        return _f32[index];
    }

    ZEN_FORCEINLINE float Vector4::operator[](const int32_t index) const noexcept
    {
        ZEN_EXPECTS_MSG(0 <= index && index <= 3, u"IndexOutOfRange");
        return _f32[index];
    }

    ZEN_FORCEINLINE Vector4& Vector4::operator+=(const Vector4& v) noexcept
    {
        _value = _mm_add_ps(_value, v._value);
        return *this;
    }

    ZEN_FORCEINLINE Vector4& Vector4::operator-=(const Vector4& v) noexcept
    {
        _value = _mm_sub_ps(_value, v._value);
        return *this;
    }

    ZEN_FORCEINLINE Vector4& Vector4::operator*=(const Vector4& v) noexcept
    {
        _value = _mm_mul_ps(_value, v._value);
        return *this;
    }

    ZEN_FORCEINLINE Vector4& Vector4::operator/=(const Vector4& v) noexcept
    {
        _value = _mm_div_ps(_value, v._value);
        return *this;
    }

    ZEN_FORCEINLINE Vector4 operator*(const float scale, const Vector4& v) noexcept
    {
        return{ v * scale };
    }

    ZEN_FORCEINLINE float Vector4::getX() const noexcept
    {
        return _mm_cvtss_f32(_value);
    }

    ZEN_FORCEINLINE float Vector4::getY() const noexcept
    {
        return _f32[1];
    }

    ZEN_FORCEINLINE float Vector4::getZ() const noexcept
    {
        return _f32[2];
    }

    ZEN_FORCEINLINE float Vector4::getW() const noexcept
    {
        return _f32[3];
    }

    ZEN_FORCEINLINE void Vector4::setX(const float x) noexcept
    {
        _f32[0] = x;
    }

    ZEN_FORCEINLINE void Vector4::setY(const float y) noexcept
    {
        _f32[1] = y;
    }

    ZEN_FORCEINLINE void Vector4::setZ(const float z) noexcept
    {
        _f32[2] = z;
    }

    ZEN_FORCEINLINE void Vector4::setW(const float w) noexcept
    {
        _f32[3] = w;
    }

    ZEN_FORCEINLINE float Vector4::dot(const Vector4& v1, const Vector4& v2) noexcept
    {
        return _mm_cvtss_f32(_mm_dp_ps(v1._value, v2._value, 0xff));
    }
}