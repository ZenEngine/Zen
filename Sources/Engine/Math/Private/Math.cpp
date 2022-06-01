#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>
#include <span>

namespace zen
{
    const Vector3f Vector3f::zero{ 0.0f };
    const Vector3f Vector3f::one{ 1.0f };

    const Vector4f Vector4f::zero{ 0.0f };
    const Vector4f Vector4f::one{ 1.0f };

    Vector3f::Vector3f(std::span<const float, 3> span) noexcept
        : Vector3f{ span[0], span[1], span[2] }
    {
    }

    Vector4f::Vector4f(std::span<const float, 4> span) noexcept
        : Vector4f{ span[0], span[1], span[2], span[3] }
    {
    }
}