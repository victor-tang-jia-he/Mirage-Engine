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
                        float _m00 = 0.f, float _m01 = 0.f, float _m02 = 0.f, float _m03 = 0.f, float _m10 = 0.f, float _m11 = 0.f, float _m12 = 0.f, float _m13 = 0.f, float _m20 = 0.f,
                        float _m21 = 0.f, float _m22 = 0.f, float _m23 = 0.f, float _m30 = 0.f, float _m31 = 0.f, float _m32 = 0.f, float _m33 = 0.f) noexcept;

                    // -------------------------------------------------------------------------------- //
                    // Basic Matrix Operations
                    // -------------------------------------------------------------------------------- //

                    /// <summary>
                    /// Returns the 4x4 identity matrix.
                    /// </summary>
                    static Matrix4 Identity();

                    /// <summary>
                    /// Returns the diagonal vector of the matrix.
                    /// </summary>
                    Vector4 Diagonal();

                    /// <summary>
                    /// Returns the additive inverse (negated version) of the matrix.
                    /// </summary>
                    Matrix4 Opposed() const;

                    /// <summary>
                    /// Returns the transpose of the matrix.
                    /// </summary>
                    Matrix4 Transpose() const;

                    // -------------------------------------------------------------------------------- //
                    // Arithmetic Operations on the Matrix
                    // -------------------------------------------------------------------------------- //

                    /// <summary>
                    /// Returns the sum of this matrix and another matrix.
                    /// </summary>
                    Matrix4 AddMatrix4(const Matrix4& _other) const;

                    /// <summary>
                    /// Returns the result of scalar multiplication on this matrix.
                    /// </summary>
                    Matrix4 ProductScalar(float _n) const;

                    /// <summary>
                    /// Returns the result of matrix multiplication with another 4x4 matrix.
                    /// </summary>
                    Matrix4 ProductMatrix4(const Matrix4& _other) const;

                    // -------------------------------------------------------------------------------- //
                    // Matrix Properties
                    // -------------------------------------------------------------------------------- //

                    /// <summary>
                    /// Returns the trace of the matrix (sum of the diagonal elements).
                    /// </summary>
                    float Trace();

                    /// <summary>
                    /// Returns the determinant of the matrix.
                    /// </summary>
                    float Determinant() const;

                    /// <summary>
                    /// Returns the inverse of the matrix if it exists.
                    /// </summary>
                    Matrix4 Inverse() const;

                    /// <summary>
                    /// Returns the 3x3 submatrix by excluding one row and one column.
                    /// </summary>
                    Matrix3 GetSubMatrix(int _excludeRow, int _excludeCol) const;

                    float GetSubMatrix3x3Determinant(int _excludeRow, int _excludeCol) const;

                    // -------------------------------------------------------------------------------- //
                    // Transform / Projection matrices
                    // -------------------------------------------------------------------------------- //

                    /// <summary>
                    /// Creates a transformation matrix from position, rotation (euler), and scale.
                    /// </summary>
                    static Matrix4 CreateTransformMatrix(const Vector3& _position = Vector3(0, 0, 0), const Vector3& _rotation = Vector3(0, 0, 0), const Vector3& _scale = Vector3(1, 1, 1));

                    static Matrix4 CreateTransformMatrix(const Vector3& _position, const Quaternion& _rotation, const Vector3& _scale);

                    /// <summary>
                    /// Creates a translation matrix.
                    /// </summary>
                    static Matrix4 CreateTranslationMatrix(const Vector3& _translate);

                    /// <summary>
                    /// Creates a scale matrix.
                    /// </summary>
                    static Matrix4 CreateScaleMatrix(const Vector3& _scale);

                    /// <summary>
                    /// Creates a rotation matrix around the X-axis.
                    /// </summary>
                    static Matrix4 CreateXRotationMatrix(float _rad);

                    /// <summary>
                    /// Creates a rotation matrix around the Y-axis.
                    /// </summary>
                    static Matrix4 CreateYRotationMatrix(float _rad);

                    /// <summary>
                    /// Creates a rotation matrix around the Z-axis.
                    /// </summary>
                    static Matrix4 CreateZRotationMatrix(float _rad);

                    static Matrix4 GetPerspectiveMatrix(unsigned int _width, unsigned int _height, float _near, float _far, float _fov);

                    static Matrix4 LookAt(Vector3 eye, Vector3 center, Vector3 up);

                    // -------------------------------------------------------------------------------- //
                    // Display / Utility
                    // -------------------------------------------------------------------------------- //

                    void Print() const;

                    std::array<float, 16> ToArray() const;

                    // -------------------------------------------------------------------------------- //
                    // Operators
                    // -------------------------------------------------------------------------------- //

                    Matrix4  operator*(const Matrix4& _other) const;
                    Matrix4  operator/(const Matrix4& _other) const;
                    Matrix4  operator+(const Matrix4& _other) const;
                    Matrix4  operator-(const Matrix4& _other) const;
                    Matrix4& operator*=(const Matrix4& _other);
                    Matrix4& operator/=(const Matrix4& _other);
                    Matrix4& operator+=(const Matrix4& _other);
                    Matrix4& operator-=(const Matrix4& _other);
                    Matrix4  operator*(float _n) const;
                    Matrix4& operator*=(float _n);
                    Vector3  operator*(const Vector4& _multiply) const;
                    bool     operator==(const Matrix4& _other) const;

                    float&         operator()(size_t _row, size_t _col);
                    const float&   operator()(size_t _row, size_t _col) const;
            };
        } // namespace Math
    } // namespace Core
} // namespace MirageEngine
