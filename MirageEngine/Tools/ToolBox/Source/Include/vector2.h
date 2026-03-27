#pragma once
#include <ostream>

#define PI 3.14159265359f
#define EPSILON 1e-6f

inline float ToRad(float _angle)
{
	return _angle * PI / 180.f;
}

namespace MirageEngine
{
	namespace Core
	{
		namespace Math
		{
			struct Vector2
			{
				// -------------------------------------------------------------------------------- //	
				// Constructor & variables
				// -------------------------------------------------------------------------------- //

				float X{ 0 };
				float Y{ 0 };

				Vector2() = default;
				inline Vector2(float _x, float _y) noexcept// Checked
					: X(_x), Y(_y) {
				}
				~Vector2() = default;


				// -------------------------------------------------------------------------------- //	
				// Elementary operations on vector components
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Returns the opposite of the vector (negating its components).
				/// </summary>
				/// <returns>The opposed vector.</returns>
				Vector2 Opposed() const;  // Checked

				/// <summary>
				/// Adds the current vector with another vector.
				/// </summary>
				/// <param name="_other">The vector to add.</param>
				/// <returns>The resulting vector after addition.</returns>
				Vector2 AddVector2(const Vector2& _other) const; // Checked

				/// <summary>
				/// Addition of a _scalar to each component
				/// </summary>
				/// <param name="_scalar">The _scalar to add.</param>
				/// <returns>The resulting vector after addition of the _scalar.</returns>
				Vector2 AddScalar(float _scalar) const; // Checked

				/// <summary>
				/// Term product of two Vector
				/// </summary>
				/// <param name="_other">The vector to Element Wise Product with.</param>
				/// <returns>The resulting vector after Element Wise Product.</returns>
				Vector2 ElementWiseProduct(const Vector2& _other) const; // Checked

				/// <summary>
				/// Multiply of a _scalar to each component
				/// </summary>
				/// <param name="_scalar">The _scalar to Multiply.</param>
				/// <returns>The resulting vector after addition of the _scalar.</returns>
				Vector2 ProductScalar(float _scalar) const; // Checked

				bool NearlyEqual(const Vector2& _other, float _epsilon = EPSILON) const; // Checked

				// -------------------------------------------------------------------------------- //	
				// Geometric & standard calculations
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Calculates the midpoint between the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to find the midpoint with.</param>
				/// <returns>The midpoint vector.</returns>
				Vector2 Mid(const Vector2& _other) const; // Checked

				/// <summary>
				/// Computes the distance between the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to calculate the distance to.</param>
				/// <returns>The distance between the two vectors.</returns>
				float Distance(const Vector2& _other) const; // Checked

				/// <summary>
				/// Computes the squared distance between the current vector and another vector, without calculating the square root (faster).
				/// </summary>
				/// <param name="_other">The _other vector to calculate the squared distance to.</param>
				/// <returns>The squared distance between the two vectors.</returns>
				float DistancePow(const Vector2& _other) const; // Checked

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
				/// <returns>A new Vector2 with the same direction but a magnitude of 1.</returns>
				Vector2 Normalized() const; // Checked

				/// <summary>
				/// Returns the perspective-transformed value of the vector using the formula: (x² + y²) / t.
				/// </summary>
				/// <param name="_t">The perspective parameter. Must be greater than 0.</param>
				/// <returns>The result of the perspective function, or 0 if _t is less than or equal to 0.</returns>
				float Perspective(float _t) const; // Checked


				// -------------------------------------------------------------------------------- //	
				// Products & angles
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Computes the _scalar (dot) product of the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to compute the dot product with.</param>
				/// <returns>The _scalar (dot) product of the two vectors.</returns>
				float DotProductVector2(const Vector2& _other) const; // Checked

				/// <summary>
				/// Computes the 2D cross product between the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to compute the cross product with.</param>
				/// <returns>The _scalar result of the 2D cross product.</returns>
				float CrossProductVector2(const Vector2& _other) const; // Checked

				/// <summary>
				/// Calculates the _angle (in radian) between the current vector and another vector.
				/// </summary>
				/// <param name="_v2">The _other vector to calculate the _angle with.</param>
				/// <returns>The _angle in degrees between the two vectors.</returns>
				float Angle(const Vector2& _v2) const; // Checked


				// -------------------------------------------------------------------------------- //	
				// Transform
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Rotates the vector by the specified _angle (in radian).
				/// </summary>
				/// <param name="_angle">The _angle in degrees to rotate the vector.</param>
				/// <returns>Updated Vector 2</returns>
				Vector2 Rotate(float _rad) const; // Checked


				// -------------------------------------------------------------------------------- //	
				// Display
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Prints all values of the vector (X, Y) to the console.
				/// </summary>
				/// <param name="">None</param>
				/// <returns>None</returns>
				void Print() const; // Checked


				// -------------------------------------------------------------------------------- //	
				// Overload operators inline
				// -------------------------------------------------------------------------------- //


				Vector2 operator+(const Vector2& _vec) const; // Checked
				Vector2 operator-(const Vector2& _vec) const; // Checked
				Vector2 operator*(const Vector2& _vec) const; // Checked
				Vector2 operator/(const Vector2& _vec) const; // Checked
				Vector2& operator+=(const Vector2& _vec); // Checked
				Vector2& operator-=(const Vector2& _vec); // Checked
				Vector2& operator*=(const Vector2& _vec); // Checked
				Vector2& operator/=(const Vector2& _vec); // Checked

				Vector2 operator+(float _scalar) const; // Checked
				Vector2 operator-(float _scalar) const; // Checked
				Vector2 operator*(float _scalar) const; // Checked
				Vector2 operator/(float _scalar) const; // Checked
				Vector2& operator*=(float _scalar); // Checked
				Vector2& operator/=(float _scalar); // Checked
				Vector2& operator+=(float _scalar); // Checked
				Vector2& operator-=(float _scalar); // Checked


				Vector2 operator-() const; // Checked

				// Checked
				float& operator[](int _index);
				const float& operator[](int _index) const;

				bool operator==(const Vector2& _other) const; // Checked
				bool operator!=(const Vector2& _other) const; // Checked
			};
			Vector2 operator*(float _scalar, const Vector2& _vec); // Checked
			std::ostream& operator<<(std::ostream& _os, const Vector2& _vec); // Checked
		}
	}
}
