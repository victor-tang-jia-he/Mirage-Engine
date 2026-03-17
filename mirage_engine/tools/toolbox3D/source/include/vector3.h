#pragma once

#include "vector2.h"

namespace MirageEngine
{
	namespace Core
	{
		namespace Math
		{
			struct Vector3
			{
				// -------------------------------------------------------------------------------- //	
				// Constructor & variables
				// -------------------------------------------------------------------------------- //

				float x{ 0 };
				float y{ 0 };
				float z{ 0 };

				Vector3() = default;
				inline Vector3(Vector2 _vec2, float _z) noexcept
					: x(_vec2.x), y(_vec2.y), z(_z) {
				}
				inline Vector3(float _x, float _y, float _z) noexcept
					: x(_x), y(_y), z(_z) {
				}
				~Vector3() = default;


				// -------------------------------------------------------------------------------- //	
				// Elementary operations on vector components
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Returns the opposite of the vector (negating its components).
				/// </summary>
				/// <returns>The opposed vector.</returns>
				Vector3 Opposed() const; // Checked

				/// <summary>
				/// Adds the current vector with another vector.
				/// </summary>
				/// <param name="other">The vector to add.</param>
				/// <returns>The resulting vector after addition.</returns>
				Vector3 AddVector3(const Vector3& other) const; // Checked

				/// <summary>
				/// Addition of a scalar to each component
				/// </summary>
				/// <param name="scalar">The scalar to add.</param>
				/// <returns>The resulting vector after addition of the scalar.</returns>
				Vector3 AddScalar(float scalar) const; // Checked

				/// <summary>
				/// Term product of two Vector
				/// </summary>
				/// <param name="other">The vector to Element Wise Product with.</param>
				/// <returns>The resulting vector after Element Wise Product.</returns>
				Vector3 ElementWiseProduct(const Vector3& other) const; // Checked

				/// <summary>
				/// Multiply of a scalar to each component
				/// </summary>
				/// <param name="scalar">The scalar to Multiply.</param>
				/// <returns>The resulting vector after addition of the scalar.</returns>
				Vector3 ProductScalar(float scalar) const; // Checked

				bool NearlyEqual(const Vector3& other, float epsilon = EPSILON) const;

				bool IsNull() const; // Checked

				// -------------------------------------------------------------------------------- //	
				// Geometric & standard calculations
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Calculates the midpoint between the current vector and another vector.
				/// </summary>
				/// <param name="other">The other vector to find the midpoint with.</param>
				/// <returns>The midpoint vector.</returns>
				Vector3 Mid(const Vector3& other) const; // Checked

				/// <summary>
				/// Computes the distance between the current vector and another vector.
				/// </summary>
				/// <param name="other">The other vector to calculate the distance to.</param>
				/// <returns>The distance between the two vectors.</returns>
				float Distance(const Vector3& other) const; // Checked

				/// <summary>
				/// Computes the squared distance between the current vector and another vector, without calculating the square root (faster).
				/// </summary>
				/// <param name="other">The other vector to calculate the squared distance to.</param>
				/// <returns>The squared distance between the two vectors.</returns>
				float DistancePow(const Vector3& other) const; // Checked

				/// <summary>
				/// Returns the norm (magnitude) of the vector.
				/// </summary>
				/// <returns>The length of the vector.</returns>
				float Norm() const; // Checked

				/// <summary>
				/// Return the sum of the squares of its components.
				/// </summary>
				/// <returns>Return the Square of the vector.</returns>
				float SquareNorm() const; // Checked

				/// <summary>
				/// Returns a normalized version of the vector (unit vector).
				/// </summary>
				/// <returns>A new Vector3 with the same direction but a magnitude of 1.</returns>
				Vector3 Normalized() const; // Checked

				/// <summary>
				/// Returns the perspective-transformed value of the vector using the formula: (x² + y² + z²) / t.
				/// </summary>
				/// <param name="t">The perspective parameter. Must be greater than 0.</param>
				/// <returns>The result of the perspective function, or 0 if t is less than or equal to 0.</returns>
				float Perspective(float t) const; // Checked

				/// <summary>
				/// Return the Reflected Vector.
				/// </summary>
				/// <param name="I">vecteur incident</param>
				/// <param name="N">Surface Normal</param>
				/// <returns>The result of the perspective function, or 0 if t is less than or equal to 0.</returns>
				static Vector3 Reflect(Vector3 I, Vector3 N); // Checked

				// -------------------------------------------------------------------------------- //	
				// Products & angles
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Computes the scalar (dot) product of the current vector and another vector.
				/// </summary>
				/// <param name="other">The other vector to compute the dot product with.</param>
				/// <returns>The scalar (dot) product of the two vectors.</returns>
				float DotProductVector3(const Vector3& other) const; // Checked

				/// <summary>
				/// Computes the 3D cross product between the current vector and another vector.
				/// </summary>
				/// <param name="other">The other vector to compute the cross product with.</param>
				/// <returns>The scalar result of the 3D cross product.</returns>
				Vector3 CrossProductVector3(const Vector3& other) const; // Checked

				/// <summary>
				/// Calculates the angle (in radian) between the current vector and another vector.
				/// </summary>
				/// <param name="v2">The other vector to calculate the angle with.</param>
				/// <returns>The angle in degrees between the two vectors.</returns>
				float Angle(const Vector3& v2) const; // Checked


				// -------------------------------------------------------------------------------- //	
				// Transform
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Rotates the vector by the specified angle (in degrees).
				/// </summary>
				/// <param name="angleX">The angle X in degrees to rotate the vector.</param>
				/// <param name="angleY">The angle Y in degrees to rotate the vector.</param>
				/// <param name="angleZ">The angle Z in degrees to rotate the vector.</param>
				/// <returns>None</returns>
				Vector3 Rotate(float angleX, float angleY, float angleZ) const; // Checked


				// -------------------------------------------------------------------------------- //	
				// Display
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Prints all values of the vector (x, y, z) to the console.
				/// </summary>
				/// <param name="">None</param>
				/// <returns>None</returns>
				void Print() const; // Checked

				std::array<float, 3> AsList() const; // Checked

				// -------------------------------------------------------------------------------- //	
				// Overload operators inline
				// -------------------------------------------------------------------------------- //

				Vector3 operator+(const Vector3& vec) const; // Checked
				Vector3 operator-(const Vector3& vec) const; // Checked
				Vector3 operator*(const Vector3& vec) const; // Checked
				Vector3 operator/(const Vector3& vec) const; // Checked
				Vector3& operator+=(const Vector3& vec); // Checked
				Vector3& operator-=(const Vector3& vec); // Checked
				Vector3& operator*=(const Vector3& vec); // Checked
				Vector3& operator/=(const Vector3& vec); // Checked
				
				Vector3 operator+(float scalar) const; // Checked
				Vector3 operator-(float scalar) const; // Checked
				Vector3 operator*(float scalar) const; // Checked
				Vector3 operator/(float scalar) const; // Checked
				Vector3& operator*=(float scalar); // Checked
				Vector3& operator/=(float scalar); // Checked
				Vector3& operator+=(float scalar); // Checked
				Vector3& operator-=(float scalar); // Checked

				Vector3 operator-() const; // Checked

				explicit operator Vector2() const; // Checked

				// Checked
				float& operator[](int index);
				const float& operator[](int index) const;

				bool operator==(const Vector3& other) const; // Checked
				bool operator!=(const Vector3& other) const; // Checked
			};
			Vector3 operator*(float scalar, const Vector3& vec); // Checked
			std::ostream& operator<<(std::ostream& os, const Vector3& vec); // Checked
		}
	}
}
