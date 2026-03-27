#include "pch.h"
#include <matrix3.h>
using namespace MirageEngine::Core::Math;

// Mat3
inline static bool operator==(const Matrix3& _m1, const glm::mat3& _m2)
{
	return
		std::abs(_m1(0, 0) - _m2[0][0]) < EPSILON &&
		std::abs(_m1(0, 1) - _m2[0][1]) < EPSILON &&
		std::abs(_m1(0, 2) - _m2[0][2]) < EPSILON &&
		std::abs(_m1(1, 0) - _m2[1][0]) < EPSILON &&
		std::abs(_m1(1, 1) - _m2[1][1]) < EPSILON &&
		std::abs(_m1(1, 2) - _m2[1][2]) < EPSILON &&
		std::abs(_m1(2, 0) - _m2[2][0]) < EPSILON &&
		std::abs(_m1(2, 1) - _m2[2][1]) < EPSILON &&
		std::abs(_m1(2, 2) - _m2[2][2]) < EPSILON;
}
inline static bool operator==(const glm::mat3& _m1, const Matrix3& _m2)
{
	return _m2 == _m1;
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
	EXPECT_TRUE((newVec.X - glmMat3[0][0]) < EPSILON);
	EXPECT_TRUE((newVec.Y - glmMat3[1][1]) < EPSILON);
	EXPECT_TRUE((newVec.Z - glmMat3[2][2]) < EPSILON);
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

	EXPECT_TRUE((myOut.X - glmOut.x) < EPSILON);
	EXPECT_TRUE((myOut.Y - glmOut.y) < EPSILON);
	EXPECT_TRUE((myOut.Z - glmOut.z) < EPSILON);
}
TEST(Matrix3Test, RotationY)
{
	Matrix3 myR = Matrix3::CreateYRotationMatrix(PI / 2);
	Vector3 myV(1, 0, 0);
	Vector3 myOut = myR * myV;

	glm::mat3 glmR = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0, 1, 0));
	glm::vec3 glmV(1, 0, 0);
	glm::vec3 glmOut = glmR * glmV;

	EXPECT_TRUE((myOut.X - glmOut.x) < EPSILON);
	EXPECT_TRUE((myOut.Y - glmOut.y) < EPSILON);
	EXPECT_TRUE((myOut.Z - glmOut.z) < EPSILON);
}
TEST(Matrix3Test, RotationZ)
{
	Matrix3 myR = Matrix3::CreateZRotationMatrix(PI / 2);
	Vector3 myV(1, 1, 0);
	Vector3 myOut = myR * myV;

	glm::mat3 glmR = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0, 0, 1));
	glm::vec3 glmV(1, 1, 0);
	glm::vec3 glmOut = glmR * glmV;

	EXPECT_TRUE((myOut.X - glmOut.x) < EPSILON);
	EXPECT_TRUE((myOut.Y - glmOut.y) < EPSILON);
	EXPECT_TRUE((myOut.Z - glmOut.z) < EPSILON);
}
TEST(Matrix3Test, Inverse)
{
	// Random _rotation
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

	for (int _col = 0; _col < 3; ++_col)
	{
		for (int _row = 0; _row < 3; ++_row)
		{
			float a = myInv(_col, _row);
			float b = glmInv[_col][_row];

			EXPECT_TRUE((a - b) < EPSILON);
		}
	}
}
TEST(Matrix3Test, LuDecomposition)
{
	Matrix3 A;
	A(0, 0) = 4; A(0, 1) = 3; A(0, 2) = 2;
	A(1, 0) = 3; A(1, 1) = 2; A(1, 2) = 1;
	A(2, 0) = 2; A(2, 1) = 1; A(2, 2) = 3;

	Matrix3 R = A.luDecomposition();

	EXPECT_TRUE(R == A);
}
TEST(Matrix3Test, RotationXYZVector)
{
	// Random _rotation
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