#pragma once

#if defined(_MSC_VER)
#define ZEN_FORCEINLINE __forceinline
#else
#define ZEN_FORCEINLINE inline __attribute__((always_inline))
#endif