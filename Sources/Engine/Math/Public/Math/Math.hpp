#pragma once
#include <Core/Platform/PlatformDefine.hpp>

namespace zen
{
    namespace math
    {
        template<typename T>
        constexpr T pi_v{ static_cast<T>(3.141592653589793) };

        [[nodiscard]]
        ZEN_FORCEINLINE constexpr float toDegrees(const float radians) noexcept
        {
            return radians * (180.f / pi_v<float>);
        }

        [[nodiscard]]
        ZEN_FORCEINLINE constexpr double toDegrees(const double radians) noexcept
        {
            return radians * (180.0 / pi_v<double>);
        }

        [[nodiscard]]
        ZEN_FORCEINLINE constexpr float toRadians(const float degrees) noexcept
        {
            return degrees * (pi_v<float> / 180.f);
        }

        [[nodiscard]]
        ZEN_FORCEINLINE constexpr double toRadians(const double degrees) noexcept
        {
            return degrees * (pi_v<double> / 180.0);
        }
    }
}