#include <Core/Memory/SharedPtr.hpp>
#include <gtest/gtest.h>

TEST(SharedPtrTest, ConstructEmpty)
{
    using namespace zen;
    TSharedPtr<int, ThreadPolicy::ThreadSafe> actual{};
}
