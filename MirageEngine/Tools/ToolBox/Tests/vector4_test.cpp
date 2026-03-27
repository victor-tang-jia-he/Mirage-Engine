#include "pch.h"
#include <vector4.h>
using namespace MirageEngine::Core::Math;

inline static bool operator==(const Vector4& _v1, const glm::vec4& _v2)
{
	return std::abs(_v1.X - _v2.x) < EPSILON &&
		std::abs(_v1.Y - _v2.y) < EPSILON &&
		std::abs(_v1.Z - _v2.z) < EPSILON &&
		std::abs(_v1.W - _v2.w) < EPSILON;
}

inline static bool operator==(const glm::vec4& _v1, const Vector4& _v2)
{
	return _v2 == _v1;
}

// -------------------------------------------------------------------------------- //	
//									TEST VECTOR4									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Vector4
/// </summary>

Vector4 vec4(3.5f, -2.1f, 4.0f, 2.0f);
glm::vec4 glmVec4(3.5f, -2.1f, 4.0f, 2.0f);

Vector4 vec4Add(1.f, 1.f, 1.f, 1.f);
glm::vec4 glmVec4Add(1.f, 1.f, 1.f, 1.f);

TEST(Vector4Test, Constructor) {
	Vector4 v1;
	EXPECT_FLOAT_EQ(v1.X, 0.0f);
	EXPECT_FLOAT_EQ(v1.Y, 0.0f);
	EXPECT_FLOAT_EQ(v1.Z, 0.0f);
	EXPECT_FLOAT_EQ(v1.W, 1.0f);

	Vector4 v2(1.5f, -2.3f, 4.7f, 2.0f);
	EXPECT_FLOAT_EQ(v2.X, 1.5f);
	EXPECT_FLOAT_EQ(v2.Y, -2.3f);
	EXPECT_FLOAT_EQ(v2.Z, 4.7f);
	EXPECT_FLOAT_EQ(v2.W, 2.0f);

	Vector4 v3(1.0f, 2.0f, 3.0f);
	EXPECT_FLOAT_EQ(v3.X, 1.0f);
	EXPECT_FLOAT_EQ(v3.Y, 2.0f);
	EXPECT_FLOAT_EQ(v3.Z, 3.0f);
	EXPECT_FLOAT_EQ(v3.W, 1.0f);
}
TEST(Vector4Test, ConstructorFromVector3) {
	Vector3 v3(2.0f, 3.0f, 4.0f);
	Vector4 v4(v3);

	EXPECT_FLOAT_EQ(v4.X, 2.0f);
	EXPECT_FLOAT_EQ(v4.Y, 3.0f);
	EXPECT_FLOAT_EQ(v4.Z, 4.0f);
	EXPECT_FLOAT_EQ(v4.W, 1.0f);

	Vector4 v4b(v3, 5.0f);
	EXPECT_FLOAT_EQ(v4b.X, 2.0f);
	EXPECT_FLOAT_EQ(v4b.Y, 3.0f);
	EXPECT_FLOAT_EQ(v4b.Z, 4.0f);
	EXPECT_FLOAT_EQ(v4b.W, 5.0f);
}
TEST(Vector4Test, ConversionToVector3) {
	Vector4 v4(1.0f, 2.0f, 3.0f, 4.0f);
	Vector3 v3 = static_cast<Vector3>(v4);

	EXPECT_FLOAT_EQ(v3.X, 1.0f);
	EXPECT_FLOAT_EQ(v3.Y, 2.0f);
	EXPECT_FLOAT_EQ(v3.Z, 3.0f);

	Vector2 v2 = static_cast<Vector2>(v4);
	EXPECT_FLOAT_EQ(v2.X, 1.0f);
	EXPECT_FLOAT_EQ(v2.Y, 2.0f);
}
TEST(Vector4Test, Opposed) {
	Vector4 result = vec4.Opposed();
	glm::vec4 glmResult = -glmVec4;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, AddVector4) {
	Vector4 result = vec4.AddVector4(vec4Add);
	glm::vec4 glmResult = glmVec4 + glmVec4Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, AddScalar) {
	Vector4 result = vec4.AddScalar(5.f);
	glm::vec4 glmResult = glmVec4 + 5.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, ElementWiseProduct)
{
	Vector4 result = vec4.ElementWiseProduct(vec4Add);
	glm::vec4 glmResult = glmVec4 * glmVec4Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, ProductScalar) {
	Vector4 result = vec4.ProductScalar(5.f);
	glm::vec4 glmResult = glmVec4 * 5.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, Mid) {
	Vector4 v1(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4 v2(10.0f, 10.0f, 10.0f, 10.0f);
	Vector4 mid = v1.Mid(v2);

	EXPECT_FLOAT_EQ(mid.X, 5.0f);
	EXPECT_FLOAT_EQ(mid.Y, 5.0f);
	EXPECT_FLOAT_EQ(mid.Z, 5.0f);
	EXPECT_FLOAT_EQ(mid.W, 5.0f);
}
TEST(Vector4Test, Distance) {
	float result = vec4.Distance(vec4Add);
	float glmResult = glm::length(glmVec4 - glmVec4Add);
	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector4Test, DistancePow) {
	float result = vec4.DistancePow(vec4Add);
	float glmResult = glm::length(glmVec4Add - glmVec4) * glm::length(glmVec4Add - glmVec4);
	EXPECT_TRUE(result - glmResult < 1e-4f); // EPSILON might be too small for squared distance, using a slightly larger threshold
}
TEST(Vector4Test, Norm) {
	float result = vec4.Norm();
	float glmResult = glm::length(glmVec4);
	EXPECT_TRUE(result - glmResult < EPSILON);
	Vector4 v2(0.0f, 0.0f, 0.0f, 0.0f);
	EXPECT_FLOAT_EQ(v2.Norm(), 0.0f);
}
TEST(Vector4Test, SquareNorm) {
	float result = vec4.SquareNorm();
	float glmResult = glm::length(glmVec4) * glm::length(glmVec4);
	EXPECT_TRUE(result - glmResult < 1e-4f); // EPSILON might be too small for squared norm, using a slightly larger threshold
}
TEST(Vector4Test, Normalized) {
	Vector4 result = vec4.Normalized();
	glm::vec4 glmResult = glm::normalize(glmVec4);
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, Perspective) {
	float perspective = vec4.Perspective(5.0f);
	float glmPerspective = (glmVec4.x * glmVec4.x + glmVec4.y * glmVec4.y + glmVec4.z * glmVec4.z + glmVec4.w * glmVec4.w) / 5.0f;
	EXPECT_TRUE(perspective - glmPerspective < EPSILON);
}
TEST(Vector4Test, DotProduct) {
	float result = vec4.DotProductVector4(vec4Add);
	float glmResult = glm::dot(glmVec4, glmVec4Add);
	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector4Test, OperatorPlus) {
	Vector4 result = vec4 + vec4Add;
	glm::vec4 glmResult = glmVec4 + glmVec4Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, OperatorMinus) {
	Vector4 result = vec4 - vec4Add;
	glm::vec4 glmResult = glmVec4 - glmVec4Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, OperatorMultiply) {
	Vector4 result = vec4 * vec4Add;
	glm::vec4 glmResult = glmVec4 * glmVec4Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, OperatorDivide) {
	Vector4 result = vec4 / vec4Add;
	glm::vec4 glmResult = glmVec4 / glmVec4Add;
	EXPECT_TRUE(result == glmResult);
	EXPECT_DEATH(vec4 / Vector4(), "Division by zero");
}
TEST(Vector4Test, OperatorUnaryMinus) {
	Vector4 result = -vec4;
	glm::vec4 glmResult = -glmVec4;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, OperatorPlusEqual) {
	Vector4 vec4Copy(vec4); // using _copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy += vec4;
	glmVec4Copy += glmVec4;

	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, OperatorMinusEqual) {
	Vector4 vec4Copy(vec4); // using _copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy -= vec4;
	glmVec4Copy -= glmVec4;

	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, OperatorMultiplyEqual) {
	Vector4 vec4Copy(vec4); // using _copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy *= vec4;
	glmVec4Copy *= glmVec4;

	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, OperatorDivideEqual) {
	Vector4 vec4Copy(vec4); // using _copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy /= vec4;
	glmVec4Copy /= glmVec4;

	EXPECT_TRUE(vec4Copy == glmVec4Copy);
	EXPECT_DEATH(vec4Copy / Vector4(), "Division by zero");
}
TEST(Vector4Test, OperatorSubscript) {
	Vector4 v(3.0f, 4.0f, 5.0f);

	EXPECT_FLOAT_EQ(v[0], 3.0f);
	EXPECT_FLOAT_EQ(v[1], 4.0f);
	EXPECT_FLOAT_EQ(v[2], 5.0f);

	v[0] = 6.0f;
	v[1] = 7.0f;
	v[2] = 8.0f;
	EXPECT_FLOAT_EQ(v.X, 6.0f);
	EXPECT_FLOAT_EQ(v.Y, 7.0f);
	EXPECT_FLOAT_EQ(v.Z, 8.0f);
}
TEST(Vector4Test, OperatorAddScalar)
{
	Vector4 result = vec4 + 3.f;
	glm::vec4 glmResult = glmVec4 + 3.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, OperatorMinusScalar)
{
	Vector4 result = vec4 - 3.f;
	glm::vec4 glmResult = glmVec4 - 3.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, OperatorMultiplyScalar)
{
	Vector4 result = vec4 * 3.f;
	glm::vec4 glmResult = glmVec4 * 3.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, OperatorDivideScalar)
{
	Vector4 result = vec4 / 3.f;
	glm::vec4 glmResult = glmVec4 / 3.f;
	EXPECT_TRUE(result == glmResult);

	EXPECT_DEATH(vec4 / 0.f, "Division by zero");
}
TEST(Vector4Test, OperatorMultiplyEqualScalar)
{
	Vector4 vec4Copy(vec4); // using _copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy *= 3.f;
	glmVec4Copy *= 3.f;
	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, OperatorDivideEqualScalar)
{
	Vector4 vec4Copy(vec4); // using _copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy /= 3.f;
	glmVec4Copy /= 3.f;
	EXPECT_TRUE(vec4Copy == glmVec4Copy);
	EXPECT_DEATH(vec4 /= 0.f, "Division by zero");
}
TEST(Vector4Test, OperatorAddEqualScalar)
{
	Vector4 vec4Copy(vec4); // using _copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy += 3.f;
	glmVec4Copy += 3.f;
	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, OperatorSubEqualScalar)
{
	Vector4 vec4Copy(vec4); // using _copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy -= 3.f;
	glmVec4Copy -= 3.f;
	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, AsList) {
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	std::array<float, 4> list = v.AsList();

	EXPECT_FLOAT_EQ(list[0], 1.0f);
	EXPECT_FLOAT_EQ(list[1], 2.0f);
	EXPECT_FLOAT_EQ(list[2], 3.0f);
	EXPECT_FLOAT_EQ(list[3], 4.0f);

	list[0] = 42.0f;
	EXPECT_FLOAT_EQ(v.X, 1.0f);
}
TEST(Vector4Test, EdgeCases) {
	Vector4 v1(1e6f, 1e6f, 1e6f, 1e6f);
	Vector4 v2(1e6f, 1e6f, 1e6f, 1e6f);
	float dot = v1.DotProductVector4(v2);
	EXPECT_FLOAT_EQ(dot, 4e12f);

	Vector4 v3(1e-6f, 1e-6f, 1e-6f, 1e-6f);
	Vector4 v4(1e-6f, 1e-6f, 1e-6f, 1e-6f);
	float distance = v3.Distance(v4);
	EXPECT_NEAR(distance, 0.0f, 1e-5f);
}
TEST(Vector4Test, Vector3Consistency) {
	Vector3 v3(2.0f, 3.0f, 4.0f);
	Vector4 v4(v3, 5.0f);
	Vector3 v3b = static_cast<Vector3>(v4);

	EXPECT_FLOAT_EQ(v3b.X, v3.X);
	EXPECT_FLOAT_EQ(v3b.Y, v3.Y);
	EXPECT_FLOAT_EQ(v3b.Z, v3.Z);
}