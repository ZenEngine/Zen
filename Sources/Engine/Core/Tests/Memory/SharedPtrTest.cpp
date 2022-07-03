#include <Core/Memory/Memory.hpp>
#include <gtest/gtest.h>


using namespace zen;

//  クリエイティブ・コモンズ 表示 3.0 非移植 ライセンス(CC BY)
// https://cpprefjp.github.io/reference/memory/shared_ptr/op_constructor.html

TEST(SharedPtrTest, ConstructEmpty)
{
    {
        TSharedPtr<int, ThreadPolicy::NotThreadSafe> actual{};
        EXPECT_EQ(actual.get(), nullptr);
        EXPECT_EQ(actual.getUseCount(), 0);
    }

    {
        TSharedPtr<int, ThreadPolicy::ThreadSafe> actual{};
        EXPECT_EQ(actual.get(), nullptr);
        EXPECT_EQ(actual.getUseCount(), 0);
    }
}

TEST(SharedPtrTest, Construct2)
{
    {
        TSharedPtr<int, ThreadPolicy::NotThreadSafe> actual(new int(3));
        EXPECT_EQ(*actual.get(), 3);
        EXPECT_EQ(actual.getUseCount(), 1);
    }
    {
        TSharedPtr<int, ThreadPolicy::ThreadSafe> actual(new int(3));
        EXPECT_EQ(*actual.get(), 3);
        EXPECT_EQ(actual.getUseCount(), 1);
    }
}

TEST(SharedPtrTest, Construct3)
{
    TSharedPtr<int, ThreadPolicy::NotThreadSafe> actual(new int(3), TDefaultDelete<int>());
    EXPECT_EQ(*actual.get(), 3);
    EXPECT_EQ(actual.getUseCount(), 1);
}

