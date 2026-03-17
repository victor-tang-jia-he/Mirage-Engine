#include "pch.h"
#include <matrix2.h>
using namespace MirageEngine::Core::Math;
// Mat2
inline static bool operator==(const Matrix2& mat2, const glm::mat2& mat)
{
	return
		std::abs(mat2(0, 0) - mat[0][0]) < EPSILON &&
		std::abs(mat2(0, 1) - mat[0][1]) < EPSILON &&
		std::abs(mat2(1, 0) - mat[1][0]) < EPSILON &&
		std::abs(mat2(1, 1) - mat[1][1]) < EPSILON;
}
inline static bool operator==(const glm::mat2& mat, const Matrix2& mat2)
{
	return mat2 == mat;
}



// -------------------------------------------------------------------------------- //	
//									TEST MATRIX2									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Matrix2
/// </summary>

Matrix2 Mat2(
	1.f, 3.f,
	2.f, 4.f);
glm::mat2 glmMat2(
	1.f, 3.f,
	2.f, 4.f);
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

