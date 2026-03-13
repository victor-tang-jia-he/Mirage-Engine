#pragma once

#include "matrix3.h"

namespace Bwt
{
	namespace Core
	{
		namespace Maths
		{
			class Matrix4
			{
			public:
				float m[4][4];

				Matrix4(const Matrix4& copy);
				inline Matrix4(
					float m00 = 0.f, float m01 = 0.f, float m02 = 0.f, float m03 = 0.f,
					float m10 = 0.f, float m11 = 0.f, float m12 = 0.f, float m13 = 0.f,
					float m20 = 0.f, float m21 = 0.f, float m22 = 0.f, float m23 = 0.f,
					float m30 = 0.f, float m31 = 0.f, float m32 = 0.f, float m33 = 0.f)
				{
					m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
					m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
					m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
					m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
				};

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

				//static Matrix4 PerspectiveRH_NO(float fovy, float aspect, float zNear, float zFar)
				//{
				//	float const tanHalfFovy = tan(fovy / 2.f);

				//	Matrix4 Result = Matrix4::GetIdentity4();
				//	Result[0] = static_cast<float>(1) / (aspect * tanHalfFovy);
				//	Result[5] = static_cast<float>(1) / (tanHalfFovy);
				//	Result[10] = -(zFar + zNear) / (zFar - zNear);
				//	Result[11] = -static_cast<float>(1);
				//	Result[15] = -(static_cast<float>(2) * zFar * zNear) / (zFar - zNear);
				//	return Result;
				//}
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
				inline void Print() const
				{
					std::cout
						<< m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << "\n"
						<< m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << "\n"
						<< m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << "\n"
						<< m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << "\n";
				}

				inline std::array<float, 16> ToArray() const
				{
					return {
						m[0][0], m[0][1], m[0][2], m[0][3],
						m[1][0], m[1][1], m[1][2], m[1][3],
						m[2][0], m[2][1], m[2][2], m[2][3],
						m[3][0], m[3][1], m[3][2], m[3][3]
					};
				}

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

				float* operator[](size_t row)
				{
					assert(row < 4);
					return m[row];
				}

				const float* operator[](size_t row) const
				{
					assert(row < 4);
					return m[row];
				}
			};
		}
	}
}

#include "../Inline/Matrix4.inl"
