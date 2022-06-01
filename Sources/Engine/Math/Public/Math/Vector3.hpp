#pragma once
#include <Core/Misc/Assert.hpp>
#include <Core/Platform/PlatformDefine.hpp>
#include <cstdint>
#include <cstddef>
#include <cmath>

namespace std
{
    template <class ElementType, size_t Extent>
    class span;
}

namespace zen
{
    struct Vector3F final
    {
    public:
        Vector3F() noexcept;

        /**
        * @brief すべての成分をひとつの値で初期化するコンストラクタ。
        *
        * @param value すべての値をvalueで初期化
        */
        explicit Vector3F(float value) noexcept;

        /**
        * @brief 各成分をそれぞれの値で初期化するコンストラクタ。
        *
        * @param[in] x X成分
        * @param[in] y Y成分
        * @param[in] z Z成分
        */
        Vector3F(float x, float y, float z) noexcept;


        /**
        * @brief 配列のスライスによって初期化を行います。
        */
        Vector3F(std::span<const float, 3> span) noexcept;

        Vector3F(const Vector3F& other) noexcept = default;
        Vector3F& operator=(const Vector3F& other) noexcept = default;
        Vector3F(Vector3F&& other) noexcept = default;
        Vector3F& operator=(Vector3F&& other) noexcept = default;
        ~Vector3F() noexcept = default;

        [[nodiscard]] Vector3F operator-() const noexcept;
        [[nodiscard]] Vector3F operator+(const Vector3F& v) const noexcept;
        [[nodiscard]] Vector3F operator-(const Vector3F& v) const noexcept;
        [[nodiscard]] Vector3F operator*(const Vector3F& v) const noexcept;
        [[nodiscard]] Vector3F operator*(float scale) const noexcept;
        [[nodiscard]] Vector3F operator/(const Vector3F& v) const noexcept;
        [[nodiscard]] Vector3F operator/(float scale) const noexcept;

        [[nodiscard]] bool operator==(const Vector3F& v) const noexcept;
        [[nodiscard]] bool operator!=(const Vector3F& v) const noexcept;
        float& operator[](int32_t index) noexcept;
        float operator[](int32_t index) const noexcept;
        
        Vector3F& operator+=(const Vector3F& v) noexcept;
        Vector3F& operator-=(const Vector3F& v) noexcept;
        Vector3F& operator*=(const Vector3F& v) noexcept;
        Vector3F& operator/=(const Vector3F& v) noexcept;

        friend Vector3F operator*(float scale, const Vector3F& v) noexcept;

        void setX(float x) noexcept;
        void setY(float y) noexcept;
        void setZ(float z) noexcept;

        [[nodiscard]]
        float getX() const noexcept;

        [[nodiscard]]
        float getY() const noexcept;

        [[nodiscard]]
        float getZ() const noexcept;

        /**
        * @brief ベクトルの大きさを求めます。
        *
        * @return ベクトルの大きさ
        */
        [[nodiscard]] float length() const noexcept;

        /**
        * @brief ベクトルの大きさの二乗を求めます。
        *
        * @return ベクトルの大きさの二乗
        */
        [[nodiscard]] float lengthSquared() const noexcept;

        /**
        * @brief 正規化したベクトルを返します。高速化のために０除算のチェックを行いません。
        *
        * @pre ベクトルの長さが0より大きくなければいけません。
        */
        [[nodiscard]] Vector3F normalizedUnsafe() const noexcept;

        /**
        * @brief 二点間の距離を計算します。
        */
        [[nodiscard]] static float distance(const Vector3F& v1, const Vector3F& v2) noexcept;

        /**
        * @brief 二点間の距離の二乗を計算します。
        *
        * @param [in] v ベクトル
        *
        * @return 二点間の距離の二乗
        */
        [[nodiscard]] static float distanceSquared(const Vector3F& v1, const Vector3F& v2) noexcept;

        /**
        * @brief 外積を計算します。
        *
        * @param [in] v1 一つ目のベクトル
        * @param [in] v2 二つ目のベクトル
        *
        * @return 外積
        */
        [[nodiscard]] static Vector3F cross(const Vector3F& v1, const Vector3F& v2) noexcept;

        /**
        * @brief 内積を計算します。
        *
        * @param [in] v1 一つ目のベクトル
        * @param [in] v2 二つ目のベクトル
        *
        * @return 内積
        */
        [[nodiscard]] static float dot(const Vector3F& v1, const Vector3F& v2) noexcept;

        /**
        * @brief 反射ベクトルを求めます。
        *
        * @param[in] inDirection 入射ベクトル
        * @param[in] normal ミラーする対象のベクトル
        *
        * return 反射ベクトル
        */
        [[nodiscard]] static Vector3F reflect(const Vector3F& direction, const Vector3F& normal) noexcept;

        /**
        * @brief 二つのベクトルのなす角を求めます
        *
        * @param[in] v1 ベクトル1
        * @param[in] v2 ベクトル2
        *
        * @return 二つのベクトルのなす角
        */
        [[nodiscard]] static float angleBetween(const Vector3F& v1, const Vector3F& v2) noexcept;

        static const Vector3F zero;
        static const Vector3F one;

    private:
        float _x; ///< ベクトルのX成分
        float _y; ///< ベクトルのY成分
        float _z; ///< ベクトルのZ成分
    };

    ZEN_FORCEINLINE Vector3F::Vector3F() noexcept
        : Vector3F{ 0 }
    {
    }

    ZEN_FORCEINLINE Vector3F::Vector3F(const float value) noexcept
        : Vector3F{ value, value, value }
    {
    }

    ZEN_FORCEINLINE Vector3F::Vector3F(const float x, const float y, const float z) noexcept
        : _x{ x }
        , _y{ y }
        , _z{ z }
    {
    }

    ZEN_FORCEINLINE Vector3F Vector3F::operator-() const noexcept
    {
        return{ -_x, -_y, -_z };
    }

    ZEN_FORCEINLINE Vector3F Vector3F::operator+(const Vector3F& v) const noexcept
    {
        return{ _x + v._x, _y + v._y, _z + v._z };
    }

    ZEN_FORCEINLINE Vector3F Vector3F::operator-(const Vector3F& v) const noexcept
    {
        return{ _x - v._x, _y - v._y, _z - v._z };
    }

    ZEN_FORCEINLINE Vector3F Vector3F::operator*(const Vector3F& v) const noexcept
    {
        return{ _x * v._x, _y * v._y, _z * v._z };
    }

    ZEN_FORCEINLINE Vector3F Vector3F::operator*(const float scale) const noexcept
    {
        return{ _x * scale, _y * scale, _z * scale };
    }

    ZEN_FORCEINLINE Vector3F Vector3F::operator/(const Vector3F& v) const noexcept
    {
        return{ _x / v._x, _y / v._y, _z / v._z };
    }

    ZEN_FORCEINLINE Vector3F Vector3F::operator/(float scale) const noexcept
    {
        const float inverseScale{ 1.0f / scale };
        return{ _x * inverseScale, _y * inverseScale, _z * inverseScale };
    }

    ZEN_FORCEINLINE bool Vector3F::operator==(const Vector3F& v) const noexcept
    {
        return (_x == v._x && _y == v._y && _z == v._z);  // NOLINT(clang-diagnostic-float-equal)
    }

    ZEN_FORCEINLINE bool Vector3F::operator!=(const Vector3F& v) const noexcept
    {
        return (_x != v._x || _y != v._y || _z != v._z);  // NOLINT(clang-diagnostic-float-equal)
    }

    ZEN_FORCEINLINE float& Vector3F::operator[](const int32_t index) noexcept
    {
        ZEN_EXPECTS_MSG(0 <= index && index <= 2, u"IndexOutOfRange");
        if (index == 0) {
            return _x;
        }
        else if (index == 1) {
            return _y;
        }
        else {
            return _z;
        }
    }

    ZEN_FORCEINLINE float Vector3F::operator[](const int32_t index) const noexcept
    {
        ZEN_EXPECTS_MSG(0 <= index && index <= 2, u"IndexOutOfRange");
        if (index == 0) {
            return _x;
        }
        else if (index == 1) {
            return _y;
        }
        else {
            return _z;
        }
    }

    ZEN_FORCEINLINE Vector3F& Vector3F::operator+=(const Vector3F& v) noexcept
    {
        _x += v._x;
        _y += v._y;
        _z += v._z;
        return *this;
    }

    ZEN_FORCEINLINE Vector3F& Vector3F::operator-=(const Vector3F& v) noexcept
    {
        _x -= v._x;
        _y -= v._y;
        _z -= v._z;
        return *this;
    }

    ZEN_FORCEINLINE Vector3F& Vector3F::operator*=(const Vector3F& v) noexcept
    {
        _x *= v._x;
        _y *= v._y;
        _z *= v._z;
        return *this;
    }

    ZEN_FORCEINLINE Vector3F& Vector3F::operator/=(const Vector3F& v) noexcept
    {
        _x /= v._x;
        _y /= v._y;
        _z /= v._z;
        return *this;
    }

    ZEN_FORCEINLINE Vector3F operator*(const float scale, const Vector3F& v) noexcept
    {
        return{ v * scale };
    }


    ZEN_FORCEINLINE float Vector3F::getX() const noexcept
    {
        return _x;
    }

    ZEN_FORCEINLINE float Vector3F::getY() const noexcept
    {
        return _y;
    }

    ZEN_FORCEINLINE float Vector3F::getZ() const noexcept
    {
        return _z;
    }

    ZEN_FORCEINLINE void Vector3F::setX(const float x) noexcept
    {
        _x = x;
    }

    ZEN_FORCEINLINE void Vector3F::setY(const float y) noexcept
    {
        _y = y;
    }

    ZEN_FORCEINLINE void Vector3F::setZ(const float z) noexcept
    {
        _z = z;
    }

    ZEN_FORCEINLINE float Vector3F::length() const noexcept
    {
        return std::sqrt(lengthSquared());
    }

    ZEN_FORCEINLINE float Vector3F::lengthSquared() const noexcept
    {
        return _x * _x + _y * _y + _z * _z;
    }

    ZEN_FORCEINLINE Vector3F Vector3F::normalizedUnsafe() const noexcept
    {
        const float invLength{ 1.0f / length() };
        return *this * invLength;
    }

    ZEN_FORCEINLINE float Vector3F::distance(const Vector3F& v1, const Vector3F& v2) noexcept
    {
        return (v2 - v1).length();
    }

    ZEN_FORCEINLINE float Vector3F::distanceSquared(const Vector3F& v1, const Vector3F& v2) noexcept
    {
        return (v2 - v1).lengthSquared();
    }

    ZEN_FORCEINLINE Vector3F Vector3F::cross(const Vector3F& v1, const Vector3F& v2) noexcept
    {
        return
        {
            (v1._y * v2._z - v1._z * v2._y),
            (v1._z * v2._x - v1._x * v2._z),
            (v1._x * v2._y - v1._y * v2._x)
        };
    }

    ZEN_FORCEINLINE float Vector3F::dot(const Vector3F& v1, const Vector3F& v2) noexcept
    {
        return v1._x * v2._x + v1._y * v2._y + v1._z * v2._z;
    }

    ZEN_FORCEINLINE Vector3F Vector3F::reflect(const Vector3F& direction, const Vector3F& normal) noexcept
    {
        return direction - normal * Vector3F{ Vector3F::dot(normal, direction) * 2.0f };
    }

    ZEN_FORCEINLINE float Vector3F::angleBetween(const Vector3F& v1, const Vector3F& v2) noexcept
    {
        return std::acos(Vector3F::dot(v1, v2) / std::sqrt(v1.lengthSquared() * v2.lengthSquared()));
    }
}