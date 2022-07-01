#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>
#include <Core/Containers/Span.hpp>

namespace zen
{
    const Vector3 Vector3::zero{ 0.0f };
    const Vector3 Vector3::one{ 1.0f };

    const Vector4 Vector4::zero{ 0.0f };
    const Vector4 Vector4::one{ 1.0f };

    Vector3::Vector3(TSpan<const float, 3> span) noexcept
        : Vector3{ span[0], span[1], span[2] }
    {
    }

    Vector4::Vector4(TSpan<const float, 4> span) noexcept
        : Vector4{ span[0], span[1], span[2], span[3] }
    {
    }
}