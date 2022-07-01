#include "MathTestHelper.h"
#include <Core/Containers/Span.hpp>
#include <gtest/gtest.h>

// Based on .NET Runtime
// https://github.com/dotnet/runtime
// runtime/src/libraries/System.Numerics.Vectors/tests/Vector3Tests.cs 
// @third party code - BEGIN .NET Runtime

using namespace zen;

TEST(Vector3Test, ConstructZero)
{
	const Vector3 v{};
	EXPECT_FLOAT_EQ(v.getX(), 0.0f);
	EXPECT_FLOAT_EQ(v.getY(), 0.0f);
	EXPECT_FLOAT_EQ(v.getZ(), 0.0f);
}

TEST(Vector3Test, ConstructOneValue)
{
	const Vector3 v{ 1.0f };
	EXPECT_FLOAT_EQ(v.getX(), 1.0f);
	EXPECT_FLOAT_EQ(v.getY(), 1.0f);
	EXPECT_FLOAT_EQ(v.getZ(), 1.0f);
}

TEST(Vector3Test, ConstructValues)
{
	const Vector3 v{ 1.0f, 2.0f, 3.0f };
	EXPECT_FLOAT_EQ(v.getX(), 1.0f);
	EXPECT_FLOAT_EQ(v.getY(), 2.0f);
	EXPECT_FLOAT_EQ(v.getZ(), 3.0f);
}

// @TODO: 未対応。
#if 0
TEST(Vector3Test, ConstructFromSpan)
{

}

TEST(Vector3Test, Cross)
{
	const Vector3 a{ 1.0f, 0.0f, 0.0f };
	const Vector3 b{ 0.0f, 1.0f, 0.0f };

	const Vector3 expected{ 0.0f, 0.0f, 1.0f };
	const Vector3 actual{ Vector3::cross(a, b) };

	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Cross1)
{
	const Vector3 a{ 0.0f, 1.0f, 0.0f };
	const Vector3 b{ 0.0f, 1.0f, 0.0f };

	const Vector3 expected{ 0.0f, 0.0f, 0.0f };
	const Vector3 actual{ Vector3::cross(a, b) };

	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Distance)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	const Vector3 b{ 4.0f, 5.0f, 6.0f };

	const float expected{ math::sqrt(27) };
	const float actual{ Vector3::distance(a, b) };

	EXPECT_FLOAT_EQ(actual, expected);
}

TEST(Vector3Test, Distance1)
{
	const Vector3 a{ 1.051f, 2.05f, 3.478f };
	const Vector3 b{ 1.051f, 2.05f, 3.478f };

	const float actual{ Vector3::distance(a, b) };

	EXPECT_FLOAT_EQ(actual, 0.0f);
}

TEST(Vector3Test, DistanceSquared)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	const Vector3 b{ 4.0f, 5.0f, 6.0f };

	const float expected{ 27.0f };
	const float actual{ Vector3::distanceSquared(a, b) };

	EXPECT_FLOAT_EQ(actual, expected);
}

TEST(Vector3Test, Dot)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	const Vector3 b{ 4.0f, 5.0f, 6.0f };

	const float expected{ 32.0f };
	const float actual{ Vector3::dot(a, b) };

	EXPECT_FLOAT_EQ(actual, expected);
}

TEST(Vector3Test, Dot1)
{
	const Vector3 a{ 1.55f, 1.55f, 1.0f };
	const Vector3 b{ 2.5f, 3.0f, 1.5f };
	const Vector3 c{ Vector3::cross(a, b) };

	float expected = 0.0f;
	float actual1{ Vector3::dot(a, c) };
	float actual2{ Vector3::dot(b, c) };

	// .net coreの誤差許容と異なっていてエラーになるので、ここだけは許容値.net runtimeと同じにする。
	EXPECT_NEAR(actual1, expected, 1e-5);
	EXPECT_NEAR(actual2, expected, 1e-5);
}

TEST(Vector3Test, Length)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	const Vector3 b{ 2.5f, 3, 1.5f };
	const Vector3 c{ Vector3::cross(a, b) };

	const float expected{ math::sqrt(14.0f) };
	const float actual1{ a.length() };

	EXPECT_FLOAT_EQ(actual1, expected);
}

TEST(Vector3Test, Length1)
{
	const Vector3 target{};

	const float expected{ 0.0f };
	const float actual1{ target.length() };

	EXPECT_FLOAT_EQ(actual1, expected);
}

TEST(Vector3Test, LengthSquared)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };

	const float expected{ 14.0f };
	const float actual1{ a.lengthSquared() };

	EXPECT_FLOAT_EQ(actual1, expected);
}

TEST(Vector3Test, Min)
{
	const Vector3 a{ -1.0f, 4.0f, -3.0f };
	const Vector3 b{ 2.0f, 1.0f, -1.0f };


	const Vector3 expected{ -1.0f, 1.0f, -3.0f };
	const Vector3 actual{ Vector3::min(a, b) };

	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Max)
{
	const Vector3 a{ -1.0f, 4.0f, -3.0f };
	const Vector3 b{ 2.0f, 1.0f, -1.0f };


	const Vector3 expected{ 2.0f, 4.0f, -1.0f };
	const Vector3 actual{ Vector3::max(a, b) };
	
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, MinMaxCodeCoverage)
{
	const Vector3 min{ Vector3::zero };
	const Vector3 max{ Vector3::one };

	Vector3 actual{ Vector3::min(min, max) };
	EXPECT_VECTOR3_EQ(actual, min);

	actual = Vector3::min(max, min);
	EXPECT_VECTOR3_EQ(actual, min);

	actual = Vector3::max(max, max);
	EXPECT_VECTOR3_EQ(actual, max);

	actual = Vector3::max(max, min);
	EXPECT_VECTOR3_EQ(actual, max);
}

TEST(Vector3Test, Lerp)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	const Vector3 b{ 4.0f, 5.0f, 6.0f };

	const float t{ 0.5f };

	const Vector3 expected{ 2.5f, 3.5f, 4.5f };
	const Vector3 actual{ Vector3::lerp(a, b, t) };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Lerp1)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	const Vector3 b{ 4.0f, 5.0f, 6.0f };

	const float t{ 0.0f };

	const Vector3 expected{ 1.0f, 2.0f, 3.0f };
	const Vector3 actual{ Vector3::lerp(a, b, t) };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Lerp2)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	const Vector3 b{ 4.0f, 5.0f, 6.0f };

	const float t{ 1.0f };

	const Vector3 expected{ 4.0f, 5.0f, 6.0f };
	const Vector3 actual{ Vector3::lerp(a, b, t) };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Lerp3)
{
	const Vector3 a{ 0.0f, 0.0f, 0.0f };
	const Vector3 b{ 4.0f, 5.0f, 6.0f };

	const float t{ 2.0f };

	const Vector3 expected{ 8.0f, 10.0f, 12.0f };
	const Vector3 actual{ Vector3::lerp(a, b, t) };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Lerp4)
{
	const Vector3 a{ 0.0f, 0.0f, 0.0f };
	const Vector3 b{ 4.0f, 5.0f, 6.0f };

	const float t{ -2.0f };

	const Vector3 expected{ -8.0f, -10.0f, -12.0f };
	const Vector3 actual{ Vector3::lerp(a, b, t) };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Lerp5)
{
	const Vector3 a{ 45.67f, 90.0f, 0.0f };
	const Vector3 b
	{ 
		std::numeric_limits<float>::infinity(), 
		-std::numeric_limits<float>::infinity(),
		0.0f 
	};

	const float t{ 0.408f };

	const Vector3 actual{ Vector3::lerp(a, b, t) };
	EXPECT_FLOAT_EQ(actual.x, std::numeric_limits<float>::infinity());
	EXPECT_FLOAT_EQ(actual.y, -std::numeric_limits<float>::infinity());
}

TEST(Vector3Test, Lerp6)
{
	const Vector3 a{ 1.68f, 2.34f, 5.43f };
	const Vector3 b{ a };

	const float t{ 0.18f };

	const Vector3 expected{ 1.68f, 2.34f, 5.43f };
	const Vector3 actual{ Vector3::lerp(a, b, t) };

	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Lerp7)
{
	const Vector3 a{ 0.44728136f };
	const Vector3 b{ 0.46345946f };

	const float t{ 0.26402435f };

	const Vector3 expected{ 0.45155275f };
	const Vector3 actual{ Vector3::lerp(a, b, t) };

	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Lerp8)
{
	const Vector3 a{ -100.0f };
	const Vector3 b{ 0.33333334f };

	const float t{ 1.0f };

	const Vector3 expected{ 0.33333334f };
	const Vector3 actual{ Vector3::lerp(a, b, t) };

	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Reflect)
{
	// XZ-Plane
	const Vector3 a{ Vector3(1.0f, 1.0f, 1.0f).normalizedUnsafe() };
	Vector3 n{ 0.0f, 1.0f, 0.0f };

	Vector3 expected{ a.x, -a.y, a.z };
	Vector3 actual{ Vector3::reflect(a, n) };

	EXPECT_VECTOR3_EQ(actual, expected);

	// XY-Plane
	n = Vector3{ 0.0f, 0.0f, 1.0f };

	expected = Vector3{ a.x, a.y, -a.z };
	actual = Vector3::reflect(a, n);

	EXPECT_VECTOR3_EQ(actual, expected);

	// YZ-Plane
	n = Vector3{ 1.0f, 0.0f, 0.0f };

	expected = Vector3{ -a.x, a.y, a.z };
	actual = Vector3::reflect(a, n);

	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Reflect1)
{
	Vector3 n{ 0.45f, 1.28f, 0.86f };
	n = n.normalizedUnsafe();

	const Vector3 a{ n };
	const Vector3 actual{ Vector3::reflect(a, n) };
	const Vector3 expected{ -n };

	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Reflect2)
{
	Vector3 n{ 0.45f, 1.28f, 0.86f };
	n = n.normalizedUnsafe();

	const Vector3 a{ -n };
	const Vector3 actual{ Vector3::reflect(a, n) };
	const Vector3 expected{ n };

	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Reflect3)
{
	Vector3 n{ 0.45f, 1.28f, 0.86f };
	Vector3 temp{ 1.28f, 0.45f, 0.01f };

	// find a perpendicular vector of n
	Vector3 a{ Vector3::cross(temp, n) };

	Vector3 expected = a;
	Vector3 actual{ Vector3::reflect(a, n) };
	
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Normalize)
{
	Vector3 actual{ 1.0f, 2.0f, 3.0f };

	const Vector3 expected
	{ 
		0.26726124191242438468455348087975f,
		0.53452248382484876936910696175951f,
		0.80178372573727315405366044263926f
	};

	
	actual = actual.normalizedUnsafe();
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Normalize1)
{
	Vector3 actual{ 1.0f, 0.0f, 0.0f };

	const Vector3 expected{ 1.0f, 0.0f, 0.0f };


	actual = actual.normalizedUnsafe();
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Normalize2)
{
	const Vector3 a{ 0.0f, 0.0f, 0.0f };

	const Vector3 expected{ 0.0f, 0.0f, 0.0f };

	const Vector3 actual{ a.normalizedUnsafe() };
	EXPECT_TRUE(std::isnan(actual.x));
	EXPECT_TRUE(std::isnan(actual.y));
	EXPECT_TRUE(std::isnan(actual.z));
}

TEST(Vector3Test, UnaryNegation)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };

	const Vector3 expected{ -1.0f, -2.0f, -3.0f };
	const Vector3 actual{ -a };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, UnaryNegation1)
{
	const Vector3 a{ -Vector3{ std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()} };
	const Vector3 b{ -Vector3{0.0f, 0.0f, 0.0f} };

	EXPECT_FLOAT_EQ(std::numeric_limits<float>::quiet_NaN(), a.x);
	EXPECT_FLOAT_EQ(-std::numeric_limits<float>::infinity(), a.y);
	EXPECT_FLOAT_EQ(std::numeric_limits<float>::infinity(), a.z);
	EXPECT_FLOAT_EQ(0.0f, b.x);
	EXPECT_FLOAT_EQ(0.0f, b.y);
	EXPECT_FLOAT_EQ(0.0f, b.z);
}

TEST(Vector3Test, Substraction)
{
	const Vector3 a{ 4.0f, 2.0f, 3.0f };
	const Vector3 b{ 1.0f, 5.0f, 7.0f };

	const Vector3 expected{ 3.0f, -3.0f, -4.0f };
	const Vector3 actual{ a - b };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Multiply)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };

	const float factor{ 2.0f };

	const Vector3 expected{ 2.0f, 4.0f, 6.0f };
	const Vector3 actual{ a * factor };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Multiply1)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };

	const float factor{ 2.0f };

	const Vector3 expected{ 2.0f, 4.0f, 6.0f };
	const Vector3 actual{ factor * a };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Multiply2)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	const Vector3 b{ 4.0f, 5.0f, 6.0f };


	const Vector3 expected{ 4.0f, 10.0f, 18.0f };
	const Vector3 actual{ a * b };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Negate)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };

	const Vector3 expected{ -1.0f, -2.0f, -3.0f };
	const Vector3 actual{ -a };
	EXPECT_VECTOR3_EQ(actual, expected);
}

TEST(Vector3Test, Inequality)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	Vector3 b{ 1.0f, 2.0f, 3.0f };

	bool actual{ a != b };
	EXPECT_FALSE(actual);

	b.x = 10.0f;
	actual = a != b;
	EXPECT_TRUE(actual);
}

TEST(Vector3Test, Equality)
{
	const Vector3 a{ 1.0f, 2.0f, 3.0f };
	Vector3 b{ 1.0f, 2.0f, 3.0f };

	bool actual{ a == b };
	EXPECT_TRUE(actual);

	b.x = 10.0f;
	actual = a == b;
	EXPECT_FALSE(actual);
}

TEST(Vector3Test, One)
{
	const Vector3 expected{ 1.0f, 1.0f, 1.0f };
	EXPECT_VECTOR3_EQ(Vector3::one, expected);
}

TEST(Vector3Test, Zero)
{
	const Vector3 expected{ 0.0f, 0.0f, 0.0f };
	EXPECT_VECTOR3_EQ(Vector3::zero, expected);
}

TEST(Vector3Test, Sizeof)
{
	EXPECT_EQ(sizeof(Vector3), 12);
}

TEST(Vector3Test, SetFields)
{
	Vector3 v3{ 4.0f, 5.0f, 6.0f };
	v3.x = 1.0f;
	v3.y = 2.0f;
	v3.z = 3.0f;

	EXPECT_FLOAT_EQ(v3.x, 1.0f);
	EXPECT_FLOAT_EQ(v3.y, 2.0f);
	EXPECT_FLOAT_EQ(v3.z, 3.0f);

	Vector3 v4{ v3 };
	v4.y = 0.5f;
	v4.z = 2.2f;

	EXPECT_FLOAT_EQ(v4.x, 1.0f);
	EXPECT_FLOAT_EQ(v4.y, 0.5f);
	EXPECT_FLOAT_EQ(v4.z, 2.2f);
	EXPECT_FLOAT_EQ(v3.y, 2.0f);

	
	const Vector3 before{ 1.0f, 2.0f, 3.0f };
	Vector3 after{ before };
	after.x = 500.0f;

	EXPECT_NE(before, after);
}
#endif
// @third party code - End .NET Runtime