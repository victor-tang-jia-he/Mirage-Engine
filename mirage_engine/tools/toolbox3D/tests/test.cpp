#include <gtest/gtest.h>
#include <matrix4.h>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace MirageEngine::Core::Maths;
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

// -------------------------------------------------------------------------------- //	
//									TEST VECTOR2									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Vector2
/// </summary>

// Test du constructeur et des valeurs par défaut
TEST(Vector2Test, Constructor) {
	Vector2 v1;
	EXPECT_FLOAT_EQ(v1.x, 0.0f);
	EXPECT_FLOAT_EQ(v1.y, 0.0f);

	Vector2 v2(3.5f, -2.1f);
	EXPECT_FLOAT_EQ(v2.x, 3.5f);
	EXPECT_FLOAT_EQ(v2.y, -2.1f);
}
// Test des opérations élémentaires
TEST(Vector2Test, GetOpposed) {
	Vector2 v(3.0f, -4.0f);
	Vector2 opposed = v.GetOpposed();

	EXPECT_FLOAT_EQ(opposed.x, -3.0f);
	EXPECT_FLOAT_EQ(opposed.y, 4.0f);
}
TEST(Vector2Test, AddVector2) {
	Vector2 v1(1.0f, 2.0f);
	Vector2 v2(3.0f, 4.0f);
	Vector2 result = v1.AddVector2(v2);

	EXPECT_FLOAT_EQ(result.x, 4.0f);
	EXPECT_FLOAT_EQ(result.y, 6.0f);
}
TEST(Vector2Test, AddScalar) {
	Vector2 v(1.0f, 2.0f);
	Vector2 result = v.AddScalar(5.0f);

	EXPECT_FLOAT_EQ(result.x, 6.0f);
	EXPECT_FLOAT_EQ(result.y, 7.0f);
}
TEST(Vector2Test, ElementWiseProduct) {
	Vector2 v1(2.0f, 3.0f);
	Vector2 v2(4.0f, 5.0f);
	Vector2 result = v1.ElementWiseProduct(v2);

	EXPECT_FLOAT_EQ(result.x, 8.0f);
	EXPECT_FLOAT_EQ(result.y, 15.0f);
}
TEST(Vector2Test, ProductScalar) {
	Vector2 v(2.0f, 3.0f);
	Vector2 result = v.ProductScalar(4.0f);

	EXPECT_FLOAT_EQ(result.x, 8.0f);
	EXPECT_FLOAT_EQ(result.y, 12.0f);
}
// Tests géométriques
TEST(Vector2Test, GetMid) {
	Vector2 v1(0.0f, 0.0f);
	Vector2 v2(10.0f, 10.0f);
	Vector2 mid = v1.GetMid(v2);

	EXPECT_FLOAT_EQ(mid.x, 5.0f);
	EXPECT_FLOAT_EQ(mid.y, 5.0f);
}
TEST(Vector2Test, GetDistance) {
	Vector2 v1(1.0f, 2.0f);
	Vector2 v2(4.0f, 6.0f);
	float distance = v1.GetDistance(v2);

	// Distance = sqrt((4-1)² + (6-2)²) = sqrt(9 + 16) = sqrt(25) = 5
	EXPECT_FLOAT_EQ(distance, 5.0f);
}
TEST(Vector2Test, GetDistancePow) {
	Vector2 v1(1.0f, 2.0f);
	Vector2 v2(4.0f, 6.0f);
	float distancePow = v1.GetDistancePow(v2);

	// Distance² = (4-1)² + (6-2)² = 9 + 16 = 25
	EXPECT_FLOAT_EQ(distancePow, 25.0f);
}
TEST(Vector2Test, GetNorm) {
	Vector2 v(3.0f, 4.0f);
	float norm = v.GetNorm();

	// Norme = sqrt(3² + 4²) = sqrt(9 + 16) = sqrt(25) = 5
	EXPECT_FLOAT_EQ(norm, 5.0f);

	Vector2 v2(0.0f, 0.0f);
	EXPECT_FLOAT_EQ(v2.GetNorm(), 0.0f);
}
TEST(Vector2Test, GetSquareNorm) {
	Vector2 v(3.0f, 4.0f);
	float squareNorm = v.GetSquareNorm();

	EXPECT_FLOAT_EQ(squareNorm, 25.0f);
}
TEST(Vector2Test, Normalized) {
	Vector2 v(3.0f, 4.0f);
	Vector2 normalized = v.Normalized();

	// Vecteur unitaire = (3/5, 4/5) = (0.6, 0.8)
	EXPECT_FLOAT_EQ(normalized.x, 0.6f);
	EXPECT_FLOAT_EQ(normalized.y, 0.8f);
	EXPECT_NEAR(normalized.GetNorm(), 1.0f, EPSILON);

	// Test avec vecteur nul (devrait retourner (0,0) ou gérer l'erreur)
	Vector2 vZero(0.0f, 0.0f);
	Vector2 normZero = vZero.Normalized();
	EXPECT_FLOAT_EQ(normZero.x, 0.0f);
	EXPECT_FLOAT_EQ(normZero.y, 0.0f);
}
TEST(Vector2Test, GetPerspective) {
	Vector2 v(3.0f, 4.0f);
	float perspective = v.GetPerspective(5.0f);

	// (3² + 4²) / 5 = (9 + 16) / 5 = 25 / 5 = 5
	EXPECT_FLOAT_EQ(perspective, 5.0f);

	// Test avec t <= 0
	EXPECT_FLOAT_EQ(v.GetPerspective(0.0f), 0.0f);
	EXPECT_FLOAT_EQ(v.GetPerspective(-1.0f), 0.0f);
}
// Tests des produits
TEST(Vector2Test, DotProduct) {
	Vector2 v1(2.0f, 3.0f);
	Vector2 v2(4.0f, 5.0f);
	float dot = v1.DotProductVector2(v2);

	// 2*4 + 3*5 = 8 + 15 = 23
	EXPECT_FLOAT_EQ(dot, 23.0f);

	// Test avec vecteurs orthogonaux
	Vector2 v3(1.0f, 0.0f);
	Vector2 v4(0.0f, 1.0f);
	EXPECT_FLOAT_EQ(v3.DotProductVector2(v4), 0.0f);
}
TEST(Vector2Test, CrossProduct) {
	Vector2 v1(2.0f, 3.0f);
	Vector2 v2(4.0f, 5.0f);
	float cross = v1.CrossProductVector2(v2);

	// En 2D, cross = x1*y2 - y1*x2 = 2*5 - 3*4 = 10 - 12 = -2
	EXPECT_FLOAT_EQ(cross, -2.0f);
}
TEST(Vector2Test, GetAngle) {
	Vector2 v1(1.0f, 0.0f);
	Vector2 v2(0.0f, 1.0f);
	float angle = v1.GetAngle(v2);

	// Angle entre (1,0) et (0,1) = 90 degrés
	EXPECT_NEAR(angle, 90.0f, 0.001f);

	// Test avec vecteurs parallèles
	Vector2 v3(2.0f, 0.0f);
	EXPECT_NEAR(v1.GetAngle(v3), 0.0f, 0.001f);

	// Test avec vecteurs opposés
	Vector2 v4(-1.0f, 0.0f);
	EXPECT_NEAR(v1.GetAngle(v4), 180.0f, 0.001f);
}
// Test de rotation
TEST(Vector2Test, Rotation) {
	Vector2 v(1.0f, 0.0f);

	// Rotation de 90 degrés
	v.Rotation(90.0f);
	EXPECT_NEAR(v.x, 0.0f, EPSILON);
	EXPECT_NEAR(v.y, 1.0f, EPSILON);

	// Rotation de 180 degrés depuis la position actuelle
	v.Rotation(90.0f);  // Maintenant à 180 degrés au total
	EXPECT_NEAR(v.x, -1.0f, EPSILON);
	EXPECT_NEAR(v.y, 0.0f, EPSILON);

	// Test avec angle négatif
	Vector2 v2(1.0f, 0.0f);
	v2.Rotation(-90.0f);
	EXPECT_NEAR(v2.x, 0.0f, EPSILON);
	EXPECT_NEAR(v2.y, -1.0f, EPSILON);
}
// Tests des opérateurs
TEST(Vector2Test, OperatorPlus) {
	Vector2 v1(1.0f, 2.0f);
	Vector2 v2(3.0f, 4.0f);
	Vector2 result = v1 + v2;

	EXPECT_FLOAT_EQ(result.x, 4.0f);
	EXPECT_FLOAT_EQ(result.y, 6.0f);
}
TEST(Vector2Test, OperatorMinus) {
	Vector2 v1(5.0f, 7.0f);
	Vector2 v2(2.0f, 3.0f);
	Vector2 result = v1 - v2;

	EXPECT_FLOAT_EQ(result.x, 3.0f);
	EXPECT_FLOAT_EQ(result.y, 4.0f);
}
TEST(Vector2Test, OperatorMultiply) {
	Vector2 v1(2.0f, 3.0f);
	Vector2 v2(4.0f, 5.0f);
	Vector2 result = v1 * v2;

	EXPECT_FLOAT_EQ(result.x, 8.0f);
	EXPECT_FLOAT_EQ(result.y, 15.0f);
}
TEST(Vector2Test, OperatorDivide) {
	Vector2 v1(8.0f, 15.0f);
	Vector2 v2(2.0f, 3.0f);
	Vector2 result = v1 / v2;

	EXPECT_FLOAT_EQ(result.x, 4.0f);
	EXPECT_FLOAT_EQ(result.y, 5.0f);

	// Test division par zéro (doit assert)
	// Vector2 v3(1.0f, 0.0f);
	// EXPECT_DEATH(v1 / v3, "Division by zero");
}
TEST(Vector2Test, OperatorPlusEqual) {
	Vector2 v1(1.0f, 2.0f);
	Vector2 v2(3.0f, 4.0f);
	v1 += v2;

	EXPECT_FLOAT_EQ(v1.x, 4.0f);
	EXPECT_FLOAT_EQ(v1.y, 6.0f);
}
TEST(Vector2Test, OperatorMinusEqual) {
	Vector2 v1(5.0f, 7.0f);
	Vector2 v2(2.0f, 3.0f);
	v1 -= v2;

	EXPECT_FLOAT_EQ(v1.x, 3.0f);
	EXPECT_FLOAT_EQ(v1.y, 4.0f);
}
TEST(Vector2Test, OperatorMultiplyEqual) {
	Vector2 v1(2.0f, 3.0f);
	Vector2 v2(4.0f, 5.0f);
	v1 *= v2;

	EXPECT_FLOAT_EQ(v1.x, 8.0f);
	EXPECT_FLOAT_EQ(v1.y, 15.0f);
}
TEST(Vector2Test, OperatorDivideEqual) {
	Vector2 v1(8.0f, 15.0f);
	Vector2 v2(2.0f, 3.0f);
	v1 /= v2;

	EXPECT_FLOAT_EQ(v1.x, 4.0f);
	EXPECT_FLOAT_EQ(v1.y, 5.0f);
}
TEST(Vector2Test, OperatorSubscript) {
	Vector2 v(3.0f, 4.0f);

	EXPECT_FLOAT_EQ(v[0], 3.0f);
	EXPECT_FLOAT_EQ(v[1], 4.0f);

	// Modification
	v[0] = 5.0f;
	v[1] = 6.0f;
	EXPECT_FLOAT_EQ(v.x, 5.0f);
	EXPECT_FLOAT_EQ(v.y, 6.0f);

	// Test hors limites (doit assert en debug)
	// EXPECT_DEATH(v[2], "Index out of range");
}
// Test avec valeurs négatives
TEST(Vector2Test, NegativeValues) {
	Vector2 v1(-3.0f, -4.0f);
	Vector2 v2(1.0f, 2.0f);

	Vector2 sum = v1 + v2;
	EXPECT_FLOAT_EQ(sum.x, -2.0f);
	EXPECT_FLOAT_EQ(sum.y, -2.0f);

	float norm = v1.GetNorm();
	EXPECT_FLOAT_EQ(norm, 5.0f);  // La norme reste positive

	Vector2 normalized = v1.Normalized();
	EXPECT_NEAR(normalized.x, -0.6f, EPSILON);
	EXPECT_NEAR(normalized.y, -0.8f, EPSILON);
}
// Test des cas limites
TEST(Vector2Test, EdgeCases) {
	// Très grandes valeurs
	Vector2 v1(1e6f, 1e6f);
	Vector2 v2(1e6f, 1e6f);
	float dot = v1.DotProductVector2(v2);
	EXPECT_FLOAT_EQ(dot, 2e12f);

	// Très petites valeurs
	Vector2 v3(1e-6f, 1e-6f);
	Vector2 v4(1e-6f, 1e-6f);
	float distance = v3.GetDistance(v4);
	EXPECT_NEAR(distance, 0.0f, 1e-5f);
}
// Test de la fonction ToRad
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

// Test du constructeur et des valeurs par défaut
TEST(Vector3Test, Constructor) {
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
// Test des opérations élémentaires
TEST(Vector3Test, GetOpposed) {
	Vector3 v(3.0f, -4.0f, 5.0f);
	Vector3 opposed = v.GetOpposed();

	EXPECT_FLOAT_EQ(opposed.x, -3.0f);
	EXPECT_FLOAT_EQ(opposed.y, 4.0f);
	EXPECT_FLOAT_EQ(opposed.z, -5.0f);
}
TEST(Vector3Test, AddVector3) {
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(4.0f, 5.0f, 6.0f);
	Vector3 result = v1.AddVector3(v2);

	EXPECT_FLOAT_EQ(result.x, 5.0f);
	EXPECT_FLOAT_EQ(result.y, 7.0f);
	EXPECT_FLOAT_EQ(result.z, 9.0f);
}
TEST(Vector3Test, AddScalar) {
	Vector3 v(1.0f, 2.0f, 3.0f);
	Vector3 result = v.AddScalar(5.0f);

	EXPECT_FLOAT_EQ(result.x, 6.0f);
	EXPECT_FLOAT_EQ(result.y, 7.0f);
	EXPECT_FLOAT_EQ(result.z, 8.0f);
}
TEST(Vector3Test, ElementWiseProduct) {
	Vector3 v1(2.0f, 3.0f, 4.0f);
	Vector3 v2(5.0f, 6.0f, 7.0f);
	Vector3 result = v1.ElementWiseProduct(v2);

	EXPECT_FLOAT_EQ(result.x, 10.0f);
	EXPECT_FLOAT_EQ(result.y, 18.0f);
	EXPECT_FLOAT_EQ(result.z, 28.0f);
}
TEST(Vector3Test, ProductScalar) {
	Vector3 v(2.0f, 3.0f, 4.0f);
	Vector3 result = v.ProductScalar(3.0f);

	EXPECT_FLOAT_EQ(result.x, 6.0f);
	EXPECT_FLOAT_EQ(result.y, 9.0f);
	EXPECT_FLOAT_EQ(result.z, 12.0f);
}
// Tests géométriques
TEST(Vector3Test, GetMid) {
	Vector3 v1(0.0f, 0.0f, 0.0f);
	Vector3 v2(10.0f, 10.0f, 10.0f);
	Vector3 mid = v1.GetMid(v2);

	EXPECT_FLOAT_EQ(mid.x, 5.0f);
	EXPECT_FLOAT_EQ(mid.y, 5.0f);
	EXPECT_FLOAT_EQ(mid.z, 5.0f);
}
TEST(Vector3Test, GetDistance) {
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(4.0f, 6.0f, 8.0f);
	float distance = v1.GetDistance(v2);

	// Distance = sqrt((4-1)² + (6-2)² + (8-3)²) = sqrt(9 + 16 + 25) = sqrt(50) ≈ 7.071
	EXPECT_NEAR(distance, 7.0710678f, 0.0001f);
}
TEST(Vector3Test, GetDistancePow) {
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(4.0f, 6.0f, 8.0f);
	float distancePow = v1.GetDistancePow(v2);

	// Distance² = (4-1)² + (6-2)² + (8-3)² = 9 + 16 + 25 = 50
	EXPECT_FLOAT_EQ(distancePow, 50.0f);
}
TEST(Vector3Test, GetNorm) {
	Vector3 v(3.0f, 4.0f, 12.0f);
	float norm = v.GetNorm();

	// Norme = sqrt(3² + 4² + 12²) = sqrt(9 + 16 + 144) = sqrt(169) = 13
	EXPECT_FLOAT_EQ(norm, 13.0f);

	Vector3 v2(0.0f, 0.0f, 0.0f);
	EXPECT_FLOAT_EQ(v2.GetNorm(), 0.0f);
}
TEST(Vector3Test, GetSquareNorm) {
	Vector3 v(3.0f, 4.0f, 12.0f);
	float squareNorm = v.GetSquareNorm();

	EXPECT_FLOAT_EQ(squareNorm, 169.0f);
}
TEST(Vector3Test, Normalized) {
	Vector3 v(3.0f, 4.0f, 12.0f);
	Vector3 normalized = v.Normalized();

	// Vecteur unitaire = (3/13, 4/13, 12/13) ≈ (0.230769, 0.307692, 0.923077)
	EXPECT_NEAR(normalized.x, 3.0f / 13.0f, EPSILON);
	EXPECT_NEAR(normalized.y, 4.0f / 13.0f, EPSILON);
	EXPECT_NEAR(normalized.z, 12.0f / 13.0f, EPSILON);
	EXPECT_NEAR(normalized.GetNorm(), 1.0f, EPSILON);

	// Test avec vecteur nul
	Vector3 vZero(0.0f, 0.0f, 0.0f);
	Vector3 normZero = vZero.Normalized();
	EXPECT_FLOAT_EQ(normZero.x, 0.0f);
	EXPECT_FLOAT_EQ(normZero.y, 0.0f);
	EXPECT_FLOAT_EQ(normZero.z, 0.0f);
}
TEST(Vector3Test, GetPerspective) {
	Vector3 v(3.0f, 4.0f, 12.0f);
	float perspective = v.GetPerspective(13.0f);

	// (3² + 4² + 12²) / 13 = 169 / 13 = 13
	EXPECT_FLOAT_EQ(perspective, 13.0f);

	// Test avec t <= 0
	EXPECT_FLOAT_EQ(v.GetPerspective(0.0f), 0.0f);
	EXPECT_FLOAT_EQ(v.GetPerspective(-1.0f), 0.0f);
}
TEST(Vector3Test, Reflect) {
	// Vecteur incident (0, -1, 0) frappant une surface normale (0, 1, 0)
	Vector3 I(0.0f, -1.0f, 0.0f);
	Vector3 N(0.0f, 1.0f, 0.0f);
	Vector3 reflected = Vector3::Reflect(I, N);

	// La réflexion devrait être (0, 1, 0)
	EXPECT_NEAR(reflected.x, 0.0f, EPSILON);
	EXPECT_NEAR(reflected.y, 1.0f, EPSILON);
	EXPECT_NEAR(reflected.z, 0.0f, EPSILON);

	// Test avec angle de 45 degrés
	Vector3 I2(1.0f, -1.0f, 0.0f);
	Vector3 N2(0.0f, 1.0f, 0.0f);
	Vector3 reflected2 = Vector3::Reflect(I2, N2);

	// La réflexion devrait être (1, 1, 0)
	EXPECT_NEAR(reflected2.x, 1.0f, EPSILON);
	EXPECT_NEAR(reflected2.y, 1.0f, EPSILON);
	EXPECT_NEAR(reflected2.z, 0.0f, EPSILON);
}
// Tests des produits
TEST(Vector3Test, DotProduct) {
	Vector3 v1(2.0f, 3.0f, 4.0f);
	Vector3 v2(5.0f, 6.0f, 7.0f);
	float dot = v1.DotProductVector3(v2);

	// 2*5 + 3*6 + 4*7 = 10 + 18 + 28 = 56
	EXPECT_FLOAT_EQ(dot, 56.0f);

	// Test avec vecteurs orthogonaux
	Vector3 v3(1.0f, 0.0f, 0.0f);
	Vector3 v4(0.0f, 1.0f, 0.0f);
	EXPECT_FLOAT_EQ(v3.DotProductVector3(v4), 0.0f);
}
TEST(Vector3Test, CrossProduct) {
	Vector3 v1(2.0f, 3.0f, 4.0f);
	Vector3 v2(5.0f, 6.0f, 7.0f);
	Vector3 cross = v1.CrossProductVector3(v2);

	// Cross = (y1*z2 - z1*y2, z1*x2 - x1*z2, x1*y2 - y1*x2)
	// = (3*7 - 4*6, 4*5 - 2*7, 2*6 - 3*5)
	// = (21 - 24, 20 - 14, 12 - 15)
	// = (-3, 6, -3)
	EXPECT_FLOAT_EQ(cross.x, -3.0f);
	EXPECT_FLOAT_EQ(cross.y, 6.0f);
	EXPECT_FLOAT_EQ(cross.z, -3.0f);

	// Test avec vecteurs de base
	Vector3 i(1.0f, 0.0f, 0.0f);
	Vector3 j(0.0f, 1.0f, 0.0f);
	Vector3 k = i.CrossProductVector3(j);
	EXPECT_FLOAT_EQ(k.x, 0.0f);
	EXPECT_FLOAT_EQ(k.y, 0.0f);
	EXPECT_FLOAT_EQ(k.z, 1.0f);
}
TEST(Vector3Test, GetAngle) {
	Vector3 v1(1.0f, 0.0f, 0.0f);
	Vector3 v2(0.0f, 1.0f, 0.0f);
	float angle = v1.GetAngle(v2);

	// Angle entre (1,0,0) et (0,1,0) = 90 degrés
	EXPECT_NEAR(angle, 90.0f, 0.001f);

	// Test avec vecteurs parallèles
	Vector3 v3(2.0f, 0.0f, 0.0f);
	EXPECT_NEAR(v1.GetAngle(v3), 0.0f, 0.001f);

	// Test avec vecteurs opposés
	Vector3 v4(-1.0f, 0.0f, 0.0f);
	EXPECT_NEAR(v1.GetAngle(v4), 180.0f, 0.001f);

	// Test avec vecteurs en 3D
	Vector3 v5(1.0f, 1.0f, 0.0f);
	Vector3 v6(0.0f, 1.0f, 1.0f);
	// Angle devrait être 60 degrés (cos = 0.5)
	EXPECT_NEAR(v5.GetAngle(v6), 60.0f, 0.001f);
}
// Test de rotation
TEST(Vector3Test, RotationX) {
	Vector3 v(0, 1, 0);
	v.Rotation(PI / 2, 0, 0);  // 90° autour de X

	EXPECT_NEAR(v.x, 0, EPSILON);
	EXPECT_NEAR(v.y, 0, EPSILON);
	EXPECT_NEAR(v.z, 1, EPSILON);
}
TEST(Vector3Test, RotationY) {
	Vector3 v(1, 0, 0);
	v.Rotation(0, PI / 2, 0);  // 90° autour de Y

	EXPECT_NEAR(v.x, 0, EPSILON);
	EXPECT_NEAR(v.y, 0, EPSILON);
	EXPECT_NEAR(v.z, -1, EPSILON);
}
TEST(Vector3Test, RotationZ) {
	Vector3 v(1, 0, 0);
	v.Rotation(0, 0, PI / 2);  // 90° autour de Z

	EXPECT_NEAR(v.x, 0, EPSILON);
	EXPECT_NEAR(v.y, 1, EPSILON);
	EXPECT_NEAR(v.z, 0, EPSILON);
}
TEST(Vector3Test, RotationCombined) {
	Vector3 v(1, 0, 0);
	v.Rotation(PI / 2, PI / 2, PI / 2);  // 90° sur chaque axe

	// Le résultat dépend de l'ordre des rotations
	float norm = v.GetNorm();
	EXPECT_NEAR(norm, 1, EPSILON);  // La norme doit être préservée
}
TEST(Vector3Test, RotationPreservesNorm) {
	Vector3 v(3, 4, 12);
	float originalNorm = v.GetNorm();

	v.Rotation(PI / 3, PI / 4, PI / 6);

	EXPECT_NEAR(v.GetNorm(), originalNorm, EPSILON);
}
TEST(Vector3Test, RotationZero) {
	Vector3 v(3, 4, 5);
	Vector3 original = v;

	v.Rotation(0, 0, 0);

	EXPECT_FLOAT_EQ(v.x, original.x);
	EXPECT_FLOAT_EQ(v.y, original.y);
	EXPECT_FLOAT_EQ(v.z, original.z);
}
TEST(Vector3Test, AsList) {
	Vector3 v(1.0f, 2.0f, 3.0f);
	std::array<float, 3> list = v.AsList();

	EXPECT_FLOAT_EQ(list[0], 1.0f);
	EXPECT_FLOAT_EQ(list[1], 2.0f);
	EXPECT_FLOAT_EQ(list[2], 3.0f);

	// Vérifier que c'est une copie, pas une référence
	list[0] = 42.0f;
	EXPECT_FLOAT_EQ(v.x, 1.0f);
}
// Tests des opérateurs
TEST(Vector3Test, OperatorPlus) {
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(4.0f, 5.0f, 6.0f);
	Vector3 result = v1 + v2;

	EXPECT_FLOAT_EQ(result.x, 5.0f);
	EXPECT_FLOAT_EQ(result.y, 7.0f);
	EXPECT_FLOAT_EQ(result.z, 9.0f);
}
TEST(Vector3Test, OperatorMinus) {
	Vector3 v1(5.0f, 7.0f, 9.0f);
	Vector3 v2(2.0f, 3.0f, 4.0f);
	Vector3 result = v1 - v2;

	EXPECT_FLOAT_EQ(result.x, 3.0f);
	EXPECT_FLOAT_EQ(result.y, 4.0f);
	EXPECT_FLOAT_EQ(result.z, 5.0f);
}
TEST(Vector3Test, OperatorMultiply) {
	Vector3 v1(2.0f, 3.0f, 4.0f);
	Vector3 v2(5.0f, 6.0f, 7.0f);
	Vector3 result = v1 * v2;

	EXPECT_FLOAT_EQ(result.x, 10.0f);
	EXPECT_FLOAT_EQ(result.y, 18.0f);
	EXPECT_FLOAT_EQ(result.z, 28.0f);
}
TEST(Vector3Test, OperatorDivide) {
	Vector3 v1(10.0f, 18.0f, 28.0f);
	Vector3 v2(2.0f, 3.0f, 4.0f);
	Vector3 result = v1 / v2;

	EXPECT_FLOAT_EQ(result.x, 5.0f);
	EXPECT_FLOAT_EQ(result.y, 6.0f);
	EXPECT_FLOAT_EQ(result.z, 7.0f);
}
TEST(Vector3Test, OperatorMultiplyScalar) {
	Vector3 v(2.0f, 3.0f, 4.0f);
	Vector3 result = v * 3.0f;

	EXPECT_FLOAT_EQ(result.x, 6.0f);
	EXPECT_FLOAT_EQ(result.y, 9.0f);
	EXPECT_FLOAT_EQ(result.z, 12.0f);
}
TEST(Vector3Test, OperatorUnaryMinus) {
	Vector3 v(2.0f, -3.0f, 4.0f);
	Vector3 result = -v;

	EXPECT_FLOAT_EQ(result.x, -2.0f);
	EXPECT_FLOAT_EQ(result.y, 3.0f);
	EXPECT_FLOAT_EQ(result.z, -4.0f);
}
TEST(Vector3Test, OperatorPlusEqual) {
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(4.0f, 5.0f, 6.0f);
	v1 += v2;

	EXPECT_FLOAT_EQ(v1.x, 5.0f);
	EXPECT_FLOAT_EQ(v1.y, 7.0f);
	EXPECT_FLOAT_EQ(v1.z, 9.0f);
}
TEST(Vector3Test, OperatorMinusEqual) {
	Vector3 v1(5.0f, 7.0f, 9.0f);
	Vector3 v2(2.0f, 3.0f, 4.0f);
	v1 -= v2;

	EXPECT_FLOAT_EQ(v1.x, 3.0f);
	EXPECT_FLOAT_EQ(v1.y, 4.0f);
	EXPECT_FLOAT_EQ(v1.z, 5.0f);
}
TEST(Vector3Test, OperatorMultiplyEqual) {
	Vector3 v1(2.0f, 3.0f, 4.0f);
	Vector3 v2(5.0f, 6.0f, 7.0f);
	v1 *= v2;

	EXPECT_FLOAT_EQ(v1.x, 10.0f);
	EXPECT_FLOAT_EQ(v1.y, 18.0f);
	EXPECT_FLOAT_EQ(v1.z, 28.0f);
}
TEST(Vector3Test, OperatorDivideEqual) {
	Vector3 v1(10.0f, 18.0f, 28.0f);
	Vector3 v2(2.0f, 3.0f, 4.0f);
	v1 /= v2;

	EXPECT_FLOAT_EQ(v1.x, 5.0f);
	EXPECT_FLOAT_EQ(v1.y, 6.0f);
	EXPECT_FLOAT_EQ(v1.z, 7.0f);
}
TEST(Vector3Test, OperatorSubscript) {
	Vector3 v(3.0f, 4.0f, 5.0f);

	EXPECT_FLOAT_EQ(v[0], 3.0f);
	EXPECT_FLOAT_EQ(v[1], 4.0f);
	EXPECT_FLOAT_EQ(v[2], 5.0f);

	// Modification
	v[0] = 6.0f;
	v[1] = 7.0f;
	v[2] = 8.0f;
	EXPECT_FLOAT_EQ(v.x, 6.0f);
	EXPECT_FLOAT_EQ(v.y, 7.0f);
	EXPECT_FLOAT_EQ(v.z, 8.0f);
}
TEST(Vector3Test, OperatorStream) {
	Vector3 v(1.0f, 2.0f, 3.0f);
	std::stringstream ss;
	ss << v;

	std::string expected = "Vector3(1, 2, 3)";  // À ajuster selon votre format
	// EXPECT_EQ(ss.str(), expected);
}
// Test avec valeurs négatives
TEST(Vector3Test, NegativeValues) {
	Vector3 v1(-3.0f, -4.0f, -12.0f);
	Vector3 v2(1.0f, 2.0f, 3.0f);

	Vector3 sum = v1 + v2;
	EXPECT_FLOAT_EQ(sum.x, -2.0f);
	EXPECT_FLOAT_EQ(sum.y, -2.0f);
	EXPECT_FLOAT_EQ(sum.z, -9.0f);

	float norm = v1.GetNorm();
	EXPECT_FLOAT_EQ(norm, 13.0f);  // La norme reste positive

	Vector3 normalized = v1.Normalized();
	EXPECT_NEAR(normalized.x, -3.0f / 13.0f, EPSILON);
	EXPECT_NEAR(normalized.y, -4.0f / 13.0f, EPSILON);
	EXPECT_NEAR(normalized.z, -12.0f / 13.0f, EPSILON);
}
// Test des cas limites
TEST(Vector3Test, EdgeCases) {
	// Très grandes valeurs
	Vector3 v1(1e6f, 1e6f, 1e6f);
	Vector3 v2(1e6f, 1e6f, 1e6f);
	float dot = v1.DotProductVector3(v2);
	EXPECT_FLOAT_EQ(dot, 3e12f);

	// Très petites valeurs
	Vector3 v3(1e-6f, 1e-6f, 1e-6f);
	Vector3 v4(1e-6f, 1e-6f, 1e-6f);
	float distance = v3.GetDistance(v4);
	EXPECT_NEAR(distance, 0.0f, 1e-5f);
}
// Test de cohérence Vector2 -> Vector3
TEST(Vector3Test, Vector2Conversion) {
	Vector2 v2(3.0f, 4.0f);
	Vector3 v3(v2, 5.0f);

	EXPECT_FLOAT_EQ(v3.x, v2.x);
	EXPECT_FLOAT_EQ(v3.y, v2.y);
	EXPECT_FLOAT_EQ(v3.z, 5.0f);

	// Test des opérations combinées
	Vector3 v3b(1.0f, 2.0f, 3.0f);
	Vector3 result = v3 + v3b;
	EXPECT_FLOAT_EQ(result.x, 4.0f);
	EXPECT_FLOAT_EQ(result.y, 6.0f);
	EXPECT_FLOAT_EQ(result.z, 8.0f);
}

// -------------------------------------------------------------------------------- //	
//									TEST VECTOR4									//
// -------------------------------------------------------------------------------- //

/// <summary>
///  Test Vector4
/// </summary>

// Test du constructeur et des valeurs par défaut
TEST(Vector4Test, Constructor) {
	Vector4 v1;
	EXPECT_FLOAT_EQ(v1.x, 0.0f);
	EXPECT_FLOAT_EQ(v1.y, 0.0f);
	EXPECT_FLOAT_EQ(v1.z, 0.0f);
	EXPECT_FLOAT_EQ(v1.w, 1.0f);  // w par défaut à 1

	Vector4 v2(1.5f, -2.3f, 4.7f, 2.0f);
	EXPECT_FLOAT_EQ(v2.x, 1.5f);
	EXPECT_FLOAT_EQ(v2.y, -2.3f);
	EXPECT_FLOAT_EQ(v2.z, 4.7f);
	EXPECT_FLOAT_EQ(v2.w, 2.0f);

	// Test avec w par défaut
	Vector4 v3(1.0f, 2.0f, 3.0f);
	EXPECT_FLOAT_EQ(v3.x, 1.0f);
	EXPECT_FLOAT_EQ(v3.y, 2.0f);
	EXPECT_FLOAT_EQ(v3.z, 3.0f);
	EXPECT_FLOAT_EQ(v3.w, 1.0f);
}

// Test constructeur depuis Vector3
TEST(Vector4Test, ConstructorFromVector3) {
	Vector3 v3(2.0f, 3.0f, 4.0f);
	Vector4 v4(v3);

	EXPECT_FLOAT_EQ(v4.x, 2.0f);
	EXPECT_FLOAT_EQ(v4.y, 3.0f);
	EXPECT_FLOAT_EQ(v4.z, 4.0f);
	EXPECT_FLOAT_EQ(v4.w, 1.0f);  // w par défaut à 1

	Vector4 v4b(v3, 5.0f);
	EXPECT_FLOAT_EQ(v4b.x, 2.0f);
	EXPECT_FLOAT_EQ(v4b.y, 3.0f);
	EXPECT_FLOAT_EQ(v4b.z, 4.0f);
	EXPECT_FLOAT_EQ(v4b.w, 5.0f);
}

// Test conversion vers Vector3
TEST(Vector4Test, ConversionToVector3) {
	Vector4 v4(1.0f, 2.0f, 3.0f, 4.0f);
	Vector3 v3 = static_cast<Vector3>(v4);  // Conversion explicite via operator Vector3

	EXPECT_FLOAT_EQ(v3.x, 1.0f);
	EXPECT_FLOAT_EQ(v3.y, 2.0f);
	EXPECT_FLOAT_EQ(v3.z, 3.0f);
	// w est ignoré dans la conversion
}

// Test des opérations élémentaires
TEST(Vector4Test, GetOpposed) {
	Vector4 v(3.0f, -4.0f, 5.0f, -6.0f);
	Vector4 opposed = v.GetOpposed();

	EXPECT_FLOAT_EQ(opposed.x, -3.0f);
	EXPECT_FLOAT_EQ(opposed.y, 4.0f);
	EXPECT_FLOAT_EQ(opposed.z, -5.0f);
	EXPECT_FLOAT_EQ(opposed.w, 6.0f);
}

TEST(Vector4Test, AddVector4) {
	Vector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 v2(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 result = v1.AddVector4(v2);

	EXPECT_FLOAT_EQ(result.x, 6.0f);
	EXPECT_FLOAT_EQ(result.y, 8.0f);
	EXPECT_FLOAT_EQ(result.z, 10.0f);
	EXPECT_FLOAT_EQ(result.w, 12.0f);
}

TEST(Vector4Test, AddScalar) {
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 result = v.AddScalar(5.0f);

	EXPECT_FLOAT_EQ(result.x, 6.0f);
	EXPECT_FLOAT_EQ(result.y, 7.0f);
	EXPECT_FLOAT_EQ(result.z, 8.0f);
	EXPECT_FLOAT_EQ(result.w, 9.0f);
}

TEST(Vector4Test, ElementWiseProduct) {
	Vector4 v1(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 v2(6.0f, 7.0f, 8.0f, 9.0f);
	Vector4 result = v1.ElementWiseProduct(v2);

	EXPECT_FLOAT_EQ(result.x, 12.0f);
	EXPECT_FLOAT_EQ(result.y, 21.0f);
	EXPECT_FLOAT_EQ(result.z, 32.0f);
	EXPECT_FLOAT_EQ(result.w, 45.0f);
}

TEST(Vector4Test, ProductScalar) {
	Vector4 v(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 result = v.ProductScalar(3.0f);

	EXPECT_FLOAT_EQ(result.x, 6.0f);
	EXPECT_FLOAT_EQ(result.y, 9.0f);
	EXPECT_FLOAT_EQ(result.z, 12.0f);
	EXPECT_FLOAT_EQ(result.w, 15.0f);
}

// Tests géométriques
TEST(Vector4Test, GetMid) {
	Vector4 v1(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4 v2(10.0f, 10.0f, 10.0f, 10.0f);
	Vector4 mid = v1.GetMid(v2);

	EXPECT_FLOAT_EQ(mid.x, 5.0f);
	EXPECT_FLOAT_EQ(mid.y, 5.0f);
	EXPECT_FLOAT_EQ(mid.z, 5.0f);
	EXPECT_FLOAT_EQ(mid.w, 5.0f);
}

TEST(Vector4Test, GetDistance) {
	Vector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 v2(4.0f, 6.0f, 8.0f, 10.0f);
	float distance = v1.GetDistance(v2);

	// Distance = sqrt((4-1)² + (6-2)² + (8-3)² + (10-4)²) 
	// = sqrt(9 + 16 + 25 + 36) = sqrt(86) ≈ 9.2736
	EXPECT_NEAR(distance, 9.273618f, 0.0001f);
}

TEST(Vector4Test, GetDistancePow) {
	Vector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 v2(4.0f, 6.0f, 8.0f, 10.0f);
	float distancePow = v1.GetDistancePow(v2);

	// Distance² = (4-1)² + (6-2)² + (8-3)² + (10-4)² = 9 + 16 + 25 + 36 = 86
	EXPECT_FLOAT_EQ(distancePow, 86.0f);
}

TEST(Vector4Test, GetNorm) {
	Vector4 v(3.0f, 4.0f, 12.0f, 84.0f);
	float norm = v.GetNorm();

	// Norme = sqrt(3² + 4² + 12² + 84²) = sqrt(9 + 16 + 144 + 7056) = sqrt(7225) = 85
	EXPECT_FLOAT_EQ(norm, 85.0f);

	Vector4 v2(0.0f, 0.0f, 0.0f, 0.0f);
	EXPECT_FLOAT_EQ(v2.GetNorm(), 0.0f);
}

TEST(Vector4Test, GetSquareNorm) {
	Vector4 v(3.0f, 4.0f, 12.0f, 84.0f);
	float squareNorm = v.GetSquareNorm();

	EXPECT_FLOAT_EQ(squareNorm, 7225.0f);
}

TEST(Vector4Test, Normalized) {
	Vector4 v(3.0f, 4.0f, 12.0f, 84.0f);
	Vector4 normalized = v.Normalized();

	// Vecteur unitaire = (3/85, 4/85, 12/85, 84/85)
	EXPECT_NEAR(normalized.x, 3.0f / 85.0f, EPSILON);
	EXPECT_NEAR(normalized.y, 4.0f / 85.0f, EPSILON);
	EXPECT_NEAR(normalized.z, 12.0f / 85.0f, EPSILON);
	EXPECT_NEAR(normalized.w, 84.0f / 85.0f, EPSILON);
	EXPECT_NEAR(normalized.GetNorm(), 1.0f, EPSILON);

	// Test avec vecteur nul
	Vector4 vZero(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4 normZero = vZero.Normalized();
	EXPECT_FLOAT_EQ(normZero.x, 0.0f);
	EXPECT_FLOAT_EQ(normZero.y, 0.0f);
	EXPECT_FLOAT_EQ(normZero.z, 0.0f);
	EXPECT_FLOAT_EQ(normZero.w, 0.0f);
}

TEST(Vector4Test, GetPerspective) {
	Vector4 v(3.0f, 4.0f, 12.0f, 84.0f);
	float perspective = v.GetPerspective(85.0f);

	// (3² + 4² + 12² + 84²) / 85 = 7225 / 85 = 85
	EXPECT_FLOAT_EQ(perspective, 85.0f);

	// Test avec t <= 0
	EXPECT_FLOAT_EQ(v.GetPerspective(0.0f), 0.0f);
	EXPECT_FLOAT_EQ(v.GetPerspective(-1.0f), 0.0f);
}

// Test du produit scalaire
TEST(Vector4Test, DotProduct) {
	Vector4 v1(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 v2(6.0f, 7.0f, 8.0f, 9.0f);
	float dot = v1.DotProductVector4(v2);

	// 2*6 + 3*7 + 4*8 + 5*9 = 12 + 21 + 32 + 45 = 110
	EXPECT_FLOAT_EQ(dot, 110.0f);

	// Test avec vecteurs orthogonaux
	Vector4 v3(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4 v4(0.0f, 1.0f, 0.0f, 0.0f);
	EXPECT_FLOAT_EQ(v3.DotProductVector4(v4), 0.0f);
}

// Tests des opérateurs
TEST(Vector4Test, OperatorPlus) {
	Vector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 v2(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 result = v1 + v2;

	EXPECT_FLOAT_EQ(result.x, 6.0f);
	EXPECT_FLOAT_EQ(result.y, 8.0f);
	EXPECT_FLOAT_EQ(result.z, 10.0f);
	EXPECT_FLOAT_EQ(result.w, 12.0f);
}

TEST(Vector4Test, OperatorMinus) {
	Vector4 v1(5.0f, 7.0f, 9.0f, 11.0f);
	Vector4 v2(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 result = v1 - v2;

	EXPECT_FLOAT_EQ(result.x, 3.0f);
	EXPECT_FLOAT_EQ(result.y, 4.0f);
	EXPECT_FLOAT_EQ(result.z, 5.0f);
	EXPECT_FLOAT_EQ(result.w, 6.0f);
}

TEST(Vector4Test, OperatorMultiply) {
	Vector4 v1(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 v2(6.0f, 7.0f, 8.0f, 9.0f);
	Vector4 result = v1 * v2;

	EXPECT_FLOAT_EQ(result.x, 12.0f);
	EXPECT_FLOAT_EQ(result.y, 21.0f);
	EXPECT_FLOAT_EQ(result.z, 32.0f);
	EXPECT_FLOAT_EQ(result.w, 45.0f);
}

TEST(Vector4Test, OperatorDivide) {
	Vector4 v1(12.0f, 21.0f, 32.0f, 45.0f);
	Vector4 v2(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 result = v1 / v2;

	EXPECT_FLOAT_EQ(result.x, 6.0f);
	EXPECT_FLOAT_EQ(result.y, 7.0f);
	EXPECT_FLOAT_EQ(result.z, 8.0f);
	EXPECT_FLOAT_EQ(result.w, 9.0f);
}

TEST(Vector4Test, OperatorPlusEqual) {
	Vector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 v2(5.0f, 6.0f, 7.0f, 8.0f);
	v1 += v2;

	EXPECT_FLOAT_EQ(v1.x, 6.0f);
	EXPECT_FLOAT_EQ(v1.y, 8.0f);
	EXPECT_FLOAT_EQ(v1.z, 10.0f);
	EXPECT_FLOAT_EQ(v1.w, 12.0f);
}

TEST(Vector4Test, OperatorMinusEqual) {
	Vector4 v1(5.0f, 7.0f, 9.0f, 11.0f);
	Vector4 v2(2.0f, 3.0f, 4.0f, 5.0f);
	v1 -= v2;

	EXPECT_FLOAT_EQ(v1.x, 3.0f);
	EXPECT_FLOAT_EQ(v1.y, 4.0f);
	EXPECT_FLOAT_EQ(v1.z, 5.0f);
	EXPECT_FLOAT_EQ(v1.w, 6.0f);
}

TEST(Vector4Test, OperatorMultiplyEqual) {
	Vector4 v1(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 v2(6.0f, 7.0f, 8.0f, 9.0f);
	v1 *= v2;

	EXPECT_FLOAT_EQ(v1.x, 12.0f);
	EXPECT_FLOAT_EQ(v1.y, 21.0f);
	EXPECT_FLOAT_EQ(v1.z, 32.0f);
	EXPECT_FLOAT_EQ(v1.w, 45.0f);
}

TEST(Vector4Test, OperatorDivideEqual) {
	Vector4 v1(12.0f, 21.0f, 32.0f, 45.0f);
	Vector4 v2(2.0f, 3.0f, 4.0f, 5.0f);
	v1 /= v2;

	EXPECT_FLOAT_EQ(v1.x, 6.0f);
	EXPECT_FLOAT_EQ(v1.y, 7.0f);
	EXPECT_FLOAT_EQ(v1.z, 8.0f);
	EXPECT_FLOAT_EQ(v1.w, 9.0f);
}

TEST(Vector4Test, OperatorSubscript) {
	Vector4 v(3.0f, 4.0f, 5.0f, 6.0f);

	EXPECT_FLOAT_EQ(v[0], 3.0f);
	EXPECT_FLOAT_EQ(v[1], 4.0f);
	EXPECT_FLOAT_EQ(v[2], 5.0f);
	EXPECT_FLOAT_EQ(v[3], 6.0f);

	// Modification
	v[0] = 7.0f;
	v[1] = 8.0f;
	v[2] = 9.0f;
	v[3] = 10.0f;
	EXPECT_FLOAT_EQ(v.x, 7.0f);
	EXPECT_FLOAT_EQ(v.y, 8.0f);
	EXPECT_FLOAT_EQ(v.z, 9.0f);
	EXPECT_FLOAT_EQ(v.w, 10.0f);

	// Test hors limites (doit assert en debug)
	// EXPECT_DEATH(v[4], "Index out of range");
}

// Test de AsList avec std::array
TEST(Vector4Test, AsList) {
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	std::array<float, 4> list = v.AsList();

	EXPECT_FLOAT_EQ(list[0], 1.0f);
	EXPECT_FLOAT_EQ(list[1], 2.0f);
	EXPECT_FLOAT_EQ(list[2], 3.0f);
	EXPECT_FLOAT_EQ(list[3], 4.0f);

	// Vérifier que c'est une copie
	list[0] = 42.0f;
	EXPECT_FLOAT_EQ(v.x, 1.0f);  // L'original n'est pas modifié
}

// Test avec valeurs négatives
TEST(Vector4Test, NegativeValues) {
	Vector4 v1(-3.0f, -4.0f, -12.0f, -84.0f);
	Vector4 v2(1.0f, 2.0f, 3.0f, 4.0f);

	Vector4 sum = v1 + v2;
	EXPECT_FLOAT_EQ(sum.x, -2.0f);
	EXPECT_FLOAT_EQ(sum.y, -2.0f);
	EXPECT_FLOAT_EQ(sum.z, -9.0f);
	EXPECT_FLOAT_EQ(sum.w, -80.0f);

	float norm = v1.GetNorm();
	EXPECT_FLOAT_EQ(norm, 85.0f);  // La norme reste positive

	Vector4 normalized = v1.Normalized();
	EXPECT_NEAR(normalized.x, -3.0f / 85.0f, EPSILON);
	EXPECT_NEAR(normalized.y, -4.0f / 85.0f, EPSILON);
	EXPECT_NEAR(normalized.z, -12.0f / 85.0f, EPSILON);
	EXPECT_NEAR(normalized.w, -84.0f / 85.0f, EPSILON);
}

// Test des cas limites
TEST(Vector4Test, EdgeCases) {
	// Très grandes valeurs
	Vector4 v1(1e6f, 1e6f, 1e6f, 1e6f);
	Vector4 v2(1e6f, 1e6f, 1e6f, 1e6f);
	float dot = v1.DotProductVector4(v2);
	EXPECT_FLOAT_EQ(dot, 4e12f);

	// Très petites valeurs
	Vector4 v3(1e-6f, 1e-6f, 1e-6f, 1e-6f);
	Vector4 v4(1e-6f, 1e-6f, 1e-6f, 1e-6f);
	float distance = v3.GetDistance(v4);
	EXPECT_NEAR(distance, 0.0f, 1e-5f);
}

// Test de la division par zéro (assert)
TEST(Vector4Test, DivisionByZero) {
	Vector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 v2(0.0f, 1.0f, 1.0f, 1.0f);

	// Ces tests doivent assert en mode debug
	// EXPECT_DEATH(v1 / v2, "Division by zero");
	// EXPECT_DEATH(v1 /= v2, "Division by zero");
}

// Test de cohérence Vector3 -> Vector4 -> Vector3
TEST(Vector4Test, Vector3Consistency) {
	Vector3 v3(2.0f, 3.0f, 4.0f);
	Vector4 v4(v3, 5.0f);
	Vector3 v3b = static_cast<Vector3>(v4);

	EXPECT_FLOAT_EQ(v3b.x, v3.x);
	EXPECT_FLOAT_EQ(v3b.y, v3.y);
	EXPECT_FLOAT_EQ(v3b.z, v3.z);
}

// Test du produit scalaire avec w=0 (vecteur directionnel)
TEST(Vector4Test, DotProductDirectional) {
	Vector4 v1(1.0f, 0.0f, 0.0f, 0.0f);  // Vecteur directionnel
	Vector4 v2(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 v3(1.0f, 0.0f, 0.0f, 1.0f);  // Point

	EXPECT_FLOAT_EQ(v1.DotProductVector4(v2), 0.0f);
	EXPECT_FLOAT_EQ(v1.DotProductVector4(v3), 1.0f);
}

// Test du constructeur par défaut avec w
TEST(Vector4Test, DefaultWValue) {
	Vector4 v1;  // Par défaut w = 1
	EXPECT_FLOAT_EQ(v1.w, 1.0f);

	Vector4 v2(1.0f, 2.0f, 3.0f);  // w par défaut = 1
	EXPECT_FLOAT_EQ(v2.w, 1.0f);

	Vector4 v3(1.0f, 2.0f, 3.0f, 4.0f);  // w explicite
	EXPECT_FLOAT_EQ(v3.w, 4.0f);
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