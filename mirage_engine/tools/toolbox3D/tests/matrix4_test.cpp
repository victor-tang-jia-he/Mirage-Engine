#include "pch.h"
#include <matrix4.h>
using namespace MirageEngine::Core::Math;

// Mat4
inline static bool operator==(const Matrix4& m1, const glm::mat4& m2)
{
	return
		std::abs(m1(0, 0) - m2[0][0]) < EPSILON &&
		std::abs(m1(0, 1) - m2[0][1]) < EPSILON &&
		std::abs(m1(0, 2) - m2[0][2]) < EPSILON &&
		std::abs(m1(0, 3) - m2[0][3]) < EPSILON &&
		std::abs(m1(1, 0) - m2[1][0]) < EPSILON &&
		std::abs(m1(1, 1) - m2[1][1]) < EPSILON &&
		std::abs(m1(1, 2) - m2[1][2]) < EPSILON &&
		std::abs(m1(1, 3) - m2[1][3]) < EPSILON &&
		std::abs(m1(2, 0) - m2[2][0]) < EPSILON &&
		std::abs(m1(2, 1) - m2[2][1]) < EPSILON &&
		std::abs(m1(2, 2) - m2[2][2]) < EPSILON &&
		std::abs(m1(2, 3) - m2[2][3]) < EPSILON;
}
inline static bool operator==(const glm::mat4& m1, const Matrix4& m2)
{
	return m2 == m1;
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