#pragma once

#include "matrix3.h"

namespace MirageEngine
{
	namespace Core
	{
		namespace Math
		{
			struct Quaternion;
			struct Matrix4
			{
				float M[16];

				Matrix4(const Matrix4& _copy) noexcept;
				Matrix4(
					float _m00 = 0.f, float _m01 = 0.f, float _m02 = 0.f, float _m03 = 0.f,
					float _m10 = 0.f, float _m11 = 0.f, float _m12 = 0.f, float _m13 = 0.f,
					float _m20 = 0.f, float _m21 = 0.f, float _m22 = 0.f, float _m23 = 0.f,
					float _m30 = 0.f, float _m31 = 0.f, float _m32 = 0.f, float _m33 = 0.f) noexcept;
				Matrix4(float* _m) noexcept;

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
				/// <param name="_other">The matrix to add.</param>
				/// <returns>The resulting matrix after addition.</returns>
				Matrix4 AddMatrix4(const Matrix4& _other) const;

				/// <summary>
				/// Returns the result of _scalar multiplication on this matrix.
				/// </summary>
				/// <param name="_n">The _scalar value to multiply the matrix by.</param>
				/// <returns>The resulting matrix after multiplication.</returns>
				Matrix4 ProductScalar(float _n);

				/// <summary>
				/// Returns the result of matrix multiplication with another 4x4 matrix.
				/// </summary>
				/// <param name="_other">The matrix to multiply with.</param>
				/// <returns>The resulting matrix after multiplication.</returns>
				Matrix4 ProductMatrix4(const Matrix4& _other) const;


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
				/// Returns the 3x3 submatrix by excluding one _row and one column.
				/// </summary>
				/// <param name="_excludeRow">Row to exclude (0-3).</param>
				/// <param name="_excludeCol">Column to exclude (0-3).</param>
				/// <returns>The resulting submatrix after exclusion of the specified _row and column.</returns>
				Matrix3 GetSubMatrix(int _excludeRow, int _excludeCol) const;

				float GetSubMatrix3x3Determinant(int _excludeRow, int _excludeCol) const;

				// -------------------------------------------------------------------------------- //		
				// Rotation matrix      
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Creates a transformation matrix from _position, _rotation, and _scale.
				/// </summary>
				/// <param name="_position">Translate vector.</param>
				/// <param name="_rotation">Rotation in radian (pitch, yaw, roll).</param>
				/// <param name="_scale">Scaling vector.</param>
				static Matrix4 CreateTransformMatrix(const Vector3& _position = Vector3(0, 0, 0), const Vector3& _rotation = Vector3(0, 0, 0), const Vector3& _scale = Vector3(1, 1, 1));

				/// <summary>
				/// Creates a _translate matrix.
				/// </summary>
				/// <param name="_translate">Translate vector.</param>
				static Matrix4 CreateTranslationMatrix(const Vector3& _translate);

				/// <summary>
				/// Creates a _scale matrix.
				/// </summary>
				/// <param name="_scale">Scaling vector.</param>
				static Matrix4 CreateScaleMatrix(const Vector3& _scale);

				/// <summary>
				/// Creates a _rotation matrix around the X-axis.
				/// </summary>
				/// <param name="_angle">Angle in Radian.</param>
				static Matrix4 CreateXRotationMatrix(float _rad);

				/// <summary>
				/// Creates a _rotation matrix around the Y-axis.
				/// </summary>
				/// <param name="_angle">Angle in Radian.</param>
				static Matrix4 CreateYRotationMatrix(float _rad);

				/// <summary>
				/// Creates a _rotation matrix around the Z-axis.
				/// </summary>
				/// <param name="_angle">Angle in Radian.</param>
				static Matrix4 CreateZRotationMatrix(float _rad);

				static Matrix4 GetPerspectiveMatrix(unsigned int _width, unsigned int _height, float _near, float _far, float _fov);

				static Matrix4 LookAt(Vector3 eye, Vector3 center, Vector3 up);


				// -------------------------------------------------------------------------------- //		
				// Decomposition (For Transform usage)
				// -------------------------------------------------------------------------------- //

				/// <summary> Extracts the translation part as a Vector3. </summary>
				Vector3 GetTranslation() const;

				/// <summary> Extracts the scale part as a Vector3. </summary>
				Vector3 GetScale() const;

				/// <summary> Extracts the rotation part as a Quaternion. </summary>
				Quaternion GetRotation() const;



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

				Matrix4 operator*(const Matrix4& _other) const;
				Matrix4 operator/(const Matrix4& _other)const;
				Matrix4 operator+(const Matrix4& _other) const;
				Matrix4 operator-(const Matrix4& _other) const;
				Matrix4& operator*=(const Matrix4& _other);
				Matrix4& operator/=(const Matrix4& _other);
				Matrix4& operator+=(const Matrix4& _other);
				Matrix4& operator-=(const Matrix4& _other);
				Matrix4 operator*(float _n);
				Matrix4 operator*=(float _n);
				Vector3 operator*(const Vector4& _multiply) const;
				bool operator==(const Matrix4& _other) const;
				Vector4 operator[](int _index) const;

				// -------------------------------------------------------------------------------- //

				float& operator()(size_t _row, size_t _col);
				const float& operator()(size_t _row, size_t _col) const;
			};
		}
	}
}
