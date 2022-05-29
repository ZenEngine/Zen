#include <Math/Vector3.hpp>
#include <span>

namespace zen
{
    const Vector3f Vector3f::zero{ 0.0f };
    const Vector3f Vector3f::one{ 1.0f };

    Vector3f::Vector3f(std::span<const float, 3> span) noexcept
        : Vector3f{ span[0], span[1], span[2] }
    {
    }
}