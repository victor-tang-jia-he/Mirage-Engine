#pragma once

#include "matrix2.h"

namespace MirageEngine
{
	namespace Core
	{
		namespace Math
		{
			struct Matrix3
			{
				float M[9]; //3x3

				Matrix3(const Matrix3& _copy) noexcept;
				Matrix3(
					float _m00 = 0.f, float _m01 = 0.f, float _m02 = 0.f,
					float _m10 = 0.f, float _m11 = 0.f, float _m12 = 0.f,
					float _m20 = 0.f, float _m21 = 0.f, float _m22 = 0.f) noexcept;
					


				// -------------------------------------------------------------------------------- //	
				// Basic Matrix Operations
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Return the identity matrix of 3x3.
				/// </summary>
				/// <returns>The identity matrix.</returns>
				static Matrix3 Identity();

				/// <summary>
				/// Return the diagonal elements of the matrix as a Vector3.
				/// </summary>
				/// <returns>A Vector3 containing the diagonal elements.</returns>
				Vector3 Diagonal() const;

				/// <summary>
				/// Return the opposed (negated) matrix.
				/// </summary>
				/// <returns>The opposed matrix.</returns>
				Matrix3 Opposed() const;

				/// <summary>
				/// Return the transposed matrix.
				/// </summary>
				/// <returns>The transposed matrix.</returns>
				Matrix3 Transpose() const;


				// -------------------------------------------------------------------------------- //	
				// Arithmetic Operations on the Matrix
				// -------------------------------------------------------------------------------- //	

				/// <summary>
				/// Add another matrix to the current matrix.
				/// </summary>
				/// <param name="_other">The matrix to add.</param>
				/// <returns>The resulting matrix after addition.</returns>
				Matrix3 AddMatrix3(const Matrix3& _other) const;

				/// <summary>
				/// Multiply the matrix by a _scalar value.
				/// </summary>
				/// <param name="_n">The _scalar value to multiply the matrix by.</param>
				/// <returns>The resulting matrix after multiplication.</returns>
				Matrix3 ProductScalar(float _n) const;

				/// <summary>
				/// Multiply the matrix by another matrix.
				/// </summary>
				/// <param name="_other">The matrix to multiply with.</param>
				/// <returns>The resulting matrix after multiplication.</returns>
				Matrix3 ProductMatrix3(const Matrix3& _other) const;


				// -------------------------------------------------------------------------------- //	
				// Matrix Properties
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
				/// Returns the inverse of the matrix if it exists.
				/// </summary>
				/// <returns>The inverse matrix, or an identity matrix if non-invertible.</returns>
				Matrix3 Inverse() const;

				/// <summary>
				/// Return a submatrix by excluding the specified _row and column.
				/// </summary>
				/// <param name="_excludeRow">Row to exclude (0-2).</param>
				/// <param name="_excludeCol">Column to exclude (0-2).</param>
				/// <returns>The resulting submatrix after exclusion of the specified _row and column.</returns>
				Matrix2 GetSubMatrix(int _excludeRow, int _excludeCol) const;


				// -------------------------------------------------------------------------------- //		
				// Lu Methode      
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Performs LU decomposition of the matrix.
				/// </summary>
				/// <returns>A matrix containing the upper triangular matrix (U), 
				/// with the lower triangular matrix (L) stored implicitly.</returns>
				Matrix3 luDecomposition() const;


				// -------------------------------------------------------------------------------- //		
				// Rotation matrix      
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Create an RST (Rotation - Scale - Transpose)
				/// Angle in rad.
				/// </summary>
				static Matrix3 RotateMatrix(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);

				/// <summary>
				/// Create an MatrixRotation axe X
				/// Angle in rad.
				/// </summary>
				static Matrix3 CreateXRotationMatrix(float _rad);

				/// <summary>
				/// Create an MatrixRotation axe Y
				/// Angle in rad.
				/// </summary>
				static Matrix3 CreateYRotationMatrix(float _rad);

				/// <summary>
				/// Create an MatrixRotation axe Z
				/// Angle in rad.
				/// </summary>
				static Matrix3 CreateZRotationMatrix(float _rad);

				/// <summary>
				/// Create an MatrixRotation around a point
				/// </summary>
				static Matrix3 RotateMatrixVector(const Vector3& _vec, float _rad);


				// -------------------------------------------------------------------------------- //	
				// Display inline
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Prints all values of the Matrix3 to the console.
				/// </summary>
				void Print() const;

				std::array<float, 9> ToArray() const;
				// -------------------------------------------------------------------------------- //	
				// Overload operators inline
				// -------------------------------------------------------------------------------- //

				Matrix3 operator*(const Matrix3& _other) const;
				Matrix3 operator/(const Matrix3& _other)const;
				Matrix3 operator+(const Matrix3& _other) const;
				Matrix3 operator-(const Matrix3& _other) const;

				Matrix3& operator*=(const Matrix3& _other);
				Matrix3& operator/=(const Matrix3& _other);
				Matrix3& operator+=(const Matrix3& _other);
				Matrix3& operator-=(const Matrix3& _other);

				Matrix3 operator*(float _n);
				Matrix3 operator*=(float _n);

				bool operator==(const Matrix3& _other) const;

				Vector3 operator*(const Vector3& _m);
				Vector3 operator[](int _index) const;

				// -------------------------------------------------------------------------------- //

				float& operator()(size_t _row, size_t _col);
				const float& operator()(size_t _row, size_t _col) const;
			};
		}
	}
}