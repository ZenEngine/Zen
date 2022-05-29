#include <Core/Hash/XxHash.hpp>
#include <xxhash.h>


namespace zen
{
    uint32_t xxhash32(std::span<const std::uint8_t> span, const uint32_t seed)
    {
        return XXH32(span.data(), span.size(), seed);
    }

    uint64_t xxhash64(std::span<const std::uint8_t> span, const uint64_t seed)
    {
        return XXH64(span.data(), span.size(), seed);
    }

    uint64_t xxhash3_64(std::span<const std::uint8_t> span, const uint64_t seed)
    {
        return XXH3_64bits_withSeed(span.data(), span.size(), seed);
    }
}
