#pragma once
#include <cstdint>
#include <span>

namespace zen
{
    [[nodiscard]]
    uint32_t xxhash32(std::span<const uint8_t> span, uint32_t seed);

    [[nodiscard]]
    uint64_t xxhash64(std::span<const uint8_t> span, uint64_t seed);

    [[nodiscard]]
    uint64_t xxhash3_64(std::span<const uint8_t> span, uint64_t seed);
}