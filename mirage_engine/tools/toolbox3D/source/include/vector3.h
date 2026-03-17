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

				float X{ 0 };
				float Y{ 0 };
				float Z{ 0 };

				Vector3() = default;
				inline Vector3(float _x, float _y) noexcept
					: X(_x), Y(_y), Z(0) {
				}
				inline Vector3(Vector2 _vec2, float _z) noexcept
					: X(_vec2.X), Y(_vec2.Y), Z(_z) {
				}
				inline Vector3(float _x, float _y, float _z) noexcept
					: X(_x), Y(_y), Z(_z) {
				}
				~Vector3() = default;


				// -------------------------------------------------------------------------------- //	
				// Elementary operations on vector components
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Returns the opposite of the vector (negating its components).
				/// </summary>
				/// <returns>The opposed vector.</returns>
				Vector3 Opposed() const;

				/// <summary>
				/// Adds the current vector with another vector.
				/// </summary>
				/// <param name="_other">The vector to add.</param>
				/// <returns>The resulting vector after addition.</returns>
				Vector3 AddVector3(const Vector3& _other) const;

				/// <summary>
				/// Addition of a _scalar to each component
				/// </summary>
				/// <param name="_scalar">The _scalar to add.</param>
				/// <returns>The resulting vector after addition of the _scalar.</returns>
				Vector3 AddScalar(float _scalar) const;

				/// <summary>
				/// Term product of two Vector
				/// </summary>
				/// <param name="_other">The vector to Element Wise Product with.</param>
				/// <returns>The resulting vector after Element Wise Product.</returns>
				Vector3 ElementWiseProduct(const Vector3& _other) const;

				/// <summary>
				/// Multiply of a _scalar to each component
				/// </summary>
				/// <param name="_scalar">The _scalar to Multiply.</param>
				/// <returns>The resulting vector after addition of the _scalar.</returns>
				Vector3 ProductScalar(float _scalar) const;

				bool NearlyEqual(const Vector3& _other, float _epsilon = EPSILON) const;

				bool IsNull() const;

				// -------------------------------------------------------------------------------- //	
				// Geometric & standard calculations
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Calculates the midpoint between the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to find the midpoint with.</param>
				/// <returns>The midpoint vector.</returns>
				Vector3 Mid(const Vector3& _other) const;

				/// <summary>
				/// Computes the distance between the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to calculate the distance to.</param>
				/// <returns>The distance between the two vectors.</returns>
				float Distance(const Vector3& _other) const;

				/// <summary>
				/// Computes the squared distance between the current vector and another vector, without calculating the square root (faster).
				/// </summary>
				/// <param name="_other">The _other vector to calculate the squared distance to.</param>
				/// <returns>The squared distance between the two vectors.</returns>
				float DistancePow(const Vector3& _other) const;

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
				/// <returns>A new Vector3 with the same direction but a magnitude of 1.</returns>
				Vector3 Normalized() const;

				/// <summary>
				/// Returns the perspective-transformed value of the vector using the formula: (x² + y² + z²) / _t.
				/// </summary>
				/// <param name="_t">The perspective parameter. Must be greater than 0.</param>
				/// <returns>The result of the perspective function, or 0 if _t is less than or equal to 0.</returns>
				float Perspective(float _t) const;

				/// <summary>
				/// Return the Reflected Vector.
				/// </summary>
				/// <param name="_I">vecteur incident</param>
				/// <param name="_N">Surface Normal</param>
				/// <returns>The result of the perspective function, or 0 if _t is less than or equal to 0.</returns>
				static Vector3 Reflect(Vector3 _I, Vector3 _N);

				// -------------------------------------------------------------------------------- //	
				// Products & angles
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Computes the _scalar (dot) product of the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to compute the dot product with.</param>
				/// <returns>The _scalar (dot) product of the two vectors.</returns>
				float DotProductVector3(const Vector3& _other) const;

				/// <summary>
				/// Computes the 3D cross product between the current vector and another vector.
				/// </summary>
				/// <param name="_other">The _other vector to compute the cross product with.</param>
				/// <returns>The _scalar result of the 3D cross product.</returns>
				Vector3 CrossProductVector3(const Vector3& _other) const;

				/// <summary>
				/// Calculates the _angle (in radian) between the current vector and another vector.
				/// </summary>
				/// <param name="_v2">The _other vector to calculate the _angle with.</param>
				/// <returns>The _angle in degrees between the two vectors.</returns>
				float Angle(const Vector3& _v2) const;


				// -------------------------------------------------------------------------------- //	
				// Transform
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Rotates the vector by the specified _angle (in degrees).
				/// </summary>
				/// <param name="_angleX">The _angle X in degrees to rotate the vector.</param>
				/// <param name="_angleY">The _angle Y in degrees to rotate the vector.</param>
				/// <param name="_angleZ">The _angle Z in degrees to rotate the vector.</param>
				/// <returns>None</returns>
				Vector3 Rotate(float _angleX, float _angleY, float _angleZ) const;


				// -------------------------------------------------------------------------------- //	
				// Display
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Prints all values of the vector (X, Y, Z) to the console.
				/// </summary>
				/// <param name="">None</param>
				/// <returns>None</returns>
				void Print() const;

				std::array<float, 3> AsList() const;

				// -------------------------------------------------------------------------------- //	
				// Overload operators inline
				// -------------------------------------------------------------------------------- //

				Vector3 operator+(const Vector3& _vec) const;
				Vector3 operator-(const Vector3& _vec) const;
				Vector3 operator*(const Vector3& _vec) const;
				Vector3 operator/(const Vector3& _vec) const;
				Vector3& operator+=(const Vector3& _vec);
				Vector3& operator-=(const Vector3& _vec);
				Vector3& operator*=(const Vector3& _vec);
				Vector3& operator/=(const Vector3& _vec);
				
				Vector3 operator+(float _scalar) const;
				Vector3 operator-(float _scalar) const;
				Vector3 operator*(float _scalar) const;
				Vector3 operator/(float _scalar) const;
				Vector3& operator*=(float _scalar);
				Vector3& operator/=(float _scalar);
				Vector3& operator+=(float _scalar);
				Vector3& operator-=(float _scalar);

				Vector3 operator-() const;

				explicit operator Vector2() const;

				// Checked
				float& operator[](int _index);
				const float& operator[](int _index) const;

				bool operator==(const Vector3& _other) const;
				bool operator!=(const Vector3& _other) const;
			};
			Vector3 operator*(float _scalar, const Vector3& _vec);
			std::ostream& operator<<(std::ostream& _os, const Vector3& _vec);
		}
	}
}
