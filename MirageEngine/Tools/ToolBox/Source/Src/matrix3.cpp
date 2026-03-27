#include "matrix3.h"
#include <array>
#include <iostream>
#include <assert.h>

using namespace MirageEngine::Core::Math;
Matrix3::Matrix3(const Matrix3& _copy) noexcept
{
	(*this)(0,0) = _copy(0,0); (*this)(0,1) = _copy(0,1); (*this)(0,2) = _copy(0,2);
	(*this)(1,0) = _copy(1,0); (*this)(1,1) = _copy(1,1); (*this)(1,2) = _copy(1,2);
	(*this)(2,0) = _copy(2,0); (*this)(2,1) = _copy(2,1); (*this)(2,2) = _copy(2,2);
}
Matrix3::Matrix3(float _m00, float _m01, float _m02, float _m10, float _m11, float _m12, float _m20, float _m21, float _m22) noexcept
{
	(*this)(0, 0) = _m00; (*this)(0, 1) = _m01; (*this)(0, 2) = _m02;
	(*this)(1, 0) = _m10; (*this)(1, 1) = _m11; (*this)(1, 2) = _m12;
	(*this)(2, 0) = _m20; (*this)(2, 1) = _m21; (*this)(2, 2) = _m22;
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
Matrix3 Matrix3::AddMatrix3(const Matrix3& _other) const
{
	return Matrix3(
		(*this)(0,0) + _other(0,0),
		(*this)(0,1) + _other(0,1),
		(*this)(0,2) + _other(0,2),
		(*this)(1,0) + _other(1,0),
		(*this)(1,1) + _other(1,1),
		(*this)(1,2) + _other(1,2),
		(*this)(2,0) + _other(2,0),
		(*this)(2,1) + _other(2,1),
		(*this)(2,2) + _other(2,2));
}
Matrix3 Matrix3::ProductScalar(float _n) const
{
	return Matrix3(
		(*this)(0,0) * _n,
		(*this)(0,1) * _n,
		(*this)(0,2) * _n,
		(*this)(1,0) * _n,
		(*this)(1,1) * _n,
		(*this)(1,2) * _n,
		(*this)(2,0) * _n,
		(*this)(2,1) * _n,
		(*this)(2,2) * _n);
}
Matrix3 Matrix3::ProductMatrix3(const Matrix3& _other) const
{
	return Matrix3(
		(*this)(0,0) * _other(0,0) + (*this)(0,1) * _other(1,0) + (*this)(0,2) * _other(2,0),
		(*this)(0,0) * _other(0,1) + (*this)(0,1) * _other(1,1) + (*this)(0,2) * _other(2,1),
		(*this)(0,0) * _other(0,2) + (*this)(0,1) * _other(1,2) + (*this)(0,2) * _other(2,2),
		(*this)(1,0) * _other(0,0) + (*this)(1,1) * _other(1,0) + (*this)(1,2) * _other(2,0),
		(*this)(1,0) * _other(0,1) + (*this)(1,1) * _other(1,1) + (*this)(1,2) * _other(2,1),
		(*this)(1,0) * _other(0,2) + (*this)(1,1) * _other(1,2) + (*this)(1,2) * _other(2,2),
		(*this)(2,0) * _other(0,0) + (*this)(2,1) * _other(1,0) + (*this)(2,2) * _other(2,0),
		(*this)(2,0) * _other(0,1) + (*this)(2,1) * _other(1,1) + (*this)(2,2) * _other(2,1),
		(*this)(2,0) * _other(0,2) + (*this)(2,1) * _other(1,2) + (*this)(2,2) * _other(2,2) 
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
Matrix3 Matrix3::CreateXRotationMatrix(float _rad)
{
	return Matrix3(1.f, 0.f, 0.f,
		0.f, cos(_rad), -sin(_rad),
		0.f, sin(_rad), cos(_rad));
}
Matrix3 Matrix3::CreateYRotationMatrix(float _rad)
{
	return Matrix3(
		cos(_rad), 0.f, sin(_rad),
		0.f, 1.f, 0.f,
		-sin(_rad), 0.f, cos(_rad));
}
Matrix3 Matrix3::CreateZRotationMatrix(float _rad)
{
	return Matrix3(cos(_rad), -sin(_rad), 0.f,
		sin(_rad), cos(_rad), 0.f,
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
Matrix2 Matrix3::GetSubMatrix(int _excludeRow, int _excludeCol) const
{
    Matrix2 sub;

    int subCol = 0;
    for (int col = 0; col < 3; ++col)
    {
        if (col == _excludeCol) continue;

        int subRow = 0;
        for (int row = 0; row < 3; ++row)
        {
            if (row == _excludeRow) continue;

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
Matrix3 Matrix3::RotateMatrixVector(const Vector3& _vec, float _rad)
{
	float length = _vec.Norm();
	float x = _vec.X / length;
	float y = _vec.Y / length;
	float z = _vec.Z / length;

	float c = std::cos(_rad);
	float s = std::sin(_rad);
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
Matrix3 Matrix3::operator*(const Matrix3& _other) const
{
	return ProductMatrix3(_other);
}
Matrix3 Matrix3::operator/(const Matrix3& _other) const
{
	return ProductMatrix3(_other.Inverse());
}
Matrix3 Matrix3::operator+(const Matrix3& _other) const
{
	return AddMatrix3(_other);
}
Matrix3 Matrix3::operator-(const Matrix3& _other) const
{
	return AddMatrix3(_other.Opposed());
}
Matrix3& Matrix3::operator*=(const Matrix3& _other)
{
	float m1 = (*this)(0,0) * _other(0,0) + (*this)(0,1) * _other(1,0) + (*this)(0,2) * _other(2,0);
	float m2 = (*this)(0,0) * _other(0,1) + (*this)(0,1) * _other(1,1) + (*this)(0,2) * _other(2,1);
	float m3 = (*this)(0,0) * _other(0,2) + (*this)(0,1) * _other(1,2) + (*this)(0,2) * _other(2,2);
	float m4 = (*this)(1,0) * _other(0,0) + (*this)(1,1) * _other(1,0) + (*this)(1,2) * _other(2,0);
	float m5 = (*this)(1,0) * _other(0,1) + (*this)(1,1) * _other(1,1) + (*this)(1,2) * _other(2,1);
	float m6 = (*this)(1,0) * _other(0,2) + (*this)(1,1) * _other(1,2) + (*this)(1,2) * _other(2,2);
	float m7 = (*this)(2,0) * _other(0,0) + (*this)(2,1) * _other(1,0) + (*this)(2,2) * _other(2,0);
	float m8 = (*this)(2,0) * _other(0,1) + (*this)(2,1) * _other(1,1) + (*this)(2,2) * _other(2,1);
	float m9 = (*this)(2,0) * _other(0,2) + (*this)(2,1) * _other(1,2) + (*this)(2,2) * _other(2,2);

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
Matrix3& Matrix3::operator/=(const Matrix3& _other)
{
	*this *= _other.Inverse();
	return *this;
}
Matrix3& Matrix3::operator+=(const Matrix3& _other)
{
	(*this)(0,0) += _other(0,0);
	(*this)(0,1) += _other(0,1);
	(*this)(0,2) += _other(0,2);
	(*this)(1,0) += _other(1,0);
	(*this)(1,1) += _other(1,1);
	(*this)(1,2) += _other(1,2);
	(*this)(2,0) += _other(2,0);
	(*this)(2,1) += _other(2,1);
	(*this)(2,2) += _other(2,2);

	return *this;
}
Matrix3& Matrix3::operator-=(const Matrix3& _other)
{
	(*this)(0,0) -= _other(0,0);
	(*this)(0,1) -= _other(0,1);
	(*this)(0,2) -= _other(0,2);
	(*this)(1,0) -= _other(1,0);
	(*this)(1,1) -= _other(1,1);
	(*this)(1,2) -= _other(1,2);
	(*this)(2,0) -= _other(2,0);
	(*this)(2,1) -= _other(2,1);
	(*this)(2,2) -= _other(2,2);

	return *this;
}
Matrix3 Matrix3::operator*(float _n)
{
	return ProductScalar(_n);
}
Matrix3 Matrix3::operator*=(float _n)
{
	(*this)(0,0) *= _n;
	(*this)(0,1) *= _n;
	(*this)(0,2) *= _n;
	(*this)(1,0) *= _n;
	(*this)(1,1) *= _n;
	(*this)(1,2) *= _n;
	(*this)(2,0) *= _n;
	(*this)(2,1) *= _n;
	(*this)(2,2) *= _n;

	return *this;
}
Vector3 Matrix3::operator*(const Vector3& v)
{
	return Vector3(
		(*this)(0,0) * v.X + (*this)(0,1) * v.Y + (*this)(0,2) * v.Z,
		(*this)(1,0) * v.X + (*this)(1,1) * v.Y + (*this)(1,2) * v.Z,
		(*this)(2,0) * v.X + (*this)(2,1) * v.Y + (*this)(2,2) * v.Z
	);
}
bool Matrix3::operator==(const Matrix3& _other) const
{
	return
		std::abs((*this)(0,0) - _other(0,0)) < EPSILON &&
		std::abs((*this)(0,1) - _other(0,1)) < EPSILON &&
		std::abs((*this)(0,2) - _other(0,2)) < EPSILON &&
		std::abs((*this)(1,0) - _other(1,0)) < EPSILON &&
		std::abs((*this)(1,1) - _other(1,1)) < EPSILON &&
		std::abs((*this)(1,2) - _other(1,2)) < EPSILON &&
		std::abs((*this)(2,0) - _other(2,0)) < EPSILON &&
		std::abs((*this)(2,1) - _other(2,1)) < EPSILON &&
		std::abs((*this)(2,2) - _other(2,2)) < EPSILON;
}
Vector3 Matrix3::operator[](int _index) const
{
	assert(_index >= 0 && _index < 3 && "Index out of range");
	return Vector3((*this)(_index,0), (*this)(_index, 1), (*this)(_index, 2));
}
const float& Matrix3::operator()(size_t _row, size_t _col) const
{
	assert(_row < 3 && _col < 3);
	return M[_col + _row * 3];
}
float& Matrix3::operator()(size_t _row, size_t _col)
{
	assert(_row < 3 && _col < 3);
	return M[_col + _row * 3];
}