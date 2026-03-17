#include "pch.h"
#include <vector3.h>
using namespace MirageEngine::Core::Math;

inline static bool operator==(const Vector3& v1, const glm::vec3& v2)
{
	return std::abs(v1.x - v2.x) < EPSILON &&
		std::abs(v1.y - v2.y) < EPSILON &&
		std::abs(v1.z - v2.z) < EPSILON;
}

inline static bool operator==(const glm::vec3& v1, const Vector3& v2)
{
	return v2 == v1;
}

// -------------------------------------------------------------------------------- //	
//									TEST VECTOR3									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Vector3
/// </summary>

Vector3 vec3(3.5f, -2.f, 4.0f);
glm::vec3 glmVec3(3.5f, -2.f, 4.0f);

Vector3 vec3Add(1.f, 1.f, 1.f);
glm::vec3 glmVec3Add(1.f, 1.f, 1.f);

TEST(Vector3Test, Constructor)
{
	Vector3 v1;
	EXPECT_FLOAT_EQ(v1.x, 0.0f);
	EXPECT_FLOAT_EQ(v1.y, 0.0f);
	EXPECT_FLOAT_EQ(v1.z, 0.0f);

	Vector3 v2(1.5f, -2.3f, 4.7f);
	EXPECT_FLOAT_EQ(v2.x, 1.5f);
	EXPECT_FLOAT_EQ(v2.y, -2.3f);
	EXPECT_FLOAT_EQ(v2.z, 4.7f);

	Vector2 v2d(3.0f, 4.0f);
	Vector3 v3(v2d, 5.0f);
	EXPECT_FLOAT_EQ(v3.x, 3.0f);
	EXPECT_FLOAT_EQ(v3.y, 4.0f);
	EXPECT_FLOAT_EQ(v3.z, 5.0f);
}
TEST(Vector3Test, Opposed)
{
	Vector3 opposed = vec3.Opposed();
	glm::vec3 glmOpposed = -glmVec3;
	EXPECT_TRUE(opposed == glmOpposed);
}
TEST(Vector3Test, AddVector3) {
	Vector3 result = vec3.AddVector3(vec3Add);
	glm::vec3 glmResult = glmVec3 + glmVec3Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, AddScalar) {
	Vector3 result = vec3.AddScalar(5.f);
	glm::vec3 glmResult = glmVec3 + 5.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, ElementWiseProduct) {
	Vector3 result = vec3.ElementWiseProduct(vec3Add);
	glm::vec3 glmResult = glmVec3 * glmVec3Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, ProductScalar)
{
	Vector3 result = vec3.ProductScalar(5.f);
	glm::vec3 glmResult = glmVec3 * 5.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, NearlyEqual) {
	Vector3 v1(1.0f, 1.0f, 1.0f);
	Vector3 v2(1.0f + EPSILON * 0.5f, 1.0f - EPSILON * 0.5f, 1.0f + EPSILON * 0.5f);
	Vector3 v3(1.0f + EPSILON * 2.0f, 1.0f - EPSILON * 2.0f, 1.0f + EPSILON * 2.0f);
	EXPECT_TRUE(v1.NearlyEqual(v2));
	EXPECT_FALSE(v1.NearlyEqual(v3));
}
TEST(Vector3Test, Mid) {
	Vector3 v1(0.0f, 0.0f, 0.0f);
	Vector3 v2(10.0f, 10.0f, 10.0f);
	Vector3 mid = v1.Mid(v2);

	EXPECT_FLOAT_EQ(mid.x, 5.0f);
	EXPECT_FLOAT_EQ(mid.y, 5.0f);
	EXPECT_FLOAT_EQ(mid.z, 5.0f);
}
TEST(Vector3Test, Distance) {
	float result = vec3.Distance(vec3Add);
	float glmResult = glm::distance(glmVec3, glmVec3Add);
	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector3Test, DistancePow) {
	float result = vec3.DistancePow(vec3Add);
	float glmResult = glm::distance(glmVec3, glmVec3Add) * glm::distance(glmVec3, glmVec3Add);
	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector3Test, Norm)
{
	float result = vec3.Norm();
	float glmResult = glm::length(glmVec3);
	EXPECT_TRUE(result - glmResult < EPSILON);

	Vector3 v2(0.0f, 0.0f, 0.0f);
	EXPECT_FLOAT_EQ(v2.Norm(), 0.0f);
}
TEST(Vector3Test, SquareNorm)
{
	float result = vec3.SquareNorm();
	float glmResult = glm::length(glmVec3) * glm::length(glmVec3);
	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector3Test, Normalized) {
	Vector3 result = vec3.Normalized();
	glm::vec3 glmResult = glm::normalize(glmVec3);
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, Perspective) {
	float perspective = vec3.Perspective(5.0f);
	float glmPerspective = (glmVec3.x * glmVec3.x + glmVec3.y * glmVec3.y + glmVec3.z * glmVec3.z) / 5.0f;
	EXPECT_TRUE(perspective - glmPerspective < EPSILON);
}
TEST(Vector3Test, Reflect)
{
	Vector3 result = Vector3::Reflect(vec3.Normalized(), vec3Add.Normalized());
	glm::vec3 glmResult = glm::reflect(glm::normalize(glmVec3), glm::normalize(glmVec3Add));
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, DotProduct)
{
	float result = vec3.DotProductVector3(vec3Add);
	float glmResult = glm::dot(glmVec3, glmVec3Add);
	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector3Test, CrossProduct)
{
	Vector3 cross = vec3.CrossProductVector3(vec3Add);
	glm::vec3 glmCross = glm::cross(glmVec3, glmVec3Add);
	EXPECT_TRUE(cross == glmCross);
}
TEST(Vector3Test, Angle) {
	float result = vec3.Angle(vec3Add);
	float glmResult = glm::angle(glm::normalize(glmVec3), glm::normalize(glmVec3Add));
	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector3Test, RotateX) {
	Vector3 result = vec3.Rotate(PI / 2, 0, 0);
	glm::vec3 glmResult = glm::rotateX(glmVec3, PI / 2);
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, RotateY) {
	Vector3 result = vec3.Rotate(0, PI / 2, 0);
	glm::vec3 glmResult = glm::rotateY(glmVec3, PI / 2);
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, RotateZ) {
	Vector3 result = vec3.Rotate(0, 0, PI / 2);
	glm::vec3 glmResult = glm::rotateZ(glmVec3, PI / 2);
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, AsList)
{
	Vector3 v(1.0f, 2.0f, 3.0f);
	std::array<float, 3> list = v.AsList();

	EXPECT_FLOAT_EQ(list[0], 1.0f);
	EXPECT_FLOAT_EQ(list[1], 2.0f);
	EXPECT_FLOAT_EQ(list[2], 3.0f);

	list[0] = 42.0f;
	EXPECT_FLOAT_EQ(v.x, 1.0f);
}
TEST(Vector3Test, OperatorPlus) {
	Vector3 result = vec3 + vec3Add;
	glm::vec3 glmResult = glmVec3 + glmVec3Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, OperatorMinus) {
	Vector3 result = vec3 - vec3Add;
	glm::vec3 glmResult = glmVec3 - glmVec3Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, OperatorMultiply) {
	Vector3 result = vec3 * vec3Add;
	glm::vec3 glmResult = glmVec3 * glmVec3Add;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, OperatorDivide) {
	Vector3 result = vec3 / vec3Add;
	glm::vec3 glmResult = glmVec3 / glmVec3Add;
	EXPECT_TRUE(result == glmResult);
	EXPECT_DEATH(vec3 /= 0.f, "Division by zero");
}
TEST(Vector3Test, OperatorUnaryMinus) {
	Vector3 result = -vec3;
	glm::vec3 glmResult = -glmVec3;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, OperatorPlusEqual) {
	Vector3 vec3Copy(vec3); // using copy constructor
	glm::vec3 glmVec3Copy = glmVec3;

	vec3Copy += vec3;
	glmVec3Copy += glmVec3;

	EXPECT_TRUE(vec3Copy == glmVec3Copy);
}
TEST(Vector3Test, OperatorMinusEqual) {
	Vector3 vec3Copy(vec3); // using copy constructor
	glm::vec3 glmVec3Copy = glmVec3;

	vec3Copy -= vec3;
	glmVec3Copy -= glmVec3;

	EXPECT_TRUE(vec3Copy == glmVec3Copy);
}
TEST(Vector3Test, OperatorMultiplyEqual) {
	Vector3 vec3Copy(vec3); // using copy constructor
	glm::vec3 glmVec3Copy = glmVec3;

	vec3Copy *= vec3;
	glmVec3Copy *= glmVec3;

	EXPECT_TRUE(vec3Copy == glmVec3Copy);
}
TEST(Vector3Test, OperatorDivideEqual) {
	Vector3 vec3Copy(vec3); // using copy constructor
	glm::vec3 glmVec3Copy = glmVec3;

	vec3Copy /= vec3;
	glmVec3Copy /= glmVec3;

	EXPECT_TRUE(vec3Copy == glmVec3Copy);
	EXPECT_DEATH(vec3Copy /= 0.f, "Division by zero");
}
TEST(Vector3Test, OperatorSubscript) {
	Vector3 v(3.0f, 4.0f, 5.0f);

	EXPECT_FLOAT_EQ(v[0], 3.0f);
	EXPECT_FLOAT_EQ(v[1], 4.0f);
	EXPECT_FLOAT_EQ(v[2], 5.0f);

	v[0] = 6.0f;
	v[1] = 7.0f;
	v[2] = 8.0f;
	EXPECT_FLOAT_EQ(v.x, 6.0f);
	EXPECT_FLOAT_EQ(v.y, 7.0f);
	EXPECT_FLOAT_EQ(v.z, 8.0f);
}
TEST(Vector3Test, OperatorAddScalar)
{
	Vector3 result = vec3 + 3.f;
	glm::vec3 glmResult = glmVec3 + 3.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, OperatorMinusScalar)
{
	Vector3 result = vec3 - 3.f;
	glm::vec3 glmResult = glmVec3 - 3.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, OperatorMultiplyScalar)
{
	Vector3 result = vec3 * 3.f;
	glm::vec3 glmResult = glmVec3 * 3.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, OperatorDivideScalar)
{
	Vector3 result = vec3 / 3.f;
	glm::vec3 glmResult = glmVec3 / 3.f;
	EXPECT_TRUE(result == glmResult);

	EXPECT_DEATH(vec3 / 0.f, "Division by zero");
}
TEST(Vector3Test, OperatorMultiplyEqualScalar)
{
	Vector3 vec3Copy(vec3); // using copy constructor
	glm::vec3 glmVec3Copy = glmVec3;

	vec3Copy *= 3.f;
	glmVec3Copy *= 3.f;
	EXPECT_TRUE(vec3Copy == glmVec3Copy);
}
TEST(Vector3Test, OperatorDivideEqualScalar)
{
	Vector3 vec3Copy(vec3); // using copy constructor
	glm::vec3 glmVec3Copy = glmVec3;

	vec3Copy /= 3.f;
	glmVec3Copy /= 3.f;
	EXPECT_TRUE(vec3Copy == glmVec3Copy);
	EXPECT_DEATH(vec3 /= 0.f, "Division by zero");
}
TEST(Vector3Test, OperatorAddEqualScalar)
{
	Vector3 vec3Copy(vec3); // using copy constructor
	glm::vec3 glmVec3Copy = glmVec3;

	vec3Copy += 3.f;
	glmVec3Copy += 3.f;
	EXPECT_TRUE(vec3Copy == glmVec3Copy);
}
TEST(Vector3Test, OperatorSubEqualScalar)
{
	Vector3 vec3Copy(vec3); // using copy constructor
	glm::vec3 glmVec3Copy = glmVec3;

	vec3Copy -= 3.f;
	glmVec3Copy -= 3.f;
	EXPECT_TRUE(vec3Copy == glmVec3Copy);
}
TEST(Vector3Test, Equal)
{
	Vector3 v1(1.0f, 1.0f);
	Vector3 v2(1.0f, 1.0f);
	Vector3 v3(1.0f + EPSILON * 0.5f, 1.0f - EPSILON * 0.5f);
	EXPECT_TRUE(v1 == v2);
	EXPECT_FALSE(v1 == v3);
	EXPECT_TRUE(v1 != v3);
}
TEST(Vector3Test, operatorMultiply)
{
	Vector3 result = 2.f * Vector3(3.5f, -2.f, 4.0f);
	glm::vec3 glmResult = glmVec3 * 2.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector3Test, Print) {
	vec3.Print();
	std::cout << vec3 << std::endl;
}
TEST(Vector3Test, EdgeCases) {
	Vector3 v1(1e6f, 1e6f, 1e6f);
	Vector3 v2(1e6f, 1e6f, 1e6f);
	float dot = v1.DotProductVector3(v2);
	EXPECT_FLOAT_EQ(dot, 3e12f);

	Vector3 v3(1e-6f, 1e-6f, 1e-6f);
	Vector3 v4(1e-6f, 1e-6f, 1e-6f);
	float distance = v3.Distance(v4);
	EXPECT_NEAR(distance, 0.0f, 1e-5f);
}
TEST(Vector3Test, Vector2Conversion) {
	Vector2 v2(3.0f, 4.0f);
	Vector3 v3(v2, 5.0f);

	EXPECT_FLOAT_EQ(v3.x, v2.x);
	EXPECT_FLOAT_EQ(v3.y, v2.y);
	EXPECT_FLOAT_EQ(v3.z, 5.0f);

	Vector3 v3b(1.0f, 2.0f, 3.0f);
	Vector3 result = v3 + v3b;
	EXPECT_FLOAT_EQ(result.x, 4.0f);
	EXPECT_FLOAT_EQ(result.y, 6.0f);
	EXPECT_FLOAT_EQ(result.z, 8.0f);

	Vector2 v2FromV3 = static_cast<Vector2>(v3);
	EXPECT_TRUE(v2 == v2FromV3);
}