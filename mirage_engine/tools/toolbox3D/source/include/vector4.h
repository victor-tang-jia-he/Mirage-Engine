#pragma once

#include "vector3.h"
namespace MirageEngine
{
	namespace Core
	{
		namespace Math
		{
			struct Vector4
			{
				float X;
				float Y;
				float Z;
				float W;

				inline Vector4(const Vector3 _vec3, float _w = 1.f) noexcept
					: X(_vec3.X), Y(_vec3.Y), Z(_vec3.Z), W(_w) {
				}
				inline Vector4(float _x = 0.f, float _y = 0.f, float _z = 0.f, float _w = 1.f) noexcept
					: X(_x), Y(_y), Z(_z), W(_w) {
				}
				~Vector4() = default;

				// -------------------------------------------------------------------------------- //	
				// Elementary operations on vector components
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Returns the opposite of the vector (negating its components).
				/// </summary>
				/// <returns>The opposed vector.</returns>
				Vector4 Opposed() const;

				/// <summary>
				/// Adds the current vector with another vector.
				/// </summary>
				/// <param name="_other">The vector to add.</param>
				/// <returns>The resulting vector after addition.</returns>
				Vector4 AddVector4(const Vector4& _other) const;

				/// <summary>
				/// Addition of a _scalar to each component
				/// </summary>
				/// <param name="_scalar">The _scalar to add.</param>
				/// <returns>The resulting vector after addition of the _scalar.</returns>
				Vector4 AddScalar(float _scalar) const;

				/// <summary>
				/// Term product of two Vector
				/// </summary>
				/// <param name="_other">The vector to Element Wise Product with.</param>
				/// <returns>The resulting vector after Element Wise Product.</returns>
				Vector4 ElementWiseProduct(const Vector4& _other) const;

				/// <summary>
				/// Multiply of a _scalar to each component
				/// </summary>
				/// <param name="_scalar">The _scalar to Multiply.</param>
				/// <returns>The resulting vector after addition of the _scalar.</returns>
				Vector4 ProductScalar(float _scalar) const;

				bool NearlyEqual(const Vector4& _other, float _epsilon = EPSILON) const;
				// -------------------------------------------------------------------------------- //	
				// Geometric & standard calculations
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Calculates the midpoint between the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to find the midpoint with.</param>
				/// <returns>The midpoint vector.</returns>
				Vector4 Mid(const Vector4& _other) const;

				/// <summary>
				/// Computes the distance between the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to calculate the distance to.</param>
				/// <returns>The distance between the two vectors.</returns>
				float Distance(const Vector4& _other) const;

				/// <summary>
				/// Computes the squared distance between the current vector and another vector, without calculating the square root (faster).
				/// </summary>
				/// <param name="_other">The _other vector to calculate the squared distance to.</param>
				/// <returns>The squared distance between the two vectors.</returns>
				float DistancePow(const Vector4& _other) const;

				/// <summary>
				/// Returns the norm (magnitude) of the vector.
				/// </summary>
				/// <returns>The length of the vector.</returns>
				float Norm() const;

				/// <summary>
				/// Return the sum of the squares of its components.
				/// </summary>
				/// <returns>Return the Square of the vector.</returns>
				float SquareNorm() const;

				/// <summary>
				/// Returns a normalized version of the vector (unit vector).
				/// </summary>
				/// <returns>A new Vector4 with the same direction but a magnitude of 1.</returns>
				Vector4 Normalized() const;

				/// <summary>
				/// Returns the perspective-transformed value of the vector using the formula: (x² + y² + z² + w²) / _t.
				/// </summary>
				/// <param name="_t">The perspective parameter. Must be greater than 0.</param>
				/// <returns>The result of the perspective function, or 0 if _t is less than or equal to 0.</returns>
				float Perspective(float _t) const;

				// -------------------------------------------------------------------------------- //	
				// Products
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Computes the _scalar (dot) product of the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to compute the dot product with.</param>
				/// <returns>The _scalar (dot) product of the two vectors.</returns>
				float DotProductVector4(const Vector4& _other) const;


				// -------------------------------------------------------------------------------- //	
				// Display
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Prints all values of the vector (X, Y, Z, W) to the console.
				/// </summary>
				/// <param name="">None</param>
				/// <returns>None</returns>
				inline void Print() const;

				std::array<float, 4> AsList() const;

				// -------------------------------------------------------------------------------- //	
				// Overload operators inline
				// -------------------------------------------------------------------------------- //

				Vector4 operator+(const Vector4& _vec) const;
				Vector4 operator-(const Vector4& _vec) const;
				Vector4 operator*(const Vector4& _vec) const;
				Vector4 operator/(const Vector4& _vec) const;
				Vector4& operator+=(const Vector4& _vec);
				Vector4& operator-=(const Vector4& _vec);
				Vector4& operator*=(const Vector4& _vec);
				Vector4& operator/=(const Vector4& _vec);

				Vector4 operator+(float _scalar) const;
				Vector4 operator-(float _scalar) const;
				Vector4 operator*(float _scalar) const;
				Vector4 operator/(float _scalar) const;
				Vector4& operator*=(float _scalar);
				Vector4& operator/=(float _scalar);
				Vector4& operator+=(float _scalar);
				Vector4& operator-=(float _scalar);

				Vector4 operator-() const;

				// cast operators
				explicit operator Vector3() const;
				explicit operator Vector2() const;

				float& operator[](int _index);
				const float& operator[](int _index) const;

				bool operator==(const Vector4& _other) const;
				bool operator!=(const Vector4& _other) const;

				Vector3 operator=(const Vector4& vec4);
			};
			Vector4 operator*(float _scalar, const Vector4& _vec);
			std::ostream& operator<<(std::ostream& _os, const Vector4& _vec);
		}
	}
}
