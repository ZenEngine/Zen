#include <Core/Memory/UniquePtr.hpp>
#include <gtest/gtest.h>
#include <memory>

class Parent
{
public:
    virtual ~Parent() noexcept = default;
};

class Child : public Parent
{
public:

};

TEST(UniquePtrTest, ConstructEmpty)
{
    zen::TUniquePtr<int> actual{};
    EXPECT_EQ(actual, nullptr);
    EXPECT_EQ(actual.get(), nullptr);
}

TEST(UniquePtrTest, Construct)
{
    zen::TUniquePtr<int> actual{ zen::makeUnique<int>(1) };
    EXPECT_EQ(*actual.get(), 1);
    *actual = 10;
    EXPECT_EQ(*actual.get(), 10);
}

TEST(UniquePtrTest, ConstructChild)
{
    zen::TUniquePtr<Parent> test{ zen::makeUnique<Child>() };
}
