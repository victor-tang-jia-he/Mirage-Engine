#pragma once

#include "vector4.h"

namespace MirageEngine
{
	namespace Core
	{
		namespace Math
		{
			class Matrix2
			{
			public:
				float m[2][2]; // m[col][row]

				Matrix2(const Matrix2& copy);
				Matrix2(float m00 = 0.f, float m01 = 0.f, float m10 = 0.f, float m11 = 0.f);

				/// <summary>
				/// Return the identity matrix of 2x2.
				/// </summary>
				/// <returns>The identity matrix.</returns>
				static Matrix2 Identity();

				// -------------------------------------------------------------------------------- //	
				// Basic matrix operations
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Return the diagonal elements of the matrix as a Vector2.
				/// </summary>
				/// <returns>A Vector2 containing the diagonal elements.</returns>
				Vector2 Diagonal() const;

				/// <summary>
				/// Return the opposed (negated) matrix.
				/// </summary>
				/// <returns>The opposed matrix.</returns>
				Matrix2 Opposed() const;

				/// <summary>
				/// Return the transposed matrix.
				/// </summary>
				/// <returns>The transposed matrix.</returns>
				Matrix2 Transpose() const;


				// -------------------------------------------------------------------------------- //	
				// Arithmetic operations on the matrix
				// -------------------------------------------------------------------------------- //	

				/// <summary>
				/// Add another matrix to the current matrix.
				/// </summary>
				/// <param name="other">The matrix to add.</param>
				/// <returns>The resulting matrix after addition.</returns>
				Matrix2 AddMatrix2(const Matrix2& other) const;

				/// <summary>
				/// Multiply the matrix by a scalar value.
				/// </summary>
				/// <param name="n">The scalar value to multiply the matrix by.</param>
				/// <returns>The resulting matrix after multiplication.</returns>
				Matrix2 ProductScalar(float n) const;

				/// <summary>
				/// Multiply the matrix by another matrix.
				/// </summary>
				/// <param name="other">The matrix to multiply with.</param>
				/// <returns>The resulting matrix after multiplication.</returns>
				Matrix2 ProductMatrix2(const Matrix2& other) const;


				// -------------------------------------------------------------------------------- //	
				// Matrix properties
				// -------------------------------------------------------------------------------- //	

				/// <summary>
				/// Return the trace of the matrix (sum of the diagonal elements).
				/// </summary>
				/// <returns>The trace of the matrix.</returns>
				float Trace() const;

				/// <summary>
				/// Return the determinant of the matrix.
				/// </summary>
				/// <returns>The determinant of the matrix.</returns>
				float Determinant() const;

				/// <summary>
				/// Return the inverse of the matrix.
				/// </summary>
				/// <returns>The inverse matrix, or an Null matrix if non-invertible.</returns>
				Matrix2 Inverse() const;


				// -------------------------------------------------------------------------------- //	
				// Rotation matrix
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Rotate the matrix by a given angle (in Radian).
				/// </summary>
				/// <param name="angle">The angle by which to rotate the matrix.</param>
				/// <returns>The resulting rotated matrix.</returns>
				Matrix2 Rotate(float angle);

				// -------------------------------------------------------------------------------- //	
				// Display
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Prints all values of the Matrix2 to the console.
				/// </summary>
				void Print() const;


				// -------------------------------------------------------------------------------- //	
				// Overload operators inline
				// -------------------------------------------------------------------------------- //

				Matrix2 operator*(const Matrix2& other)const;
				Matrix2 operator/(const Matrix2& other)const;
				Matrix2 operator+(const Matrix2& other) const;
				Matrix2 operator-(const Matrix2& other) const;

				Matrix2& operator*=(const Matrix2& other);
				Matrix2& operator/=(const Matrix2& other);
				Matrix2& operator+=(const Matrix2& other);
				Matrix2& operator-=(const Matrix2& other);

				Matrix2 operator*(float other) const;
				Matrix2& operator*=(float other);

				bool operator==(const Matrix2& other) const;

				float* operator[](size_t row);

				const float* operator[](size_t row) const;

			};
		}
	}
}