#pragma once

#include "matrix3.h"

namespace MirageEngine
{
	namespace Core
	{
		namespace Math
		{
			struct Matrix4
			{
				float m[16];

				Matrix4(const Matrix4& copy);
				Matrix4(
					float m00 = 0.f, float m01 = 0.f, float m02 = 0.f, float m03 = 0.f,
					float m10 = 0.f, float m11 = 0.f, float m12 = 0.f, float m13 = 0.f,
					float m20 = 0.f, float m21 = 0.f, float m22 = 0.f, float m23 = 0.f,
					float m30 = 0.f, float m31 = 0.f, float m32 = 0.f, float m33 = 0.f);

				// -------------------------------------------------------------------------------- //
				// Basic Matrix Operations
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Returns the 4x4 identity matrix.
				/// </summary>
				/// <returns>The identity matrix.</returns>
				static Matrix4 Identity();

				/// <summary>
				/// Returns the diagonal vector of the matrix.
				/// </summary>
				/// <returns>A Vector4 containing the diagonal elements.</returns>
				Vector4 Diagonal();

				/// <summary>
				/// Returns the additive inverse (negated version) of the matrix.
				/// </summary>
				/// <returns>The opposed matrix.</returns>
				Matrix4 Opposed() const;

				/// <summary>
				/// Returns the transpose of the matrix.
				/// </summary>
				/// <returns>The transposed matrix.</returns>
				Matrix4 Transpose() const;


				// -------------------------------------------------------------------------------- //	
				// Arithmetic Operations on the Matrix
				// -------------------------------------------------------------------------------- //	

				/// <summary>
				/// Returns the sum of this matrix and another matrix.
				/// </summary>
				/// <param name="other">The matrix to add.</param>
				/// <returns>The resulting matrix after addition.</returns>
				Matrix4 AddMatrix4(const Matrix4& other) const;

				/// <summary>
				/// Returns the result of scalar multiplication on this matrix.
				/// </summary>
				/// <param name="n">The scalar value to multiply the matrix by.</param>
				/// <returns>The resulting matrix after multiplication.</returns>
				Matrix4 ProductScalar(float n);

				/// <summary>
				/// Returns the result of matrix multiplication with another 4x4 matrix.
				/// </summary>
				/// <param name="other">The matrix to multiply with.</param>
				/// <returns>The resulting matrix after multiplication.</returns>
				Matrix4 ProductMatrix4(const Matrix4& other) const;


				// -------------------------------------------------------------------------------- //	
				// Matrix Properties
				// -------------------------------------------------------------------------------- //	

				/// <summary>
				/// Return the trace of the matrix (sum of the diagonal elements).
				/// </summary>
				/// <returns>The trace of the matrix.</returns>
				float Trace();

				/// <summary>
				/// Returns the determinant of the matrix.
				/// </summary>
				/// <returns>The determinant of the matrix.</returns>
				float Determinant() const;

				//float GetDeterminantSSE() const;

				/// <summary>
				/// Returns the inverse of the matrix if it exists.
				/// </summary>
				/// <returns>The inverse matrix, or an identity matrix if non-invertible.</returns>
				Matrix4 Inverse() const;

				/// <summary>
				/// Returns the 3x3 submatrix by excluding one row and one column.
				/// </summary>
				/// <param name="excludeRow">Row to exclude (0-3).</param>
				/// <param name="excludeCol">Column to exclude (0-3).</param>
				/// <returns>The resulting submatrix after exclusion of the specified row and column.</returns>
				Matrix3 GetSubMatrix(int excludeRow, int excludeCol) const;

				float GetSubMatrix3x3Determinant(int excludeRow, int excludeCol) const;

				// -------------------------------------------------------------------------------- //		
				// Rotation matrix      
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Creates a transformation matrix from position, rotation, and scale.
				/// </summary>
				/// <param name="position">Translation vector.</param>
				/// <param name="rotation">Rotation in radian (pitch, yaw, roll).</param>
				/// <param name="scale">Scaling vector.</param>
				static Matrix4 CreateTransformMatrix(const Vector3& position = Vector3(0, 0, 0), const Vector3& rotation = Vector3(0, 0, 0), const Vector3& scale = Vector3(1, 1, 1));

				/// <summary>
				/// Creates a translation matrix.
				/// </summary>
				/// <param name="translation">Translation vector.</param>
				static Matrix4 CreateTranslationMatrix(const Vector3& translation);

				/// <summary>
				/// Creates a scale matrix.
				/// </summary>
				/// <param name="scale">Scaling vector.</param>
				static Matrix4 CreateScaleMatrix(const Vector3& scale);

				/// <summary>
				/// Creates a rotation matrix around the X-axis.
				/// </summary>
				/// <param name="angle">Angle in Radian.</param>
				static Matrix4 CreateXRotationMatrix(float angle);

				/// <summary>
				/// Creates a rotation matrix around the Y-axis.
				/// </summary>
				/// <param name="angle">Angle in Radian.</param>
				static Matrix4 CreateYRotationMatrix(float angle);

				/// <summary>
				/// Creates a rotation matrix around the Z-axis.
				/// </summary>
				/// <param name="angle">Angle in Radian.</param>
				static Matrix4 CreateZRotationMatrix(float angle);

				static Matrix4 GetPerspectiveMatrix(unsigned int width, unsigned int height, float near, float far, float fov);

				// -------------------------------------------------------------------------------- //	
				// Display inline
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Prints all values of the Matrix4 to the console.
				/// </summary>
				void Print() const;

				std::array<float, 16> ToArray() const;

				// -------------------------------------------------------------------------------- //	
				// Overload operators inline
				// -------------------------------------------------------------------------------- //

				Matrix4 operator*(const Matrix4& other) const;
				Matrix4 operator/(const Matrix4& other)const;
				Matrix4 operator+(const Matrix4& other) const;
				Matrix4 operator-(const Matrix4& other) const;
				Matrix4& operator*=(const Matrix4& other);
				Matrix4& operator/=(const Matrix4& other);
				Matrix4& operator+=(const Matrix4& other);
				Matrix4& operator-=(const Matrix4& other);
				Matrix4 operator*(float n);
				Matrix4 operator*=(float n);
				Vector3 operator*(const Vector4& mult) const;
				bool operator==(const Matrix4& other) const;
				Vector4 operator[](int index) const;

				// -------------------------------------------------------------------------------- //

				float& operator()(size_t row, size_t col);
				const float& operator()(size_t row, size_t col) const;
			};
		}
	}
}
