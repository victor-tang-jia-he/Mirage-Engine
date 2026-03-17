#include "matrix3.h"
#include <array>
#include <iostream>
#include <assert.h>

using namespace MirageEngine::Core::Math;
Matrix3::Matrix3(const Matrix3& copy) noexcept
{
	(*this)(0,0) = copy(0,0); (*this)(0,1) = copy(0,1); (*this)(0,2) = copy(0,2);
	(*this)(1,0) = copy(1,0); (*this)(1,1) = copy(1,1); (*this)(1,2) = copy(1,2);
	(*this)(2,0) = copy(2,0); (*this)(2,1) = copy(2,1); (*this)(2,2) = copy(2,2);
}
Matrix3::Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) noexcept
{
	(*this)(0, 0) = m00; (*this)(0, 1) = m01; (*this)(0, 2) = m02;
	(*this)(1, 0) = m10; (*this)(1, 1) = m11; (*this)(1, 2) = m12;
	(*this)(2, 0) = m20; (*this)(2, 1) = m21; (*this)(2, 2) = m22;
}
Matrix3 Matrix3::Identity()
{
	return Matrix3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f);
}
Vector3 Matrix3::Diagonal() const
{
	return Vector3((*this)(0,0), (*this)(1,1), (*this)(2,2));
}
float Matrix3::Trace() const
{
	return (*this)(0,0) + (*this)(1,1) + (*this)(2,2);
}
Matrix3 Matrix3::Opposed() const
{
	return Matrix3(
		-(*this)(0,0), -(*this)(0,1), -(*this)(0,2),
		-(*this)(1,0), -(*this)(1,1), -(*this)(1,2),
		-(*this)(2,0), -(*this)(2,1), -(*this)(2,2));
}
Matrix3 Matrix3::Transpose() const
{
	return Matrix3(
		(*this)(0,0), (*this)(1,0), (*this)(2,0),
		(*this)(0,1), (*this)(1,1), (*this)(2,1),
		(*this)(0,2), (*this)(1,2), (*this)(2,2));
}
Matrix3 Matrix3::AddMatrix3(const Matrix3& other) const
{
	return Matrix3(
		(*this)(0,0) + other(0,0),
		(*this)(0,1) + other(0,1),
		(*this)(0,2) + other(0,2),
		(*this)(1,0) + other(1,0),
		(*this)(1,1) + other(1,1),
		(*this)(1,2) + other(1,2),
		(*this)(2,0) + other(2,0),
		(*this)(2,1) + other(2,1),
		(*this)(2,2) + other(2,2));
}
Matrix3 Matrix3::ProductScalar(float n) const
{
	return Matrix3(
		(*this)(0,0) * n,
		(*this)(0,1) * n,
		(*this)(0,2) * n,
		(*this)(1,0) * n,
		(*this)(1,1) * n,
		(*this)(1,2) * n,
		(*this)(2,0) * n,
		(*this)(2,1) * n,
		(*this)(2,2) * n);
}
Matrix3 Matrix3::ProductMatrix3(const Matrix3& other) const
{
	return Matrix3(
		// Colonne 1
		(*this)(0,0) * other(0,0) + (*this)(0,1) * other(1,0) + (*this)(0,2) * other(2,0), // row 0
		(*this)(0,0) * other(0,1) + (*this)(0,1) * other(1,1) + (*this)(0,2) * other(2,1), // row 0
		(*this)(0,0) * other(0,2) + (*this)(0,1) * other(1,2) + (*this)(0,2) * other(2,2), // row 0

		// Colonne 2
		(*this)(1,0) * other(0,0) + (*this)(1,1) * other(1,0) + (*this)(1,2) * other(2,0), // row 1
		(*this)(1,0) * other(0,1) + (*this)(1,1) * other(1,1) + (*this)(1,2) * other(2,1), // row 1
		(*this)(1,0) * other(0,2) + (*this)(1,1) * other(1,2) + (*this)(1,2) * other(2,2), // row 1

		// Colonne 3
		(*this)(2,0) * other(0,0) + (*this)(2,1) * other(1,0) + (*this)(2,2) * other(2,0), // row 2
		(*this)(2,0) * other(0,1) + (*this)(2,1) * other(1,1) + (*this)(2,2) * other(2,1), // row 2
		(*this)(2,0) * other(0,2) + (*this)(2,1) * other(1,2) + (*this)(2,2) * other(2,2)  // row 2
	);
}
float Matrix3::Determinant() const
{
	return (*this)(0,0) * ((*this)(1,1) * (*this)(2,2) - (*this)(1,2) * (*this)(2,1)) -
		(*this)(0,1) * ((*this)(1,0) * (*this)(2,2) - (*this)(1,2) * (*this)(2,0)) +
		(*this)(0,2) * ((*this)(1,0) * (*this)(2,1) - (*this)(1,1) * (*this)(2,0));

}
Matrix3 Matrix3::RotateMatrix(float _x, float _y, float _z)
{
	Matrix3 rotX = CreateXRotationMatrix(_x);
	Matrix3 rotY = CreateYRotationMatrix(_y);
	Matrix3 rotZ = CreateZRotationMatrix(_z);
	return rotZ * rotY * rotX;
}
Matrix3 Matrix3::CreateXRotationMatrix(float rad)
{
	return Matrix3(1.f, 0.f, 0.f,
		0.f, cos(rad), -sin(rad),
		0.f, sin(rad), cos(rad));
}
Matrix3 Matrix3::CreateYRotationMatrix(float rad)
{
	return Matrix3(
		cos(rad), 0.f, sin(rad),
		0.f, 1.f, 0.f,
		-sin(rad), 0.f, cos(rad));
}
Matrix3 Matrix3::CreateZRotationMatrix(float rad)
{
	return Matrix3(cos(rad), -sin(rad), 0.f,
		sin(rad), cos(rad), 0.f,
		0, 0, 1.f);
}
std::array<float, 9> Matrix3::ToArray() const
{
	return {
		(*this)(0,0), (*this)(0,1), (*this)(0,2),
		(*this)(1,0), (*this)(1,1), (*this)(1,2),
		(*this)(2,0), (*this)(2,1), (*this)(2,2) };
}
Matrix3 Matrix3::Inverse() const
{
    const float det = Determinant();
    if (std::abs(det) < 1e-6f)
    {
        throw std::runtime_error("Matrix is not invertible (det == 0)");
    }

    Matrix3 inverse;
    const float invDet = 1.0f / det;

    for (int col = 0; col < 3; ++col)
    {
        for (int row = 0; row < 3; ++row)
        {
            const float cofactor =
                ((row + col) % 2 == 0 ? 1.f : -1.f) *
                GetSubMatrix(row, col).Determinant();

            inverse(row,col) = cofactor * invDet;
        }
    }

    return inverse;
}
Matrix2 Matrix3::GetSubMatrix(int excludeRow, int excludeCol) const
{
    Matrix2 sub;

    int subCol = 0;
    for (int col = 0; col < 3; ++col)
    {
        if (col == excludeCol) continue;

        int subRow = 0;
        for (int row = 0; row < 3; ++row)
        {
            if (row == excludeRow) continue;

            sub(subRow,subCol) = (*this)(row,col);
            ++subRow;
        }
        ++subCol;
    }

    return sub;
}
Matrix3 Matrix3::luDecomposition() const
{
    Matrix3 lower = Matrix3::Identity();
    Matrix3 upper = *this;

    // colonne 0
    lower(1,0) = upper(1,0) / upper(0,0);
    lower(2,0) = upper(2,0) / upper(0,0);

    for (int j = 0; j < 3; ++j)
    {
        upper(1,j) -= lower(1,0) * upper(0,j);
        upper(2,j) -= lower(2,0) * upper(0,j);
    }

    // colonne 1
    lower(2,1) = upper(2,1) / upper(1,1);

    for (int j = 1; j < 3; ++j)
    {
        upper(2,j) -= lower(2,1) * upper(1,j);
    }

    return lower * upper;
}
Matrix3 Matrix3::RotateMatrixVector(const Vector3& vec, float rad)
{
	float length = vec.Norm();
	float x = vec.x / length;
	float y = vec.y / length;
	float z = vec.z / length;

	float c = std::cos(rad);
	float s = std::sin(rad);
	float oneMinusC = 1 - c;

	// Rodrigues formula
	return Matrix3(
		c + x * x * oneMinusC, y * x * oneMinusC + z * s, z * x * oneMinusC - y * s,
        x * y * oneMinusC - z * s, c + y * y * oneMinusC, z * y * oneMinusC + x * s,
        x * z * oneMinusC + y * s, y * z * oneMinusC - x * s, c + z * z * oneMinusC);
}
void Matrix3::Print() const
{
	std::cout
		<< (*this)(0,0) << " " << (*this)(0,1) << " " << (*this)(0,2) << "\n"
		<< (*this)(1,0) << " " << (*this)(1,1) << " " << (*this)(1,2) << "\n"
		<< (*this)(2,0) << " " << (*this)(2,1) << " " << (*this)(2,2) << "\n";
}
Matrix3 Matrix3::operator*(const Matrix3& other) const
{
	return ProductMatrix3(other);
}
Matrix3 Matrix3::operator/(const Matrix3& other) const
{
	return ProductMatrix3(other.Inverse());
}
Matrix3 Matrix3::operator+(const Matrix3& other) const
{
	return AddMatrix3(other);
}
Matrix3 Matrix3::operator-(const Matrix3& other) const
{
	return AddMatrix3(other.Opposed());
}
Matrix3& Matrix3::operator*=(const Matrix3& other)
{
	float m1 = (*this)(0,0) * other(0,0) + (*this)(0,1) * other(1,0) + (*this)(0,2) * other(2,0);
	float m2 = (*this)(0,0) * other(0,1) + (*this)(0,1) * other(1,1) + (*this)(0,2) * other(2,1);
	float m3 = (*this)(0,0) * other(0,2) + (*this)(0,1) * other(1,2) + (*this)(0,2) * other(2,2);
	float m4 = (*this)(1,0) * other(0,0) + (*this)(1,1) * other(1,0) + (*this)(1,2) * other(2,0);
	float m5 = (*this)(1,0) * other(0,1) + (*this)(1,1) * other(1,1) + (*this)(1,2) * other(2,1);
	float m6 = (*this)(1,0) * other(0,2) + (*this)(1,1) * other(1,2) + (*this)(1,2) * other(2,2);
	float m7 = (*this)(2,0) * other(0,0) + (*this)(2,1) * other(1,0) + (*this)(2,2) * other(2,0);
	float m8 = (*this)(2,0) * other(0,1) + (*this)(2,1) * other(1,1) + (*this)(2,2) * other(2,1);
	float m9 = (*this)(2,0) * other(0,2) + (*this)(2,1) * other(1,2) + (*this)(2,2) * other(2,2);

	(*this)(0,0) = m1;
	(*this)(0,1) = m2;
	(*this)(0,2) = m3;
	(*this)(1,0) = m4;
	(*this)(1,1) = m5;
	(*this)(1,2) = m6;
	(*this)(2,0) = m7;
	(*this)(2,1) = m8;
	(*this)(2,2) = m9;

	return *this;
}
Matrix3& Matrix3::operator/=(const Matrix3& other)
{
	*this *= other.Inverse();
	return *this;
}
Matrix3& Matrix3::operator+=(const Matrix3& other)
{
	(*this)(0,0) += other(0,0);
	(*this)(0,1) += other(0,1);
	(*this)(0,2) += other(0,2);
	(*this)(1,0) += other(1,0);
	(*this)(1,1) += other(1,1);
	(*this)(1,2) += other(1,2);
	(*this)(2,0) += other(2,0);
	(*this)(2,1) += other(2,1);
	(*this)(2,2) += other(2,2);

	return *this;
}
Matrix3& Matrix3::operator-=(const Matrix3& other)
{
	(*this)(0,0) -= other(0,0);
	(*this)(0,1) -= other(0,1);
	(*this)(0,2) -= other(0,2);
	(*this)(1,0) -= other(1,0);
	(*this)(1,1) -= other(1,1);
	(*this)(1,2) -= other(1,2);
	(*this)(2,0) -= other(2,0);
	(*this)(2,1) -= other(2,1);
	(*this)(2,2) -= other(2,2);

	return *this;
}
Matrix3 Matrix3::operator*(float n)
{
	return ProductScalar(n);
}
Matrix3 Matrix3::operator*=(float n)
{
	(*this)(0,0) *= n;
	(*this)(0,1) *= n;
	(*this)(0,2) *= n;
	(*this)(1,0) *= n;
	(*this)(1,1) *= n;
	(*this)(1,2) *= n;
	(*this)(2,0) *= n;
	(*this)(2,1) *= n;
	(*this)(2,2) *= n;

	return *this;
}
Vector3 Matrix3::operator*(const Vector3& v)
{
	return Vector3(
		(*this)(0,0) * v.x + (*this)(0,1) * v.y + (*this)(0,2) * v.z,
		(*this)(1,0) * v.x + (*this)(1,1) * v.y + (*this)(1,2) * v.z,
		(*this)(2,0) * v.x + (*this)(2,1) * v.y + (*this)(2,2) * v.z
	);
}
bool Matrix3::operator==(const Matrix3& other) const
{
	return
		std::abs((*this)(0,0) - other(0,0)) < EPSILON &&
		std::abs((*this)(0,1) - other(0,1)) < EPSILON &&
		std::abs((*this)(0,2) - other(0,2)) < EPSILON &&
		std::abs((*this)(1,0) - other(1,0)) < EPSILON &&
		std::abs((*this)(1,1) - other(1,1)) < EPSILON &&
		std::abs((*this)(1,2) - other(1,2)) < EPSILON &&
		std::abs((*this)(2,0) - other(2,0)) < EPSILON &&
		std::abs((*this)(2,1) - other(2,1)) < EPSILON &&
		std::abs((*this)(2,2) - other(2,2)) < EPSILON;
}
Vector3 Matrix3::operator[](int index) const
{
	assert(index >= 0 && index < 3 && "Index out of range");
	return Vector3((*this)(index,0), (*this)(index, 1), (*this)(index, 2));
}
const float& Matrix3::operator()(size_t row, size_t col) const
{
	assert(row < 3 && col < 3);
	return m[col + row * 3];
}
float& Matrix3::operator()(size_t row, size_t col)
{
	assert(row < 3 && col < 3);
	return m[col + row * 3];
}