#pragma once

#include "vector3.h"
namespace MirageEngine
{
	namespace Core
	{
		namespace Math
		{
			class Vector4
			{
			public:
				float x;
				float y;
				float z;
				float w;

				inline Vector4(Vector3 vec3, float _w = 1.f)
					: x(vec3.x), y(vec3.y), z(vec3.z), w(_w) {
				}
				inline Vector4(float _x = 0.f, float _y = 0.f, float _z = 0.f, float _w = 1.f)
					: x(_x), y(_y), z(_z), w(_w) {
				}
				~Vector4() = default;

				// -------------------------------------------------------------------------------- //	
				// Elementary operations on vector components
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Returns the opposite of the vector (negating its components).
				/// </summary>
				/// <returns>The opposed vector.</returns>
				Vector4 Opposed() const; // Checked

				/// <summary>
				/// Adds the current vector with another vector.
				/// </summary>
				/// <param name="other">The vector to add.</param>
				/// <returns>The resulting vector after addition.</returns>
				Vector4 AddVector4(const Vector4& other) const; // Checked

				/// <summary>
				/// Addition of a scalar to each component
				/// </summary>
				/// <param name="scalar">The scalar to add.</param>
				/// <returns>The resulting vector after addition of the scalar.</returns>
				Vector4 AddScalar(float scalar) const; // Checked

				/// <summary>
				/// Term product of two Vector
				/// </summary>
				/// <param name="other">The vector to Element Wise Product with.</param>
				/// <returns>The resulting vector after Element Wise Product.</returns>
				Vector4 ElementWiseProduct(const Vector4& other) const; // Checked

				/// <summary>
				/// Multiply of a scalar to each component
				/// </summary>
				/// <param name="scalar">The scalar to Multiply.</param>
				/// <returns>The resulting vector after addition of the scalar.</returns>
				Vector4 ProductScalar(float scalar) const; // Checked

				bool NearlyEqual(const Vector4& other, float epsilon = EPSILON) const; // Checked
				// -------------------------------------------------------------------------------- //	
				// Geometric & standard calculations
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Calculates the midpoint between the current vector and another vector.
				/// </summary>
				/// <param name="other">The other vector to find the midpoint with.</param>
				/// <returns>The midpoint vector.</returns>
				Vector4 Mid(const Vector4& other) const; // Checked

				/// <summary>
				/// Computes the distance between the current vector and another vector.
				/// </summary>
				/// <param name="other">The other vector to calculate the distance to.</param>
				/// <returns>The distance between the two vectors.</returns>
				float Distance(const Vector4& other) const; // Checked

				/// <summary>
				/// Computes the squared distance between the current vector and another vector, without calculating the square root (faster).
				/// </summary>
				/// <param name="other">The other vector to calculate the squared distance to.</param>
				/// <returns>The squared distance between the two vectors.</returns>
				float DistancePow(const Vector4& other) const; // Checked

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
				/// <returns>A new Vector4 with the same direction but a magnitude of 1.</returns>
				Vector4 Normalized() const; // Checked

				/// <summary>
				/// Returns the perspective-transformed value of the vector using the formula: (x² + y² + z² + w²) / t.
				/// </summary>
				/// <param name="t">The perspective parameter. Must be greater than 0.</param>
				/// <returns>The result of the perspective function, or 0 if t is less than or equal to 0.</returns>
				float Perspective(float t) const; // Checked

				// -------------------------------------------------------------------------------- //	
				// Products
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Computes the scalar (dot) product of the current vector and another vector.
				/// </summary>
				/// <param name="other">The other vector to compute the dot product with.</param>
				/// <returns>The scalar (dot) product of the two vectors.</returns>
				float DotProductVector4(const Vector4& other) const; // Checked


				// -------------------------------------------------------------------------------- //	
				// Display
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Prints all values of the vector (x, y, z, w) to the console.
				/// </summary>
				/// <param name="">None</param>
				/// <returns>None</returns>
				inline void Print() const; // Checked

				std::array<float, 4> AsList() const; // Checked

				// -------------------------------------------------------------------------------- //	
				// Overload operators inline
				// -------------------------------------------------------------------------------- //

				Vector4 operator+(const Vector4& vec) const; // Checked
				Vector4 operator-(const Vector4& vec) const; // Checked
				Vector4 operator*(const Vector4& vec) const; // Checked
				Vector4 operator/(const Vector4& vec) const; // Checked
				Vector4& operator+=(const Vector4& vec); // Checked
				Vector4& operator-=(const Vector4& vec); // Checked
				Vector4& operator*=(const Vector4& vec); // Checked
				Vector4& operator/=(const Vector4& vec); // Checked

				Vector4 operator+(float scalar) const; // Checked
				Vector4 operator-(float scalar) const; // Checked
				Vector4 operator*(float scalar) const; // Checked
				Vector4 operator/(float scalar) const; // Checked
				Vector4& operator*=(float scalar); // Checked
				Vector4& operator/=(float scalar); // Checked

				Vector4 operator-() const; // Checked

				// cast operators
				explicit operator Vector3() const; // Checked
				explicit operator Vector2() const; // Checked

				float& operator[](int index); // Checked
				const float& operator[](int index) const;

				bool operator==(const Vector4& other) const; // Checked
				bool operator!=(const Vector4& other) const; // Checked

				Vector3 operator=(const Vector4& vec4); // Checked
			};
			Vector4 operator*(float scalar, const Vector4& vec); // Checked
			std::ostream& operator<<(std::ostream& os, const Vector4& vec); // Checked
		}
	}
}
