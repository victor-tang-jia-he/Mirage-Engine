#include "pch.h"
#include <vector2.h>
using namespace MirageEngine::Core::Math;

inline static bool operator==(const Vector2& _v1, const glm::vec2& _v2)
{
	return std::abs(_v1.X - _v2.x) < EPSILON &&
		std::abs(_v1.Y - _v2.y) < EPSILON;
}

inline static bool operator==(const glm::vec2& _v1, const Vector2& _v2)
{
	return _v2 == _v1;
}

// -------------------------------------------------------------------------------- //	
//									TEST VECTOR2									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Vector2
/// </summary>

Vector2 vec2(3.5f, -2.1f);
glm::vec2 glmVec2(3.5f, -2.1f);

Vector2 vec2Add(1.f, 1.f);
glm::vec2 glmVec2Add(1.f, 1.f);

TEST(Vector2Test, Constructor)
{
	Vector2 v1;
	EXPECT_FLOAT_EQ(v1.X, 0.0f);
	EXPECT_FLOAT_EQ(v1.Y, 0.0f);

	Vector2 v2(3.5f, -2.1f);
	EXPECT_FLOAT_EQ(v2.X, 3.5f);
	EXPECT_FLOAT_EQ(v2.Y, -2.1f);
}
TEST(Vector2Test, Opposed)
{
	Vector2 v = vec2.Opposed();
	glm::vec2 glmV = -glmVec2;

	EXPECT_TRUE(v == glmV);
}
TEST(Vector2Test, AddVector2)
{
	Vector2 result = vec2.AddVector2(vec2Add);
	glm::vec2 glmV1 = glmVec2 + glmVec2Add;

	EXPECT_TRUE(result == glmV1);
}
TEST(Vector2Test, AddScalar)
{
	Vector2 result = vec2.AddScalar(5.f);
	glm::vec2 glmV1 = glmVec2 + 5.f;

	EXPECT_TRUE(result == glmV1);
}
TEST(Vector2Test, ElementWiseProduct)
{
	Vector2 result = vec2.ElementWiseProduct(vec2Add);
	glm::vec2 glmResult = glmVec2 * glmVec2Add;

	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, ProductScalar)
{
	Vector2 result = vec2.ProductScalar(4.f);
	glm::vec2 glmResult = glmVec2 * 4.f;

	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, NearlyEqual)
{
	Vector2 v1(1.0f, 1.0f);
	Vector2 v2(1.0f + EPSILON * 0.5f, 1.0f - EPSILON * 0.5f);
	Vector2 v3(1.0f + EPSILON * 2.0f, 1.0f - EPSILON * 2.0f);
	EXPECT_TRUE(v1.NearlyEqual(v2));
	EXPECT_FALSE(v1.NearlyEqual(v3));
}
TEST(Vector2Test, Mid)
{
	Vector2 v1(0.0f, 0.0f);
	Vector2 v2(10.0f, 10.0f);
	Vector2 mid = v1.Mid(v2);

	EXPECT_FLOAT_EQ(mid.X, 5.0f);
	EXPECT_FLOAT_EQ(mid.Y, 5.0f);
}
TEST(Vector2Test, Distance) {
	float result = vec2.Distance(vec2Add);
	float glmResult = glm::distance(glmVec2, glmVec2Add);

	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector2Test, DistancePow)
{
	Vector2 v1(1.0f, 2.0f);
	Vector2 v2(4.0f, 6.0f);
	float distancePow = v1.DistancePow(v2);

	// Distance? = (4-1)? + (6-2)? = 9 + 16 = 25
	EXPECT_FLOAT_EQ(distancePow, 25.0f);
}
TEST(Vector2Test, Norm) {
	float result = vec2.Norm();
	float glmResult = glm::length(glmVec2);

	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector2Test, SquareNorm)
{
	float result = vec2.SquareNorm();
	float glmResult = glm::length(glmVec2) * glm::length(glmVec2);

	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector2Test, Normalized)
{
	Vector2 result = vec2.Normalized();
	glm::vec2 glmResult = glm::normalize(glmVec2);

	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, Perspective)
{
	float perspective = vec2.Perspective(5.0f);
	float glmPerspective = (glmVec2.x * glmVec2.x + glmVec2.y * glmVec2.y) / 5.0f;

	EXPECT_TRUE(perspective - glmPerspective < EPSILON);
}
TEST(Vector2Test, DotProduct)
{
	float dot = vec2.DotProductVector2(vec2Add);
	float glmDot = glm::dot(glmVec2, glmVec2Add);
	EXPECT_TRUE(dot - glmDot < EPSILON);

	Vector2 v1(1.0f, 0.0f);
	Vector2 v2(0.0f, 1.0f);
	EXPECT_FLOAT_EQ(v1.DotProductVector2(v2), 0.0f);
}
TEST(Vector2Test, CrossProduct) {
	float result = vec2.CrossProductVector2(vec2Add);
	float glmResult = glm::cross(glm::vec3(glmVec2, 0.0f), glm::vec3(glmVec2Add, 0.0f)).z;

	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector2Test, Angle)
{
	float result = vec2.Angle(vec2Add);
	float glmResult = glm::orientedAngle(glm::normalize(glmVec2), glm::normalize(glmVec2Add));

	EXPECT_TRUE(result - glmResult < EPSILON);
}
TEST(Vector2Test, Rotate)
{
	Vector2 result = vec2.Rotate(PI);
	glm::vec2 glmResult = glm::rotate(glmVec2, PI);
	EXPECT_TRUE(result == glmResult);

	result = vec2.Rotate(PI * 0.5f);
	glmResult = glm::rotate(glmVec2, PI * 0.5f);
	EXPECT_TRUE(result == glmResult);

	result = vec2.Rotate(PI * 0.5f);
	glmResult = glm::rotate(glmVec2, PI * 0.5f);
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, OperatorPlus)
{
	Vector2 result = vec2 + vec2Add;
	glm::vec2 glmResult = glmVec2 + glmVec2Add;

	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, OperatorMinus)
{
	Vector2 result = vec2 - vec2Add;
	glm::vec2 glmResult = glmVec2 - glmVec2Add;

	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, OperatorMultiply)
{
	Vector2 result = vec2 * vec2Add;
	glm::vec2 glmResult = glmVec2 * glmVec2Add;

	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, OperatorDivide)
{
	Vector2 result = vec2 / vec2Add;
	glm::vec2 glmResult = glmVec2 / glmVec2Add;

	EXPECT_TRUE(result == glmResult);

	Vector2 v3(0.0f, 0.0f);
	EXPECT_DEATH(vec2 / v3, "Division by zero");
}
TEST(Vector2Test, OperatorPlusEqual)
{
	Vector2 vec2Copy(vec2); // using _copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy += vec2;
	glmVec2Copy += glmVec2;

	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorMinusEqual)
{
	Vector2 vec2Copy(vec2); // using _copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy -= vec2;
	glmVec2Copy -= glmVec2;

	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorMultiplyEqual) {
	Vector2 vec2Copy(vec2); // using _copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy *= vec2;
	glmVec2Copy *= glmVec2;

	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorDivideEqual) {
	Vector2 vec2Copy(vec2); // using _copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy /= vec2;
	glmVec2Copy /= glmVec2;

	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorSubscript)
{
	Vector2 v(3.0f, 4.0f);

	EXPECT_FLOAT_EQ(v[0], 3.0f);
	EXPECT_FLOAT_EQ(v[1], 4.0f);

	v[0] = 5.0f;
	v[1] = 6.0f;
	EXPECT_FLOAT_EQ(v.X, 5.0f);
	EXPECT_FLOAT_EQ(v.Y, 6.0f);

	EXPECT_DEATH(v[2], "Index out of range");
}
TEST(Vector2Test, OperatorUnaryMinus)
{
	Vector2 result = -vec2;
	glm::vec2 glmResult = -glmVec2;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, OperatorAddScalar)
{
	Vector2 result = vec2 + 3.f;
	glm::vec2 glmResult = glmVec2 + 3.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, OperatorMinusScalar)
{
	Vector2 result = vec2 - 3.f;
	glm::vec2 glmResult = glmVec2 - 3.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, OperatorMultiplyScalar)
{
	Vector2 result = vec2 * 3.f;
	glm::vec2 glmResult = glmVec2 * 3.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, OperatorDivideScalar)
{
	Vector2 result = vec2 / 3.f;
	glm::vec2 glmResult = glmVec2 / 3.f;
	EXPECT_TRUE(result == glmResult);

	EXPECT_DEATH(vec2 / 0.f, "Division by zero");
}
TEST(Vector2Test, OperatorMultiplyEqualScalar)
{
	Vector2 vec2Copy(vec2); // using _copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy *= 3.f;
	glmVec2Copy *= 3.f;
	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorDivideEqualScalar)
{
	Vector2 vec2Copy(vec2); // using _copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy /= 3.f;
	glmVec2Copy /= 3.f;
	EXPECT_TRUE(vec2Copy == glmVec2Copy);
	EXPECT_DEATH(vec2 /= 0.f, "Division by zero");
}
TEST(Vector2Test, OperatorAddEqualScalar)
{
	Vector2 vec2Copy(vec2); // using _copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy += 3.f;
	glmVec2Copy += 3.f;
	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorSubEqualScalar)
{
	Vector2 vec2Copy(vec2); // using _copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy -= 3.f;
	glmVec2Copy -= 3.f;
	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, NegativeValues)
{
	Vector2 result = vec2 + (-vec2 * 2.f); // This should give us the same result as vec2 - (vec2 * 2.f) or vec2 * -1.f
	glm::vec2 glmResult = glmVec2 - (glmVec2 * 2.f);

	EXPECT_TRUE(result == glmResult);

	float resultNorm = result.Norm();
	float glmResultNorm = glm::length(glmResult);
	EXPECT_TRUE(resultNorm - glmResultNorm < EPSILON);
}
TEST(Vector2Test, Equal)
{
	Vector2 v1(1.0f, 1.0f);
	Vector2 v2(1.0f, 1.0f);
	Vector2 v3(1.0f + EPSILON * 0.5f, 1.0f - EPSILON * 0.5f);
	EXPECT_TRUE(v1 == v2);
	EXPECT_FALSE(v1 == v3);
	EXPECT_TRUE(v1 != v3);
}
TEST(Vector2Test, operatorMultiply)
{
	Vector2 result = 2.f * Vector2(3.5f, -2.1f);
	glm::vec2 glmResult = glmVec2 * 2.f;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector2Test, Print)
{
	// Just to check if the Print function works without crashing, not comparing output
	vec2.Print();
	std::cout << vec2 << std::endl;
}
TEST(Vector2Test, EdgeCases) {
	Vector2 v1(1e6f, 1e6f);
	Vector2 v2(1e6f, 1e6f);
	float dot = v1.DotProductVector2(v2);
	EXPECT_FLOAT_EQ(dot, 2e12f);

	Vector2 v3(1e-6f, 1e-6f);
	Vector2 v4(1e-6f, 1e-6f);
	float distance = v3.Distance(v4);
	EXPECT_NEAR(distance, 0.0f, 1e-5f);
}
TEST(UtilityTest, ToRad) {
	EXPECT_NEAR(ToRad(0.0f), 0.0f, EPSILON);
	EXPECT_NEAR(ToRad(90.0f), PI / 2, EPSILON);
	EXPECT_NEAR(ToRad(180.0f), PI, EPSILON);
	EXPECT_NEAR(ToRad(360.0f), 2 * PI, EPSILON);
	EXPECT_NEAR(ToRad(-90.0f), -PI / 2, EPSILON);
}