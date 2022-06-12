#include <Core/Memory/SharedPtr.hpp>
#include <gtest/gtest.h>

using namespace zen;

TEST(SharedPtrTest, ConstructEmpty)
{
    TSharedPtr<int, ThreadPolicy::ThreadSafe> actual{};
    EXPECT_EQ(actual.get(), nullptr);
    EXPECT_EQ(actual.getUseCount(), 0);
}

TEST(SharedPtrTest, Construct2)
{
    TSharedPtr<int, ThreadPolicy::NotThreadSafe> actual(new int(3));
    EXPECT_EQ(*actual.get(), 3);
    EXPECT_EQ(actual.getUseCount(), 1);
}

