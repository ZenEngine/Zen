#pragma once
//#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>
//#include <Math/Matrix4x4.hpp>

#define EXPECT_VECTOR2_EQ(v1, v2) \
EXPECT_FLOAT_EQ(v1.getX(), v2.getX()); \
EXPECT_FLOAT_EQ(v1.getY(), v2.getY())

#define EXPECT_VECTOR3_EQ(v1, v2) \
EXPECT_FLOAT_EQ(v1.getX(), v2.getX()); \
EXPECT_FLOAT_EQ(v1.getY(), v2.getY()); \
EXPECT_FLOAT_EQ(v1.getZ(), v2.getZ())

#define EXPECT_VECTOR4_EQ(v1, v2) \
EXPECT_FLOAT_EQ(v1.getX(), v2.getX()); \
EXPECT_FLOAT_EQ(v1.getY(), v2.getY()); \
EXPECT_FLOAT_EQ(v1.getZ(), v2.getZ()); \
EXPECT_FLOAT_EQ(v1.getW(), v2.getW())

#define EXPECT_MATRIX44_EQ(mat1, mat2) \
EXPECT_VECTOR4_EQ(mat1.getXAxis(), mat2.getXAxis()); \
EXPECT_VECTOR4_EQ(mat1.getYAxis(), mat2.getYAxis()); \
EXPECT_VECTOR4_EQ(mat1.getZAxis(), mat2.getZAxis()); \
EXPECT_VECTOR4_EQ(mat1.getWAxis(), mat2.getWAxis())
