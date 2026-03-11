#include <gtest/gtest.h>
#include <Matrix4.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

// Mat3
inline static bool operator==(const Matrix2& mat2, const glm::mat2& mat)
{
	return std::abs(mat2.m[0][0] - mat[0][0]) < EPSILON &&
		std::abs(mat2.m[0][1] - mat[0][1]) < EPSILON &&
		std::abs(mat2.m[1][0] - mat[1][0]) < EPSILON &&
		std::abs(mat2.m[1][1] - mat[1][1]) < EPSILON;
}
inline static bool operator==(const glm::mat2& mat, const Matrix2& mat2)
{
	return mat2 == mat;
}

// Mat3
inline static bool operator==(const Matrix3& m1, const glm::mat3& m2)
{
	return
		std::abs(m1.m[0][0] - m2[0][0]) < EPSILON &&
		std::abs(m1.m[0][1] - m2[0][1]) < EPSILON &&
		std::abs(m1.m[0][2] - m2[0][2]) < EPSILON &&
		std::abs(m1.m[1][0] - m2[1][0]) < EPSILON &&
		std::abs(m1.m[1][1] - m2[1][1]) < EPSILON &&
		std::abs(m1.m[1][2] - m2[1][2]) < EPSILON &&
		std::abs(m1.m[2][0] - m2[2][0]) < EPSILON &&
		std::abs(m1.m[2][1] - m2[2][1]) < EPSILON &&
		std::abs(m1.m[2][2] - m2[2][2]) < EPSILON;
}
inline static bool operator==(const glm::mat3& m1, const Matrix3& m2)
{
	return m2 == m1;
}

// Mat4
inline static bool operator==(const Matrix4& m1, const glm::mat4& m2)
{
	return
		std::abs(m1.tab[0][0] - m2[0][0]) < EPSILON &&
		std::abs(m1.tab[0][1] - m2[0][1]) < EPSILON &&
		std::abs(m1.tab[0][2] - m2[0][2]) < EPSILON &&
		std::abs(m1.tab[0][3] - m2[0][3]) < EPSILON &&
		std::abs(m1.tab[1][0] - m2[1][0]) < EPSILON &&
		std::abs(m1.tab[1][1] - m2[1][1]) < EPSILON &&
		std::abs(m1.tab[1][2] - m2[1][2]) < EPSILON &&
		std::abs(m1.tab[1][3] - m2[1][3]) < EPSILON &&
		std::abs(m1.tab[2][0] - m2[2][0]) < EPSILON &&
		std::abs(m1.tab[2][1] - m2[2][1]) < EPSILON &&
		std::abs(m1.tab[2][2] - m2[2][2]) < EPSILON &&
		std::abs(m1.tab[2][3] - m2[2][3]) < EPSILON;
}
inline static bool operator==(const glm::mat4& m1, const Matrix4& m2)
{
	return m2 == m1;
}

//
//
////Mat4
//static void Matrix4Equal(const Matrix4& m1, const glm::mat4 m2, float epsilon = EPSILON)
//{
//	
//}
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
TEST(Matrix2Test, Rotation)
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
	Vector3 newVec = Mat4.Diagonal();
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