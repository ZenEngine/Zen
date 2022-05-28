#pragma once
#include <cstdint>

namespace zen
{
    struct Vector3f final
    {
    public:
        Vector3f() noexcept;

        /**
        * @brief すべての成分をひとつの値で初期化するコンストラクタ。
        *
        * @param value すべての値をvalueで初期化
        */
        explicit Vector3f(float value) noexcept;

        /**
        * @brief 各成分をそれぞれの値で初期化するコンストラクタ。
        *
        * @param[in] x X成分
        * @param[in] y Y成分
        * @param[in] z Z成分
        */
        Vector3f(float x, float y, float z) noexcept;

        Vector3f(const Vector3f& other) noexcept = default;
        Vector3f& operator=(const Vector3f& other) noexcept = default;
        Vector3f(Vector3f&& other) noexcept = default;
        Vector3f& operator=(Vector3f&& other) noexcept = default;
        ~Vector3f() noexcept = default;

        [[nodiscard]] Vector3f operator-() const noexcept;
        [[nodiscard]] Vector3f operator+(const Vector3f& v) const noexcept;
        [[nodiscard]] Vector3f operator-(const Vector3f& v) const noexcept;
        [[nodiscard]] Vector3f operator*(const Vector3f& v) const noexcept;
        [[nodiscard]] Vector3f operator*(float scale) const noexcept;
        [[nodiscard]] Vector3f operator/(const Vector3f& v) const noexcept;
        [[nodiscard]] Vector3f operator/(float scale) const noexcept;

        [[nodiscard]] bool operator==(const Vector3f& v) const noexcept;
        [[nodiscard]] bool operator!=(const Vector3f& v) const noexcept;
        float& operator[](int32_t index) noexcept;
        float operator[](int32_t index) const noexcept;
        
        Vector3f& operator+=(const Vector3f& v) noexcept;
        Vector3f& operator-=(const Vector3f& v) noexcept;
        Vector3f& operator*=(const Vector3f& v) noexcept;
        Vector3f& operator/=(const Vector3f& v) noexcept;

        friend Vector3f operator*(float scale, const Vector3f& v) noexcept;

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
         * @brief 外積を計算します。
         *
         * @param [in] v1 一つ目のベクトル
         * @param [in] v2 二つ目のベクトル
         *
         * @return 外積
         */
        [[nodiscard]] static Vector3f cross(const Vector3f& v1, const Vector3f& v2) noexcept;

        /**
         * @brief 内積を計算します。
         *
         * @param [in] v1 一つ目のベクトル
         * @param [in] v2 二つ目のベクトル
         *
         * @return 内積
         */
        [[nodiscard]] static float dot(const Vector3f& v1, const Vector3f& v2) noexcept;

    private:
        float _x; ///< ベクトルのX成分
        float _y; ///< ベクトルのY成分
        float _z; ///< ベクトルのZ成分
    };

    inline Vector3f::Vector3f() noexcept
        : Vector3f{ 0 }
    {
    }

    inline Vector3f::Vector3f(const float value) noexcept
        : Vector3f{ value, value, value }
    {
    }

    inline Vector3f::Vector3f(const float x, const float y, const float z) noexcept
        : _x{ x }
        , _y{ y }
        , _z{ z }
    {
    }

    inline Vector3f Vector3f::operator-() const noexcept
    {
        return{ -_x, -_y, -_z };
    }

    inline Vector3f Vector3f::operator+(const Vector3f& v) const noexcept
    {
        return{ _x + v._x, _y + v._y, _z + v._z };
    }

    inline Vector3f Vector3f::operator-(const Vector3f& v) const noexcept
    {
        return{ _x - v._x, _y - v._y, _z - v._z };
    }

    inline Vector3f Vector3f::operator*(const Vector3f& v) const noexcept
    {
        return{ _x * v._x, _y * v._y, _z * v._z };
    }

    inline Vector3f Vector3f::operator*(const float scale) const noexcept
    {
        return{ _x * scale, _y * scale, _z * scale };
    }

    inline Vector3f Vector3f::operator/(const Vector3f& v) const noexcept
    {
        return{ _x / v._x, _y / v._y, _z / v._z };
    }

    inline Vector3f Vector3f::operator/(float scale) const noexcept
    {
        const float inverseScale{ 1.0f / scale };
        return{ _x * inverseScale, _y * inverseScale, _z * inverseScale };
    }

    inline bool Vector3f::operator==(const Vector3f& v) const noexcept
    {
        return (_x == v._x && _y == v._y && _z == v._z);  // NOLINT(clang-diagnostic-float-equal)
    }

    inline bool Vector3f::operator!=(const Vector3f& v) const noexcept
    {
        return (_x != v._x || _y != v._y || _z != v._z);  // NOLINT(clang-diagnostic-float-equal)
    }

    inline float& Vector3f::operator[](const int32_t index) noexcept
    {
        // ZEN_EXPECTS_MSG(0 <= index && index <= 2, u"IndexOutOfRange");
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

    inline float Vector3f::operator[](const int32_t index) const noexcept
    {
        // ZEN_EXPECTS_MSG(0 <= index && index <= 2, u"IndexOutOfRange");
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

    inline Vector3f& Vector3f::operator+=(const Vector3f& v) noexcept
    {
        _x += v._x;
        _y += v._y;
        _z += v._z;
        return *this;
    }

    inline Vector3f& Vector3f::operator-=(const Vector3f& v) noexcept
    {
        _x -= v._x;
        _y -= v._y;
        _z -= v._z;
        return *this;
    }

    inline Vector3f& Vector3f::operator*=(const Vector3f& v) noexcept
    {
        _x *= v._x;
        _y *= v._y;
        _z *= v._z;
        return *this;
    }

    inline Vector3f& Vector3f::operator/=(const Vector3f& v) noexcept
    {
        _x /= v._x;
        _y /= v._y;
        _z /= v._z;
        return *this;
    }

    inline Vector3f operator*(const float scale, const Vector3f& v) noexcept
    {
        return{ v * scale };
    }


    inline float Vector3f::getX() const noexcept
    {
        return _x;
    }

    inline float Vector3f::getY() const noexcept
    {
        return _y;
    }

    inline float Vector3f::getZ() const noexcept
    {
        return _z;
    }

    inline void Vector3f::setX(const float x) noexcept
    {
        _x = x;
    }

    inline void Vector3f::setY(const float y) noexcept
    {
        _y = y;
    }

    inline void Vector3f::setZ(const float z) noexcept
    {
        _z = z;
    }

    inline Vector3f Vector3f::cross(const Vector3f& v1, const Vector3f& v2) noexcept
    {
        return
        {
            (v1._y * v2._z - v1._z * v2._y),
            (v1._z * v2._x - v1._x * v2._z),
            (v1._x * v2._y - v1._y * v2._x)
        };
    }

    inline float Vector3f::dot(const Vector3f& v1, const Vector3f& v2) noexcept
    {
        return v1._x * v2._x + v1._y * v2._y + v1._z * v2._z;
    }
}