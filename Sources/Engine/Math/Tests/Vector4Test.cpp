#include "MathTestHelper.hpp"
#include <gtest/gtest.h>

// Based on .NET Runtime
// https://github.com/dotnet/runtime
// @third party code - BEGIN .NET Runtime

using namespace zen;

TEST(Vector4Test, Constructor)
{
	const float x{ 1.0f };
	const float y{ 2.0f };
	const float z{ 3.0f };
	const float w{ 4.0f };

	const Vector4 target{ x, y, z, w };

	EXPECT_FLOAT_EQ(target.getX(), x);
	EXPECT_FLOAT_EQ(target.getY(), y);
	EXPECT_FLOAT_EQ(target.getZ(), z);
	EXPECT_FLOAT_EQ(target.getW(), w);
}

// @TODO Iml Vec2
/*
TEST(Vector4Test, Constructor1)
{
	const Vector2 a{ 1.0f, 2.0f };
	const float z{ 3.0f };
	const float w{ 4.0f };

	const Vector4 target{ a, z, w };

	EXPECT_FLOAT_EQ(target.getX(), a.getX());
	EXPECT_FLOAT_EQ(target.getY(), a.y);
	EXPECT_FLOAT_EQ(target.getZ(), z);
	EXPECT_FLOAT_EQ(target.getW(), w);
}

TEST(Vector4Test, Constructor3)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	const float w{ 4.0f };

	const Vector4 target{ a, w };

	EXPECT_FLOAT_EQ(target.getX(), a.getX());
	EXPECT_FLOAT_EQ(target.getY(), a.getY());
	EXPECT_FLOAT_EQ(target.getZ(), a.getZ());
	EXPECT_FLOAT_EQ(target.getW(), w);
}
*/

TEST(Vector4Test, Constructor4)
{
	const Vector4 a{};

	EXPECT_FLOAT_EQ(0.0f, a.getX());
	EXPECT_FLOAT_EQ(0.0f, a.getY());
	EXPECT_FLOAT_EQ(0.0f, a.getZ());
	EXPECT_FLOAT_EQ(0.0f, a.getW());
}

TEST(Vector4Test, Constructor5)
{
	const Vector4 a
	{
		std::numeric_limits<float>::quiet_NaN(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::epsilon()
	};

	EXPECT_TRUE(std::isnan(a.getX()));
	EXPECT_FLOAT_EQ(std::numeric_limits<float>::max(), a.getY());
	EXPECT_FLOAT_EQ(std::numeric_limits<float>::infinity(), a.getZ());
	EXPECT_FLOAT_EQ(std::numeric_limits<float>::epsilon(), a.getW());
}

TEST(Vector4Test, Constructor6)
{
	float value{ 1.0f };
	Vector4 target{ value };

	Vector4 expected{ value, value, value, value };
	EXPECT_VECTOR4_EQ(target, expected);

	value = 2.0f;
	target = Vector4{ value };
	expected = Vector4{ value, value, value, value };
	EXPECT_VECTOR4_EQ(target, expected);
}


TEST(Vector4Test, Dot)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };
	const Vector4 b{ 5.0f, 6.0f, 7.0f, 8.0f };

	const float expected{ 70.0f };
	const float actual{ Vector4::dot(a, b) };

	EXPECT_FLOAT_EQ(actual, expected);
}

TEST(Vector4Test, Dot1)
{
	const Vector3 a{ 1.55f, 1.55f, 1.0f };
	const Vector3 b{ 2.5f, 3.0f, 1.5f };
	const Vector3 c{ Vector3::cross(a, b) };

	const Vector4 d{ a, 0.0f };
	const Vector4 e{ c, 0.0f };

	// .net coreの誤差許容と異なっていてエラーになるので、ここだけは許容値.net runtimeと同じにする。
	const float actual{ Vector4::dot(d, e) };
	EXPECT_NEAR(actual, 0.0f, 1e-5);
	
}

TEST(Vector4Test, Lerp)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };
	const Vector4 b{ 5.0f, 6.0f, 7.0f, 8.0f };

	const float t{ 0.5f };

	const Vector4 expected{ 3.0f, 4.0f, 5.0f, 6.0f };
	const Vector4 actual{ Vector4::lerp(a, b, t) };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Lerp1)
{
	const Vector4 a{ Vector3{ 1.0f, 2.0f, 3.0f }, 4.0f };
	const Vector4 b{ 4.0f, 5.0f, 6.0f, 7.0f };

	const float t{ 0.0f };

	const Vector4 expected{ 1.0f, 2.0f, 3.0f, 4.0f };
	const Vector4 actual{ Vector4::lerp(a, b, t) };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Lerp2)
{
	const Vector4 a{ Vector3{ 1.0f, 2.0f, 3.0f }, 4.0f };
	const Vector4 b{ 4.0f, 5.0f, 6.0f, 7.0f };

	const float t{ 1.0f };

	const Vector4 expected{ 4.0f, 5.0f, 6.0f, 7.0f };
	const Vector4 actual{ Vector4::lerp(a, b, t) };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Lerp3)
{
	const Vector4 a{ Vector3{ 0.0f, 0.0f, 0.0f }, 0.0f };
	const Vector4 b{ 4.0f, 5.0f, 6.0f, 7.0f };

	const float t{ 2.0f };

	const Vector4 expected{ 8.0f, 10.0f, 12.0f, 14.0f };
	const Vector4 actual{ Vector4::lerp(a, b, t) };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Lerp4)
{
	const Vector4 a{ Vector3{ 0.0f, 0.0f, 0.0f }, 0.0f };
	const Vector4 b{ 4.0f, 5.0f, 6.0f, 7.0f };

	const float t{ -2.0f };

	const Vector4 expected{ -(b * 2.0f) };
	const Vector4 actual{ Vector4::lerp(a, b, t) };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Lerp5)
{
	const Vector4 a{ 45.67f, 90.0f, 0.0f, 0.0f };
	const Vector4 b{ std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), 0.0f, 0.0f };

	const float t{ 0.408f };

	const Vector4 actual{ Vector4::lerp(a, b, t) };

	EXPECT_FLOAT_EQ(actual.getX(), std::numeric_limits<float>::infinity());
	EXPECT_FLOAT_EQ(actual.getY(), -std::numeric_limits<float>::infinity());
}

TEST(Vector4Test, Lerp6)
{
	const Vector4 a{ 4.0f, 5.0f, 6.0f, 7.0f };
	const Vector4 b{ 4.0f, 5.0f, 6.0f, 7.0f };

	const float t{ 0.85f };

	const Vector4 expected{ a };
	const Vector4 actual{ Vector4::lerp(a, b, t) };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Lerp7)
{
	const Vector4 a{ 0.44728136f };
	const Vector4 b{ 0.46345946f };

	const float t{ 0.26402435f };

	const Vector4 expected{ 0.45155275f };
	const Vector4 actual{ Vector4::lerp(a, b, t) };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Lerp8)
{
	const Vector4 a{ -100.0f };
	const Vector4 b{ 0.33333334f };

	const float t{ 1.0f };

	const Vector4 expected{ 0.33333334f };
	const Vector4 actual{ Vector4::lerp(a, b, t) };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, UnaryNegation)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };

	const Vector4 expected{ -1.0f, -2.0f, -3.0f, -4.0f };
	const Vector4 actual{ -a };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Subtraction)
{
	const Vector4 a{ 1.0f, 6.0f, 3.0f, 4.0f };
	const Vector4 b{ 5.0f, 2.0f, 3.0f, 9.0f };

	const Vector4 expected{ -4.0f, 4.0f, 0.0f, -5.0f };
	const Vector4 actual{ a - b };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Multiply)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };

	const float factor{ 2.0f };

	const Vector4 expected{ 2.0f, 4.0f, 6.0f, 8.0f };
	const Vector4 actual{ a * factor };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Multiply2)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };

	const float factor{ 2.0f };

	const Vector4 expected{ 2.0f, 4.0f, 6.0f, 8.0f };
	const Vector4 actual{ factor * a };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Multiply3)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };
	const Vector4 b{ 5.0f, 6.0f, 7.0f, 8.0f };

	const Vector4 expected{ 5.0f, 12.0f, 21.0f, 32.0f };
	const Vector4 actual{ a * b };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Division)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };
	
	const float div{ 2.0f };

	const Vector4 expected{ 0.5f, 1.0f, 1.5f, 2.0f };
	const Vector4 actual{ a / div };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Division1)
{
	const Vector4 a{ 1.0f, 6.0f, 7.0f, 4.0f };
	const Vector4 b{ 5.0f, 2.0f, 3.0f, 8.0f };

	const Vector4 expected{ 1.0f / 5.0f, 6.0f / 2.0f, 7.0f / 3.0f, 4.0f / 8.0f };
	const Vector4 actual{ a / b };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Division2)
{
	const Vector4 a{ -2.0f, 3.0f, std::numeric_limits<float>::max(), std::numeric_limits<float>::quiet_NaN() };
	
	const float div{ 0.0f };

	const Vector4 actual{ a / div };

	EXPECT_FLOAT_EQ(-std::numeric_limits<float>::infinity(), actual.getX());
	EXPECT_FLOAT_EQ(std::numeric_limits<float>::infinity(), actual.getY());
	EXPECT_FLOAT_EQ(std::numeric_limits<float>::infinity(), actual.getZ());
	EXPECT_TRUE(std::isnan(actual.getW()));
}

TEST(Vector4Test, Division3)
{
	const Vector4 a{ 0.047f, -3.0f, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::min() };
	const Vector4 b{};

	const Vector4 actual{ a / b };

	EXPECT_FLOAT_EQ(std::numeric_limits<float>::infinity(), actual.getX());
	EXPECT_FLOAT_EQ(-std::numeric_limits<float>::infinity(), actual.getY());
	EXPECT_FLOAT_EQ(-std::numeric_limits<float>::infinity(), actual.getZ());
	EXPECT_FLOAT_EQ(-std::numeric_limits<float>::infinity(), actual.getW());
}

TEST(Vector4Test, Addition)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };
	const Vector4 b{ 5.0f, 6.0f, 7.0f, 8.0f };

	const Vector4 expected{ 6.0f, 8.0f, 10.0f, 12.0f };
	const Vector4 actual{ a + b };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Subtract)
{
	const Vector4 a{ 1.0f, 6.0f, 3.0f, 4.0f };
	const Vector4 b{ 5.0f, 2.0f, 3.0f, 9.0f };

	const Vector4 expected{ -4.0f, 4.0f, 0.0f, -5.0f };
	const Vector4 actual{ a - b };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, One)
{
	const Vector4 val{ 1.0f, 1.0f, 1.0f, 1.0f };
	EXPECT_VECTOR4_EQ(val, Vector4::one);
}

TEST(Vector4Test, Zero)
{
	const Vector4 val{ 0.0f, 0.0f, 0.0f, 0.0f };
	EXPECT_VECTOR4_EQ(val, Vector4::zero);
}

TEST(Vector4Test, Equals)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };
	Vector4 b{ 1.0f, 2.0f, 3.0f, 4.0f };

	EXPECT_EQ(a, b);
	b.setX(10.0f);
	
	EXPECT_NE(a, b);
}

TEST(Vector4Test, Negate)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };

	const Vector4 expected{ -1.0f, -2.0f, -3.0f, -4.0f };
	const Vector4 actual{ -a };

	EXPECT_VECTOR4_EQ(actual, expected);
}

TEST(Vector4Test, Inequality)
{
	const Vector4 a{ 1.0f, 2.0f, 3.0f, 4.0f };
	Vector4 b{ 1.0f, 2.0f, 3.0f, 4.0f };
	bool actual{ a != b };

	EXPECT_FALSE(actual);

	b.setX(10.0f);
	actual = a != b;

	EXPECT_TRUE(actual);
}

TEST(Vector4Test, Sizeof)
{
	EXPECT_EQ(sizeof(Vector4), 16);
}

TEST(Vector4Test, SetFields)
{
	Vector4 v3{ 4.0f, 5.0f, 6.0f, 7.0f };
	v3.setX(1.0f);
	v3.setY(2.0f);
	v3.setZ(3.0f);
	v3.setW(4.0f);

	EXPECT_FLOAT_EQ(v3.getX(), 1.0f);
	EXPECT_FLOAT_EQ(v3.getY(), 2.0f);
	EXPECT_FLOAT_EQ(v3.getZ(), 3.0f);
	EXPECT_FLOAT_EQ(v3.getW(), 4.0f);

	Vector4 v4{ v3 };
	v4.setY(0.5f);
	v4.setZ(2.2f);
	v4.setW(3.5f);

	EXPECT_FLOAT_EQ(v4.getX(), 1.0f);
	EXPECT_FLOAT_EQ(v4.getY(), 0.5f);
	EXPECT_FLOAT_EQ(v4.getZ(), 2.2f);
	EXPECT_FLOAT_EQ(v4.getW(), 3.5f);
	EXPECT_FLOAT_EQ(v3.getY(), 2.0f);
}

// @third party code - End .NET Runtime