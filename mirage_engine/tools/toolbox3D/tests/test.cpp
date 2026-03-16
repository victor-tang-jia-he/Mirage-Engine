#include <gtest/gtest.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <matrix4.h>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
using namespace MirageEngine::Core::Math;
// TEST TEMPLATEx
//TEST(/*Name*/, /*Name*/)
//{
//	/*TYPE*/ m1(/*....*/), m2(/*....*/);
//	auto start = std::chrono::high_resolution_clock::now();
//	for (int i = 0; i < 10000; ++i) {
//		/*Function need to be tested*/
//	}
//	auto end = std::chrono::high_resolution_clock::now();
//	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//	std::cout << duration.count() << std::endl;
//	EXPECT_LT(duration.count(), /*Time in 祍*/); // 100000 祍 = 100 ms
//}

// -------------------------------------------------------------------------------- //	
// All Comparator & Define value
// -------------------------------------------------------------------------------- //

// Mat2
inline static bool operator==(const Matrix2& mat2, const glm::mat2& mat)
{
	return std::abs(mat2[0][0] - mat[0][0]) < EPSILON &&
		std::abs(mat2[0][1] - mat[0][1]) < EPSILON &&
		std::abs(mat2[1][0] - mat[1][0]) < EPSILON &&
		std::abs(mat2[1][1] - mat[1][1]) < EPSILON;
}
inline static bool operator==(const glm::mat2& mat, const Matrix2& mat2)
{
	return mat2 == mat;
}

// Mat3
inline static bool operator==(const Matrix3& m1, const glm::mat3& m2)
{
	return
		std::abs(m1[0][0] - m2[0][0]) < EPSILON &&
		std::abs(m1[0][1] - m2[0][1]) < EPSILON &&
		std::abs(m1[0][2] - m2[0][2]) < EPSILON &&
		std::abs(m1[1][0] - m2[1][0]) < EPSILON &&
		std::abs(m1[1][1] - m2[1][1]) < EPSILON &&
		std::abs(m1[1][2] - m2[1][2]) < EPSILON &&
		std::abs(m1[2][0] - m2[2][0]) < EPSILON &&
		std::abs(m1[2][1] - m2[2][1]) < EPSILON &&
		std::abs(m1[2][2] - m2[2][2]) < EPSILON;
}
inline static bool operator==(const glm::mat3& m1, const Matrix3& m2)
{
	return m2 == m1;
}

// Mat4
inline static bool operator==(const Matrix4& m1, const glm::mat4& m2)
{
	return
		std::abs(m1[0][0] - m2[0][0]) < EPSILON &&
		std::abs(m1[0][1] - m2[0][1]) < EPSILON &&
		std::abs(m1[0][2] - m2[0][2]) < EPSILON &&
		std::abs(m1[0][3] - m2[0][3]) < EPSILON &&
		std::abs(m1[1][0] - m2[1][0]) < EPSILON &&
		std::abs(m1[1][1] - m2[1][1]) < EPSILON &&
		std::abs(m1[1][2] - m2[1][2]) < EPSILON &&
		std::abs(m1[1][3] - m2[1][3]) < EPSILON &&
		std::abs(m1[2][0] - m2[2][0]) < EPSILON &&
		std::abs(m1[2][1] - m2[2][1]) < EPSILON &&
		std::abs(m1[2][2] - m2[2][2]) < EPSILON &&
		std::abs(m1[2][3] - m2[2][3]) < EPSILON;
}
inline static bool operator==(const glm::mat4& m1, const Matrix4& m2)
{
	return m2 == m1;
}

inline static bool operator==(const Vector2& v1, const glm::vec2& v2)
{
	return std::abs(v1.x - v2.x) < EPSILON &&
		std::abs(v1.y - v2.y) < EPSILON;
}

inline static bool operator==(const glm::vec2& v1, const Vector2& v2)
{
	return v2 == v1;
}

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

inline static bool operator==(const Vector4& v1, const glm::vec4& v2)
{
	return std::abs(v1.x - v2.x) < EPSILON &&
		std::abs(v1.y - v2.y) < EPSILON &&
		std::abs(v1.z - v2.z) < EPSILON &&
		std::abs(v1.w - v2.w) < EPSILON;
}

inline static bool operator==(const glm::vec4& v1, const Vector4& v2)
{
	return v2 == v1;
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
	EXPECT_FLOAT_EQ(v1.x, 0.0f);
	EXPECT_FLOAT_EQ(v1.y, 0.0f);

	Vector2 v2(3.5f, -2.1f);
	EXPECT_FLOAT_EQ(v2.x, 3.5f);
	EXPECT_FLOAT_EQ(v2.y, -2.1f);
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

	EXPECT_FLOAT_EQ(mid.x, 5.0f);
	EXPECT_FLOAT_EQ(mid.y, 5.0f);
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

	// Distance² = (4-1)² + (6-2)² = 9 + 16 = 25
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
	Vector2 vec2Copy(vec2); // using copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy += vec2;
	glmVec2Copy += glmVec2;

	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorMinusEqual)
{
	Vector2 vec2Copy(vec2); // using copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy -= vec2;
	glmVec2Copy -= glmVec2;

	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorMultiplyEqual) {
	Vector2 vec2Copy(vec2); // using copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy *= vec2;
	glmVec2Copy *= glmVec2;

	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorDivideEqual) {
	Vector2 vec2Copy(vec2); // using copy constructor
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
	EXPECT_FLOAT_EQ(v.x, 5.0f);
	EXPECT_FLOAT_EQ(v.y, 6.0f);

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
	Vector2 vec2Copy(vec2); // using copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy *= 3.f;
	glmVec2Copy *= 3.f;
	EXPECT_TRUE(vec2Copy == glmVec2Copy);
}
TEST(Vector2Test, OperatorDivideEqualScalar)
{
	Vector2 vec2Copy(vec2); // using copy constructor
	glm::vec2 glmVec2Copy = glmVec2;

	vec2Copy /= 3.f;
	glmVec2Copy /= 3.f;
	EXPECT_TRUE(vec2Copy == glmVec2Copy);
	EXPECT_DEATH(vec2 /= 0.f, "Division by zero");
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
	EXPECT_FLOAT_EQ(v1.x, 0.0f);
	EXPECT_FLOAT_EQ(v1.y, 0.0f);
	EXPECT_FLOAT_EQ(v1.z, 0.0f);
	EXPECT_FLOAT_EQ(v1.w, 1.0f); 

	Vector4 v2(1.5f, -2.3f, 4.7f, 2.0f);
	EXPECT_FLOAT_EQ(v2.x, 1.5f);
	EXPECT_FLOAT_EQ(v2.y, -2.3f);
	EXPECT_FLOAT_EQ(v2.z, 4.7f);
	EXPECT_FLOAT_EQ(v2.w, 2.0f);

	Vector4 v3(1.0f, 2.0f, 3.0f);
	EXPECT_FLOAT_EQ(v3.x, 1.0f);
	EXPECT_FLOAT_EQ(v3.y, 2.0f);
	EXPECT_FLOAT_EQ(v3.z, 3.0f);
	EXPECT_FLOAT_EQ(v3.w, 1.0f);
}
TEST(Vector4Test, ConstructorFromVector3) {
	Vector3 v3(2.0f, 3.0f, 4.0f);
	Vector4 v4(v3);

	EXPECT_FLOAT_EQ(v4.x, 2.0f);
	EXPECT_FLOAT_EQ(v4.y, 3.0f);
	EXPECT_FLOAT_EQ(v4.z, 4.0f);
	EXPECT_FLOAT_EQ(v4.w, 1.0f);

	Vector4 v4b(v3, 5.0f);
	EXPECT_FLOAT_EQ(v4b.x, 2.0f);
	EXPECT_FLOAT_EQ(v4b.y, 3.0f);
	EXPECT_FLOAT_EQ(v4b.z, 4.0f);
	EXPECT_FLOAT_EQ(v4b.w, 5.0f);
}
TEST(Vector4Test, ConversionToVector3) {
	Vector4 v4(1.0f, 2.0f, 3.0f, 4.0f);
	Vector3 v3 = static_cast<Vector3>(v4);

	EXPECT_FLOAT_EQ(v3.x, 1.0f);
	EXPECT_FLOAT_EQ(v3.y, 2.0f);
	EXPECT_FLOAT_EQ(v3.z, 3.0f);

	Vector2 v2 = static_cast<Vector2>(v4);
	EXPECT_FLOAT_EQ(v2.x, 1.0f);
	EXPECT_FLOAT_EQ(v2.y, 2.0f);
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

	EXPECT_FLOAT_EQ(mid.x, 5.0f);
	EXPECT_FLOAT_EQ(mid.y, 5.0f);
	EXPECT_FLOAT_EQ(mid.z, 5.0f);
	EXPECT_FLOAT_EQ(mid.w, 5.0f);
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
	EXPECT_ANY_THROW(vec4 / Vector4(0.0f, 0.0f, 0.0f));
}
TEST(Vector4Test, OperatorUnaryMinus) {
	Vector4 result = -vec4;
	glm::vec4 glmResult = -glmVec4;
	EXPECT_TRUE(result == glmResult);
}
TEST(Vector4Test, OperatorPlusEqual) {
	Vector4 vec4Copy(vec4); // using copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy += vec4;
	glmVec4Copy += glmVec4;

	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, OperatorMinusEqual) {
	Vector4 vec4Copy(vec4); // using copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy -= vec4;
	glmVec4Copy -= glmVec4;

	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, OperatorMultiplyEqual) {
	Vector4 vec4Copy(vec4); // using copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy *= vec4;
	glmVec4Copy *= glmVec4;

	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, OperatorDivideEqual) {
	Vector4 vec4Copy(vec4); // using copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy /= vec4;
	glmVec4Copy /= glmVec4;

	EXPECT_TRUE(vec4Copy == glmVec4Copy);
	EXPECT_ANY_THROW(vec4Copy /= Vector4());
}
TEST(Vector4Test, OperatorSubscript) {
	Vector4 v(3.0f, 4.0f, 5.0f);

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
	Vector4 vec4Copy(vec4); // using copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy *= 3.f;
	glmVec4Copy *= 3.f;
	EXPECT_TRUE(vec4Copy == glmVec4Copy);
}
TEST(Vector4Test, OperatorDivideEqualScalar)
{
	Vector4 vec4Copy(vec4); // using copy constructor
	glm::vec4 glmVec4Copy = glmVec4;

	vec4Copy /= 3.f;
	glmVec4Copy /= 3.f;
	EXPECT_TRUE(vec4Copy == glmVec4Copy);
	EXPECT_DEATH(vec4 /= 0.f, "Division by zero");
}
TEST(Vector4Test, AsList) {
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	std::array<float, 4> list = v.AsList();

	EXPECT_FLOAT_EQ(list[0], 1.0f);
	EXPECT_FLOAT_EQ(list[1], 2.0f);
	EXPECT_FLOAT_EQ(list[2], 3.0f);
	EXPECT_FLOAT_EQ(list[3], 4.0f);

	list[0] = 42.0f;
	EXPECT_FLOAT_EQ(v.x, 1.0f);
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

	EXPECT_FLOAT_EQ(v3b.x, v3.x);
	EXPECT_FLOAT_EQ(v3b.y, v3.y);
	EXPECT_FLOAT_EQ(v3b.z, v3.z);
}

// -------------------------------------------------------------------------------- //	
//									TEST MATRIX2									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Matrix2
/// </summary>

Matrix2 Mat2(1.f, 3.f, 2.f, 4.f);
glm::mat2 glmMat2(1.f, 3.f, 2.f, 4.f);

TEST(Matrix2Test, Identity)
{
	Matrix2 newMat = Matrix2::Identity();
	glm::mat2 newGlmMat = glm::identity<glm::mat2>();
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix2Test, Equal)
{
	Matrix2 newMat(Mat2);
	EXPECT_TRUE(Mat2 == newMat);
	EXPECT_TRUE(Mat2 == glmMat2);
}
TEST(Matrix2Test, Diagonal)
{
	Vector2 v = Mat2.Diagonal();

	EXPECT_FLOAT_EQ(v.x, 1.f);
	EXPECT_FLOAT_EQ(v.y, 4.f);
}
TEST(Matrix2Test, Opposed)
{
	Matrix2 opposed = Mat2.Opposed();

	EXPECT_TRUE(opposed == -glmMat2);
}
TEST(Matrix2Test, Transpose)
{
	Matrix2 transposed = Mat2.Transpose();
	glm::mat2 transposedGlm = glm::transpose(glmMat2);
	EXPECT_TRUE(transposed == transposedGlm);
}
TEST(Matrix2Test, Addition)
{
	Matrix2 amat = Mat2.AddMatrix2(Mat2);
	glm::mat2 aglm = glmMat2 + glmMat2;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix2Test, ProductScalar)
{
	Matrix2 Smat = Mat2.ProductScalar(10.f);
	glm::mat2 SmatGlm = glmMat2 * 10.f;

	EXPECT_TRUE(Smat == SmatGlm);
}
TEST(Matrix2Test, ProductMatrix2)
{
	Matrix2 Pmat = Mat2.ProductMatrix2(Mat2);
	glm::mat2 PmatGlm = glmMat2 * glmMat2;

	EXPECT_TRUE(Pmat == PmatGlm);
}
TEST(Matrix2Test, Trace)
{
	float trace = Mat2.Trace();
	float traceGlm = glmMat2[0][0] + glmMat2[1][1];
	EXPECT_TRUE((trace - traceGlm) < EPSILON);
}
TEST(Matrix2Test, Determinant)
{
	float newMat = Mat2.Determinant();
	float newGlmMat = glm::determinant(glmMat2);

	EXPECT_TRUE((newMat - newGlmMat) < EPSILON);
}
TEST(Matrix2Test, Inverse)
{
	Matrix2 inv = Mat2.Inverse();
	glm::mat2 glmInv = glm::inverse(glmMat2);

	EXPECT_TRUE(inv == glmInv);
}
TEST(Matrix2Test, Rotate)
{
	Matrix2 inv = Mat2.Inverse();
}
TEST(Matrix2Test, AddOperator)
{
	Matrix2 amat = Mat2 + Mat2;
	glm::mat2 aglm = glmMat2 + glmMat2;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix2Test, SubOperator)
{
	Matrix2 amat = Mat2 - Mat2;
	glm::mat2 aglm = glmMat2 - glmMat2;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix2Test, ProductOperator)
{
	Matrix2 amat = Mat2 * Mat2;
	glm::mat2 aglm = glmMat2 * glmMat2;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix2Test, DivOperator)
{
	Matrix2 amat = Mat2 / Mat2;
	glm::mat2 aglm = glmMat2 / glmMat2;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix2Test, AddEqualOperator)
{
	Matrix2 amat = Mat2;
	amat += amat;
	glm::mat2 aglm = glmMat2 + glmMat2;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix2Test, SubEqualOperator)
{
	Matrix2 amat = Mat2;
	amat -= amat;
	glm::mat2 aglm = glmMat2 - glmMat2;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix2Test, DivEqualOperator)
{
	Matrix2 amat = Mat2;
	amat /= amat;
	glm::mat2 aglm = glmMat2 / glmMat2;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix2Test, ProductEqualOperator)
{
	Matrix2 amat = Mat2;
	amat *= amat;
	glm::mat2 aglm = glmMat2 * glmMat2;

	EXPECT_TRUE(amat == aglm);
}


// -------------------------------------------------------------------------------- //	
//									TEST MATRIX3									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Matrix3
/// </summary>

Matrix3 Mat3(
	4, 3, 2,
	3, 4, 3,
	2, 3, 4
);

glm::mat3 glmMat3(
	4, 3, 2,
	3, 4, 3,
	2, 3, 4
);

TEST(Matrix3Test, Identity)
{
	Matrix3 newMat = Matrix3::Identity();
	glm::mat3 newGlmMat = glm::identity<glm::mat3>();
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix3Test, Equal)
{
	Matrix3 newMat(Mat3);
	EXPECT_TRUE(Mat3 == glmMat3);
	EXPECT_TRUE(Mat3 == newMat);
}
TEST(Matrix3Test, Diagonal)
{
	Vector3 newVec = Mat3.Diagonal();
	EXPECT_TRUE((newVec.x - glmMat3[0][0]) < EPSILON);
	EXPECT_TRUE((newVec.y - glmMat3[1][1]) < EPSILON);
	EXPECT_TRUE((newVec.z - glmMat3[2][2]) < EPSILON);
}
TEST(Matrix3Test, Oppose)
{
	Matrix3 newMat = Mat3.Opposed();
	glm::mat3 newGlmMat = -glmMat3;
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix3Test, Transpose)
{
	Matrix3 newMat = Mat3.Transpose();
	glm::mat3 newGlmMat = glm::transpose(glmMat3);
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix3Test, Addition)
{
	Matrix3 newMat = Mat3.AddMatrix3(Mat3);
	glm::mat3 newGlmMat = glmMat3 + glmMat3;
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix3Test, ProductScalar)
{
	Matrix3 newMat = Mat3.ProductScalar(10.f);
	glm::mat3 newGlmMat = glmMat3 * 10.f;
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix3Test, ProductMatrix3)
{
	Matrix3 newMat = Mat3.ProductMatrix3(Mat3);
	glm::mat3 newGlmMat = glmMat3 * glmMat3;
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix3Test, ToArray)
{
	std::array<float, 9> array = {
		4, 3, 2,
		3, 4, 3,
		2, 3, 4 };
	std::array<float, 9> newMat = Mat3.ToArray();
	EXPECT_TRUE(newMat == array);
}
TEST(Matrix3Test, Determinant)
{
	float newMat = Mat3.Determinant();
	float newGlmMat = glm::determinant(glmMat3);
	EXPECT_TRUE((newMat - newGlmMat) < EPSILON);
}
TEST(Matrix3Test, SubMatrix)
{
	Matrix2 newMat = Mat3.GetSubMatrix(2, 2);
	Matrix2 mat2(4.f, 3.f, 3.f, 4.f);
	EXPECT_TRUE(newMat == mat2);
}
TEST(Matrix3Test, RotationX)
{
	Matrix3 myR = Matrix3::CreateXRotationMatrix(PI / 2);
	Vector3 myV(1, 0, 0);
	Vector3 myOut = myR * myV;

	glm::mat3 glmR = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(1, 0, 0));
	glm::vec3 glmV(1, 0, 0);
	glm::vec3 glmOut = glmR * glmV;

	EXPECT_TRUE((myOut.x - glmOut.x) < EPSILON);
	EXPECT_TRUE((myOut.y - glmOut.y) < EPSILON);
	EXPECT_TRUE((myOut.z - glmOut.z) < EPSILON);
}
TEST(Matrix3Test, RotationY)
{
	Matrix3 myR = Matrix3::CreateYRotationMatrix(PI / 2);
	Vector3 myV(1, 0, 0);
	Vector3 myOut = myR * myV;

	glm::mat3 glmR = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0, 1, 0));
	glm::vec3 glmV(1, 0, 0);
	glm::vec3 glmOut = glmR * glmV;

	EXPECT_TRUE((myOut.x - glmOut.x) < EPSILON);
	EXPECT_TRUE((myOut.y - glmOut.y) < EPSILON);
	EXPECT_TRUE((myOut.z - glmOut.z) < EPSILON);
}
TEST(Matrix3Test, RotationZ)
{
	Matrix3 myR = Matrix3::CreateZRotationMatrix(PI / 2);
	Vector3 myV(1, 1, 0);
	Vector3 myOut = myR * myV;

	glm::mat3 glmR = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0, 0, 1));
	glm::vec3 glmV(1, 1, 0);
	glm::vec3 glmOut = glmR * glmV;

	EXPECT_TRUE((myOut.x - glmOut.x) < EPSILON);
	EXPECT_TRUE((myOut.y - glmOut.y) < EPSILON);
	EXPECT_TRUE((myOut.z - glmOut.z) < EPSILON);
}
TEST(Matrix3Test, Inverse)
{
	// Random rotation
	float rx = 0.37f;
	float ry = -1.12f;
	float rz = 0.58f;

	Matrix3 myM = Matrix3::RotateMatrix(rx, ry, rz);
	Matrix3 myInv = myM.Inverse();

	glm::mat4 g(1.0f);
	g = glm::rotate(g, rz, glm::vec3(0, 0, 1));
	g = glm::rotate(g, ry, glm::vec3(0, 1, 0));
	g = glm::rotate(g, rx, glm::vec3(1, 0, 0));

	glm::mat3 glmM(g);
	glm::mat3 glmInv = glm::inverse(glmM);

	for (int col = 0; col < 3; ++col)
	{
		for (int row = 0; row < 3; ++row)
		{
			float a = myInv.m[col][row];
			float b = glmInv[col][row];

			EXPECT_TRUE((a - b) < EPSILON);
		}
	}
}
TEST(Matrix3Test, LuDecomposition)
{
	Matrix3 A;
	A.m[0][0] = 4; A.m[0][1] = 3; A.m[0][2] = 2;
	A.m[1][0] = 3; A.m[1][1] = 2; A.m[1][2] = 1;
	A.m[2][0] = 2; A.m[2][1] = 1; A.m[2][2] = 3;

	Matrix3 R = A.luDecomposition();

	EXPECT_TRUE(R == A);
}
TEST(Matrix3Test, RotationXYZVector)
{
	// Random rotation
	float rx = 0.37f;
	float ry = -1.12f;
	float rz = 0.58f;

	Matrix3 mat3(1.0f);
	Matrix3 matZ = mat3.RotateMatrixVector(Vector3(0, 0, 1), rz);
	Matrix3 matY = mat3.RotateMatrixVector(Vector3(0, 1, 0), ry);
	Matrix3 matX = mat3.RotateMatrixVector(Vector3(1, 0, 0), rx);

	glm::mat4 g(1.0f);
	glm::mat3 gZ = glm::rotate(g, rz, glm::vec3(0, 0, 1));
	glm::mat3 gY = glm::rotate(g, ry, glm::vec3(0, 1, 0));
	glm::mat3 gX = glm::rotate(g, rx, glm::vec3(1, 0, 0));

	EXPECT_TRUE(matX == gX);
	EXPECT_TRUE(matY == gY);
	EXPECT_TRUE(matZ == gZ);
}
TEST(Matrix3Test, AddOperator)
{
	Matrix3 amat = Mat3 + Mat3;
	glm::mat3 aglm = glmMat3 + glmMat3;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix3Test, SubOperator)
{
	Matrix3 amat = Mat3 - Mat3;
	glm::mat3 aglm = glmMat3 - glmMat3;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix3Test, ProductOperator)
{
	Matrix3 amat = Mat3 * Mat3;
	glm::mat3 aglm = glmMat3 * glmMat3;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix3Test, DivOperator)
{
	Matrix3 amat = Mat3 / Mat3;
	glm::mat3 aglm = glmMat3 / glmMat3;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix3Test, AddEqualOperator)
{
	Matrix3 amat = Mat3;
	amat += amat;
	glm::mat3 aglm = glmMat3 + glmMat3;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix3Test, SubEqualOperator)
{
	Matrix3 amat = Mat3;
	amat -= amat;
	glm::mat3 aglm = glmMat3 - glmMat3;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix3Test, DivEqualOperator)
{
	Matrix3 amat = Mat3;
	amat /= amat;
	glm::mat3 aglm = glmMat3 / glmMat3;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix3Test, ProductEqualOperator)
{
	Matrix3 amat = Mat3;
	amat *= amat;
	glm::mat3 aglm = glmMat3 * glmMat3;

	EXPECT_TRUE(amat == aglm);
}

// -------------------------------------------------------------------------------- //	
//									TEST MATRIX4									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Matrix4
/// </summary>

Matrix4 Mat4(1, 0, 2, 4, 0, 1, 3, 2, 2, -1, 0, 3, 4, -1, 3, 9);
glm::mat4 glmMat4(1, 0, 2, 4, 0, 1, 3, 2, 2, -1, 0, 3, 4, -1, 3, 9);

TEST(Matrix4Test, Identity)
{
	Matrix4 newMat = Matrix4::Identity();
	glm::mat3 newGlmMat = glm::identity<glm::mat3>();
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix4Test, Equal)
{
	Matrix4 newMat(Mat4);
	EXPECT_TRUE(Mat4 == glmMat4);
	EXPECT_TRUE(Mat4 == newMat);
}
TEST(Matrix4Test, Diagonal)
{
	Vector3 newVec(Mat4.Diagonal());
	EXPECT_TRUE((newVec.x - glmMat4[0][0]) < EPSILON);
	EXPECT_TRUE((newVec.y - glmMat4[1][1]) < EPSILON);
	EXPECT_TRUE((newVec.z - glmMat4[2][2]) < EPSILON);
}
TEST(Matrix4Test, Oppose)
{
	Matrix4 newMat = Mat4.Opposed();
	glm::mat4 newGlmMat = -glmMat4;
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix4Test, Transpose)
{
	Matrix4 newMat = Mat4.Transpose();
	glm::mat4 newGlmMat = glm::transpose(glmMat4);
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix4Test, Addition)
{
	Matrix4 newMat = Mat4.AddMatrix4(Mat4);
	glm::mat4 newGlmMat = glmMat4 + glmMat4;
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix4Test, ProductScalar)
{
	Matrix4 newMat = Mat4.ProductScalar(10.f);
	glm::mat4 newGlmMat = glmMat4 * 10.f;
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix4Test, ProductMatrix4)
{
	Matrix4 newMat = Mat4.ProductMatrix4(Mat4);
	glm::mat4 newGlmMat = glmMat4 * glmMat4;
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix4Test, ToArray)
{
	std::array<float, 16> array = { 1,0,2,4,0,1,3,2,2,-1,0,3,4,-1,3,9 };
	std::array<float, 16> newMat = Mat4.ToArray();
	EXPECT_TRUE(newMat == array);
}
TEST(Matrix4Test, Determinant)
{
	float newMat = Mat4.Determinant();
	float newGlmMat = glm::determinant(glmMat4);
	EXPECT_TRUE((newMat - newGlmMat) < EPSILON);
}
TEST(Matrix4Test, Inverse)
{
	Matrix4 newMat = Mat4.Inverse();
	glm::mat4 newGlmMat = glm::inverse(glmMat4);
	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix4Test, AddOperator)
{
	Matrix4 amat = Mat4 + Mat4;
	glm::mat4 aglm = glmMat4 + glmMat4;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix4Test, SubOperator)
{
	Matrix4 amat = Mat4 - Mat4;
	glm::mat4 aglm = glmMat4 - glmMat4;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix4Test, ProductOperator)
{
	Matrix4 amat = Mat4 * Mat4;
	glm::mat4 aglm = glmMat4 * glmMat4;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix4Test, DivOperator)
{
	Matrix4 amat = Mat4 / Mat4;
	glm::mat4 aglm = glmMat4 / glmMat4;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix4Test, AddEqualOperator)
{
	Matrix4 amat = Mat4;
	amat += amat;
	glm::mat4 aglm = glmMat4 + glmMat4;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix4Test, SubEqualOperator)
{
	Matrix4 amat = Mat4;
	amat -= amat;
	glm::mat4 aglm = glmMat4 - glmMat4;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix4Test, DivEqualOperator)
{
	Matrix4 amat = Mat4;
	amat /= amat;
	glm::mat4 aglm = glmMat4 / glmMat4;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix4Test, ProductEqualOperator)
{
	Matrix4 amat = Mat4;
	amat *= amat;
	glm::mat4 aglm = glmMat4 * glmMat4;

	EXPECT_TRUE(amat == aglm);
}
TEST(Matrix4Test, Perspective)
{
	Matrix4 newMat = Matrix4::GetPerspectiveMatrix(1920, 1080, 0.1f, 100.f, 60.f);

	glm::mat4 newGlmMat = glm::perspective(
		glm::radians(60.f),
		1920.f / 1080.f,
		0.1f,
		100.f
	);

	EXPECT_TRUE(newMat == newGlmMat);
}
TEST(Matrix4Test, PerspectiveVsGLM) {
	Matrix4 myMat = Matrix4::GetPerspectiveMatrix(1920, 1080, 0.1f, 100.f, 60.f);
	glm::mat4 glmMat = glm::perspective(glm::radians(60.f), 1920.f / 1080.f, 0.1f, 100.f);
	EXPECT_TRUE(myMat == glmMat);
}
TEST(Matrix4Test, UltimateRowMajorTest) {
	// Matrice de rotation non-symétrique
	Matrix4 myRot = Matrix4::CreateZRotationMatrix(PI / 3).Transpose();

	// Même rotation en GLM
	glm::mat4 glmRot = glm::rotate(glm::mat4(1.0f), PI / 3, glm::vec3(0, 0, 1));

	// Si ça passe, c'est que tout est cohérent
	EXPECT_TRUE(myRot == glmRot);

	// Test avec un vecteur
	Vector4 myVec(1, 0, 0, 1);
	glm::vec4 glmVec(1, 0, 0, 1);

	Vector4 myTransformed = myRot * myVec;
	glm::vec4 glmTransformed = glmRot * glmVec;

	EXPECT_NEAR(myTransformed.x, glmTransformed.x, EPSILON);
	EXPECT_NEAR(myTransformed.y, glmTransformed.y, EPSILON);
}