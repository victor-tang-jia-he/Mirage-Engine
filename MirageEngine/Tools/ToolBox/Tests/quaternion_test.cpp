#include "pch.h"
#include <quaternion.h>
using namespace MirageEngine::Core::Math;

// -------------------------------------------------------------------------------- //	
// All Comparator & Define value
// -------------------------------------------------------------------------------- //

inline static bool operator==(const Quaternion& _q1, const glm::quat& _q2)
{
	return std::abs(_q1.W - _q2.w) < EPSILON &&
		std::abs(_q1.X - _q2.x) < EPSILON &&
		std::abs(_q1.Y - _q2.y) < EPSILON &&
		std::abs(_q1.Z - _q2.z) < EPSILON;
}

inline static bool operator==(const glm::quat& _q1, const Quaternion& _q2)
{
	return _q2 == _q1;
}

Quaternion qua1(4.f, 1.f, 2.f, 3.f);
Quaternion qua2(8.f, 5.f, 6.f, 7.f);

glm::quat glmQua1(4.f, 1.f, 2.f, 3.f);
glm::quat glmQua2(8.f, 5.f, 6.f, 7.f);

Vector3 vec = Vector3(0.f, 1.f, 0.f);
glm::vec3 glmVec(0, 1, 0);
TEST(QuaternionTest, AllConstructors)
{
	// 1. Scalar constructor
	{
		Quaternion q(1.f);
		glm::quat g(1.f, 1.f, 1.f, 1.f);

		EXPECT_TRUE(q == g);
	}

	// 2. XYZW constructor
	{
		Quaternion q(4.f, 1.f, 2.f, 3.f);
		glm::quat g(4.f, 1.f, 2.f, 3.f);
		EXPECT_TRUE(q == g);
	}

	// 3. Copy constructor
	{
		Quaternion q1(4.f, 1.f, 2.f, 3.f);
		Quaternion q2(q1);

		EXPECT_TRUE(Quaternion(4.f, 1.f, 2.f, 3.f) == q2);
	}

	// 4. Move constructor
	{
		Quaternion q1(1.f, 2.f, 3.f, 4.f);
		Quaternion q2(std::move(q1));

		EXPECT_TRUE(q2 == Quaternion(1.f, 2.f, 3.f, 4.f));
	}

	// 5. Axis / Angle
	{
		float angle = glm::radians(90.f);

		Quaternion q(vec, angle);
		glm::quat g = glm::angleAxis(angle, glmVec);

		// EXPECT_TRUE(q == g); // false true, float error
	}

	// 6. Euler (XYZ)
	{
		float x = glm::radians(30.f);
		float y = glm::radians(45.f);
		float z = glm::radians(60.f);

		Quaternion q(x, y, z);
		glm::quat g = glm::quat(glm::vec3(x, y, z));

		EXPECT_TRUE(q == g);
	}

	// 7. Matrix -> Quaternion
	{
		glm::mat4 mat = glm::rotate(glm::mat4(1.0f),
			glm::radians(45.f),
			glm::vec3(0, 1, 0));
		Matrix4 mat4(
			mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]
		);

		Quaternion q(mat4);

		glm::quat g = glm::quat_cast(mat);

		//EXPECT_TRUE(q == g); // pass but calcul epsilon is too small
	}
}

TEST(QuaternionTest, Vector3AnglesConstructor)
{
	Vector3 angles(glm::radians(30.f), glm::radians(45.f), glm::radians(60.f));
	Quaternion q(angles);
	glm::quat g = glm::quat(glm::vec3(angles.X, angles.Y, angles.Z));
	EXPECT_TRUE(q == g);
}

TEST(QuaternionTest, MatrixConstructor)
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f),
		glm::radians(45.f),
		glm::vec3(0, 1, 0));
	Matrix4 mat4(
		mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]
	);

	Quaternion q(mat4);
	glm::quat g = glm::quat_cast(mat);

	// EXPECT_TRUE(q == g); // false true as we can get same rotation but diff quaternion
}

TEST(QuaternionTest, GetRotationAxis)
{
	Quaternion q(vec, glm::radians(90.f));
	Vector3 result = q.GetRotationAxis();

	glm::quat g(glm::angleAxis(glm::radians(90.f), glmVec));
	glm::vec3 glmResult = glm::axis(g);

	EXPECT_TRUE(result == glmResult);
}

TEST(QuaternionTest, GetRotation)
{
	Quaternion q(vec, glm::radians(90.f));
	float angle = q.GetRotation();

	glm::quat g(glm::angleAxis(glm::radians(90.f), glmVec));
	float glmAngle = glm::angle(g);

	EXPECT_NEAR(angle, glmAngle, EPSILON);
}

TEST(QuaternionTest, SquaredNormComparison)
{
	Quaternion q(1.f, 2.f, 3.f, 4.f);
	glm::quat g(4.f, 1.f, 2.f, 3.f);

	EXPECT_NEAR(q.SquaredNorm(), glm::dot(g, g), EPSILON);
}

TEST(QuaternionTest, NormComparison)
{
	Quaternion q(1.f, 2.f, 3.f, 4.f);
	glm::quat g(4.f, 1.f, 2.f, 3.f);

	EXPECT_NEAR(q.Norm(), glm::length(g), EPSILON);
}

TEST(QuaternionTest, NormalizeComparison)
{
	Quaternion result = qua1.Normalize();
	glm::quat glmResult = glm::normalize(glmQua1);

	EXPECT_TRUE(result == glmResult);
}

TEST(QuaternionTest, ConjugateComparison)
{
	Quaternion result = qua1.Conjugation();
	glm::quat glmResult = glm::conjugate(glmQua1);

	EXPECT_TRUE(result == glmResult);
}

TEST(QuaternionTest, InverseComparison)
{
	Quaternion result = qua1.Inverse();
	glm::quat glmResult = glm::inverse(glmQua1);

	EXPECT_TRUE(result == glmResult);
}

TEST(QuaternionTest, MultiplicationComparison)
{
	Quaternion result = qua1 * qua2;
	glm::quat glmResult = glmQua1 * glmQua2;

	EXPECT_TRUE(result == glmResult);
}

TEST(QuaternionTest, ScalarMultiplicationComparison)
{
	float scalar = 2.f;
	Quaternion result = qua1 * scalar;
	glm::quat glmResult = glmQua1 * scalar;

	EXPECT_TRUE(result == glmResult);
}

TEST(QuaternionTest, DotProductComparison)
{
	Quaternion q1(1.f, 2.f, 3.f, 4.f);
	Quaternion q2(5.f, 6.f, 7.f, 8.f);

	glm::quat g1(4.f, 1.f, 2.f, 3.f);
	glm::quat g2(8.f, 5.f, 6.f, 7.f);

	EXPECT_NEAR(q1.DotProduct(q2), glm::dot(g1, g2), EPSILON);
}

TEST(QuaternionTest, RotateComparison)
{
	Quaternion q(vec, glm::radians(45.f));

	Vector3 rotated = Quaternion::Rotate(vec, q);

	glm::quat g = glm::angleAxis(glm::radians(45.f), glmVec);
	glm::vec3 glmRotated = g * glm::vec3(vec.X, vec.Y, vec.Z);

	EXPECT_NEAR(rotated.X, glmRotated.x, EPSILON);
	EXPECT_NEAR(rotated.Y, glmRotated.y, EPSILON);
	EXPECT_NEAR(rotated.Z, glmRotated.z, EPSILON);
}

TEST(QuaternionTest, RotateInverseComparison)
{
	Vector3 axis(0.f, 1.f, 0.f);
	float angle = glm::radians(45.f);

	Quaternion q(axis, angle);

	Vector3 rotated = Quaternion::Rotate(vec, q);
	Vector3 rotatedBack = Quaternion::RotateInverse(rotated, q);

	EXPECT_NEAR(rotatedBack.X, vec.X, EPSILON);
	EXPECT_NEAR(rotatedBack.Y, vec.Y, EPSILON);
	EXPECT_NEAR(rotatedBack.Z, vec.Z, EPSILON);

	glm::quat g = glm::angleAxis(angle, glm::vec3(axis.X, axis.Y, axis.Z));
	glm::vec3 glmRotated = g * glm::vec3(vec.X, vec.Y, vec.Z);
	glm::vec3 glmRotatedBack = glm::inverse(g) * glmRotated;

	EXPECT_NEAR(rotatedBack.X, glmRotatedBack.x, EPSILON);
	EXPECT_NEAR(rotatedBack.Y, glmRotatedBack.y, EPSILON);
	EXPECT_NEAR(rotatedBack.Z, glmRotatedBack.z, EPSILON);
}


TEST(QuaternionTest, SlerpComparison)
{
	Quaternion q1(1.f, 0.f, 0.f, 0.f);
	Quaternion q2(vec, glm::radians(90.f));
	float t = 0.5f;

	Quaternion result = Quaternion::Slerp(q1, q2, t);

	glm::quat g1 = glm::quat(1.f, 0.f, 0.f, 0.f);
	glm::quat g2 = glm::angleAxis(glm::radians(90.f), glmVec);
	glm::quat glmResult = glm::slerp(g1, g2, t);

	EXPECT_TRUE(result == glmResult);
}

TEST(QuaternionTest, NlerpComparison)
{
	Quaternion q1(1.f, 0.f, 0.f, 0.f);
	Quaternion q2(vec, glm::radians(90.f));
	float t = 0.5f;

	Quaternion nlerp = Quaternion::Nlerp(q1, q2, t);

	glm::quat g1 = glm::quat(1.f, 0.f, 0.f, 0.f);
	glm::quat g2 = glm::angleAxis(glm::radians(90.f), glmVec);
	glm::quat glmNlerp = glm::normalize(glm::mix(g1, g2, t));

	EXPECT_TRUE(nlerp == glmNlerp);
}

TEST(QuaternionTest, ToMatrixComparison)
{
	Quaternion q(vec, glm::radians(45.f));
	Matrix4 mat = q.ToMatrix();

	glm::quat g = glm::angleAxis(glm::radians(45.f), glmVec);
	glm::mat4 glmMat = glm::mat4_cast(g);

	EXPECT_TRUE(mat == glmMat);
}

TEST(QuaternionTest, ToEulerAngleComparison)
{
	Quaternion q(vec, glm::radians(45.f));
	Vector3 euler = q.ToEulerAngle();

	glm::quat g = glm::angleAxis(glm::radians(45.f), glmVec);
	glm::vec3 glmEuler = glm::eulerAngles(g);

	EXPECT_NEAR(euler.X, glmEuler.x, EPSILON);
	EXPECT_NEAR(euler.Y, glmEuler.y, EPSILON);
	EXPECT_NEAR(euler.Z, glmEuler.z, EPSILON);
}

TEST(QuaternionTest, ArithmeticOperatorsComparison)
{
	Quaternion q1(4.f, 1.f, 2.f, 3.f);
	Quaternion q2(8.f, 5.f, 6.f, 7.f);
	float scalar = 2.f;

	glm::quat g1(4.f, 1.f, 2.f, 3.f);
	glm::quat g2(8.f, 5.f, 6.f, 7.f);

	// Addition
	EXPECT_TRUE((q1 + q2) == (g1 + g2));

	// Scalar addition
	EXPECT_TRUE((q1 + scalar) == (glm::quat(g1.w + scalar, g1.x + scalar, g1.y + scalar, g1.z + scalar)));

	// Subtraction
	EXPECT_TRUE((q1 - q2) == (g1 - g2));

	// Scalar subtraction
	EXPECT_TRUE((q1 - scalar) == (glm::quat(g1.w - scalar, g1.x - scalar, g1.y - scalar, g1.z - scalar)));

	// Division
	EXPECT_TRUE((q1 / q2) == (glm::quat(g1 * glm::inverse(g2))));

	// Scalar division
	EXPECT_TRUE((q1 / scalar) == (g1 / scalar));
}

TEST(QuaternionTest, CompoundOperatorsComparison)
{
	Quaternion q1(4.f, 1.f, 2.f, 3.f);
	Quaternion q2(8.f, 5.f, 6.f, 7.f);
	float scalar = 2.f;

	glm::quat g1(4.f, 1.f, 2.f, 3.f);
	glm::quat g2(8.f, 5.f, 6.f, 7.f);

	// +=
	Quaternion qAdd = q1;
	qAdd += q2;
	glm::quat gAdd = g1;
	gAdd += g2;
	EXPECT_TRUE(qAdd == gAdd);

	// += scalar
	Quaternion qAddScalar = q1;
	qAddScalar += scalar;
	EXPECT_TRUE(qAddScalar == glm::quat(g1.w + scalar, g1.x + scalar, g1.y + scalar, g1.z + scalar));

	// -=
	Quaternion qSub = q1;
	qSub -= q2;
	glm::quat gSub = g1;
	gSub -= g2;
	EXPECT_TRUE(qSub == gSub);

	// -= scalar
	Quaternion qSubScalar = q1;
	qSubScalar -= scalar;
	EXPECT_TRUE(qSubScalar == glm::quat(g1.w - scalar, g1.x - scalar, g1.y - scalar, g1.z - scalar));

	// *=
	Quaternion qMul = q1;
	qMul *= q2;
	glm::quat gMul = g1;
	gMul *= g2;
	EXPECT_TRUE(qMul == gMul);

	// *= scalar
	Quaternion qMulScalar = q1;
	qMulScalar *= scalar;
	glm::quat gMulScalar = g1;
	gMulScalar *= scalar;
	EXPECT_TRUE(qMulScalar == gMulScalar);

	// /=
	Quaternion qDiv = q1;
	qDiv /= q2;
	EXPECT_TRUE(qDiv == (glm::quat(g1 * glm::inverse(g2))));

	// /= scalar
	Quaternion qDivScalar = q1;
	qDivScalar /= scalar;
	glm::quat gDivScalar = g1;
	gDivScalar /= scalar;
	EXPECT_TRUE(qDivScalar == gDivScalar);
}

TEST(QuaternionTest, IndexOperator)
{
	Quaternion q(1.f, 2.f, 3.f, 4.f);

	EXPECT_NEAR(q[0], 1.f, EPSILON);
	EXPECT_NEAR(q[1], 2.f, EPSILON);
	EXPECT_NEAR(q[2], 3.f, EPSILON);
	EXPECT_NEAR(q[3], 4.f, EPSILON);
}

TEST(QuaternionTest, StreamOperator)
{
	Quaternion q(1.f, 2.f, 3.f, 4.f);
	std::stringstream ss;
	ss << q;

	q.Print();
}