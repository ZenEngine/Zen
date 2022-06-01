#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>
#include <span>

namespace zen
{
    const Vector3F Vector3F::zero{ 0.0f };
    const Vector3F Vector3F::one{ 1.0f };

    const Vector4F Vector4F::zero{ 0.0f };
    const Vector4F Vector4F::one{ 1.0f };

    Vector3F::Vector3F(std::span<const float, 3> span) noexcept
        : Vector3F{ span[0], span[1], span[2] }
    {
    }

    Vector4F::Vector4F(std::span<const float, 4> span) noexcept
        : Vector4F{ span[0], span[1], span[2], span[3] }
    {
    }
}