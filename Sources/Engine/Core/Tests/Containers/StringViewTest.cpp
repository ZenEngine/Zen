#include <Core/Containers/StringView.hpp>
#include <gtest/gtest.h>

TEST(StringViewTest, Construct_Empty)
{
    zen::StringView actual{};
    ASSERT_EQ(actual.getData(), nullptr);
    ASSERT_EQ(actual.getSize(), 0);
    ASSERT_TRUE(actual.isEmpty());
}

TEST(StringViewTest, Construct_Copy)
{
    zen::StringView expected{ u"Copyconstructor" };
    zen::StringView actual{ expected  };
    ASSERT_EQ(actual, expected);
}
