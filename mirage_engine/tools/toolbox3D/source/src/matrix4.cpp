#include "matrix4.h"
#include <iostream>
#include <array>
#include <assert.h>

using namespace MirageEngine::Core::Math;
Matrix4::Matrix4(const Matrix4& _copy) noexcept
{
	(*this)(0,0) = _copy(0,0); (*this)(0,1) = _copy(0,1); (*this)(0,2) = _copy(0,2); (*this)(0,3) = _copy(0,3);
	(*this)(1,0) = _copy(1,0); (*this)(1,1) = _copy(1,1); (*this)(1,2) = _copy(1,2); (*this)(1,3) = _copy(1,3);
	(*this)(2,0) = _copy(2,0); (*this)(2,1) = _copy(2,1); (*this)(2,2) = _copy(2,2); (*this)(2,3) = _copy(2,3);
	(*this)(3,0) = _copy(3,0); (*this)(3,1) = _copy(3,1); (*this)(3,2) = _copy(3,2); (*this)(3,3) = _copy(3,3);
}
Matrix4::Matrix4(
	float _m00 , float _m01 , float _m02 , float _m03,
	float _m10 , float _m11 , float _m12 , float _m13,
	float _m20 , float _m21 , float _m22 , float _m23,
	float _m30 , float _m31 , float _m32 , float _m33) noexcept
{
	(*this)(0,0) = _m00; (*this)(0,1) = _m01; (*this)(0,2) = _m02; (*this)(0,3) = _m03;
	(*this)(1,0) = _m10; (*this)(1,1) = _m11; (*this)(1,2) = _m12; (*this)(1,3) = _m13;
	(*this)(2,0) = _m20; (*this)(2,1) = _m21; (*this)(2,2) = _m22; (*this)(2,3) = _m23;
	(*this)(3,0) = _m30; (*this)(3,1) = _m31; (*this)(3,2) = _m32; (*this)(3,3) = _m33;
};
Matrix4 Matrix4::Identity()
{
	return Matrix4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}
Vector4 Matrix4::Diagonal()
{
	return Vector4((*this)(0,0), (*this)(1,1), (*this)(2,2), (*this)(3,3));
}
float Matrix4::Trace()
{
	Vector4 _vec = Diagonal();
	return _vec.X + _vec.Y + _vec.Z + _vec.W;
}
Matrix4 Matrix4::Opposed() const
{
	return	Matrix4(
		-(*this)(0,0), -(*this)(0,1), -(*this)(0,2), -(*this)(0,3),
		-(*this)(1,0), -(*this)(1,1), -(*this)(1,2), -(*this)(1,3),
		-(*this)(2,0), -(*this)(2,1), -(*this)(2,2), -(*this)(2,3),
		-(*this)(3,0), -(*this)(3,1), -(*this)(3,2), -(*this)(3,3));
}
Matrix4 Matrix4::Transpose() const
{
	return Matrix4(
		(*this)(0,0), (*this)(1,0), (*this)(2,0), (*this)(3,0),
		(*this)(0,1), (*this)(1,1), (*this)(2,1), (*this)(3,1),
		(*this)(0,2), (*this)(1,2), (*this)(2,2), (*this)(3,2),
		(*this)(0,3), (*this)(1,3), (*this)(2,3), (*this)(3,3));
}
Matrix4 Matrix4::AddMatrix4(const Matrix4& _other) const
{
	return	Matrix4(
		(*this)(0,0) + _other(0,0), (*this)(0,1) + _other(0,1), (*this)(0,2) + _other(0,2), (*this)(0,3) + _other(0,3),
		(*this)(1,0) + _other(1,0), (*this)(1,1) + _other(1,1), (*this)(1,2) + _other(1,2), (*this)(1,3) + _other(1,3),
		(*this)(2,0) + _other(2,0), (*this)(2,1) + _other(2,1), (*this)(2,2) + _other(2,2), (*this)(2,3) + _other(2,3),
		(*this)(3,0) + _other(3,0), (*this)(3,1) + _other(3,1), (*this)(3,2) + _other(3,2), (*this)(3,3) + _other(3,3));
}
Matrix4 Matrix4::ProductScalar(float _n)
{
	return	Matrix4(
		(*this)(0,0) * _n, (*this)(0,1) * _n, (*this)(0,2) * _n, (*this)(0,3) * _n,
		(*this)(1,0) * _n, (*this)(1,1) * _n, (*this)(1,2) * _n, (*this)(1,3) * _n,
		(*this)(2,0) * _n, (*this)(2,1) * _n, (*this)(2,2) * _n, (*this)(2,3) * _n,
		(*this)(3,0) * _n, (*this)(3,1) * _n, (*this)(3,2) * _n, (*this)(3,3) * _n);
}
Matrix4 Matrix4::ProductMatrix4(const Matrix4& _other) const
{
	return Matrix4(
		(*this)(0,0) * _other(0,0) + (*this)(0,1) * _other(1,0) + (*this)(0,2) * _other(2,0) + (*this)(0,3) * _other(3,0),
		(*this)(0,0) * _other(0,1) + (*this)(0,1) * _other(1,1) + (*this)(0,2) * _other(2,1) + (*this)(0,3) * _other(3,1),
		(*this)(0,0) * _other(0,2) + (*this)(0,1) * _other(1,2) + (*this)(0,2) * _other(2,2) + (*this)(0,3) * _other(3,2),
		(*this)(0,0) * _other(0,3) + (*this)(0,1) * _other(1,3) + (*this)(0,2) * _other(2,3) + (*this)(0,3) * _other(3,3),
		(*this)(1,0) * _other(0,0) + (*this)(1,1) * _other(1,0) + (*this)(1,2) * _other(2,0) + (*this)(1,3) * _other(3,0),
		(*this)(1,0) * _other(0,1) + (*this)(1,1) * _other(1,1) + (*this)(1,2) * _other(2,1) + (*this)(1,3) * _other(3,1),
		(*this)(1,0) * _other(0,2) + (*this)(1,1) * _other(1,2) + (*this)(1,2) * _other(2,2) + (*this)(1,3) * _other(3,2),
		(*this)(1,0) * _other(0,3) + (*this)(1,1) * _other(1,3) + (*this)(1,2) * _other(2,3) + (*this)(1,3) * _other(3,3),
		(*this)(2,0) * _other(0,0) + (*this)(2,1) * _other(1,0) + (*this)(2,2) * _other(2,0) + (*this)(2,3) * _other(3,0),
		(*this)(2,0) * _other(0,1) + (*this)(2,1) * _other(1,1) + (*this)(2,2) * _other(2,1) + (*this)(2,3) * _other(3,1),
		(*this)(2,0) * _other(0,2) + (*this)(2,1) * _other(1,2) + (*this)(2,2) * _other(2,2) + (*this)(2,3) * _other(3,2),
		(*this)(2,0) * _other(0,3) + (*this)(2,1) * _other(1,3) + (*this)(2,2) * _other(2,3) + (*this)(2,3) * _other(3,3),
		(*this)(3,0) * _other(0,0) + (*this)(3,1) * _other(1,0) + (*this)(3,2) * _other(2,0) + (*this)(3,3) * _other(3,0),
		(*this)(3,0) * _other(0,1) + (*this)(3,1) * _other(1,1) + (*this)(3,2) * _other(2,1) + (*this)(3,3) * _other(3,1),
		(*this)(3,0) * _other(0,2) + (*this)(3,1) * _other(1,2) + (*this)(3,2) * _other(2,2) + (*this)(3,3) * _other(3,2),
		(*this)(3,0) * _other(0,3) + (*this)(3,1) * _other(1,3) + (*this)(3,2) * _other(2,3) + (*this)(3,3) * _other(3,3));
}
Matrix4 Matrix4::CreateTranslationMatrix(const Vector3& _translate)
{
	return Matrix4(
		1, 0, 0, _translate.X,
		0, 1, 0, _translate.Y,
		0, 0, 1, _translate.Z,
		0, 0, 0, 1
	);
}
Matrix4 Matrix4::CreateScaleMatrix(const Vector3& _scale)
{
	return Matrix4(
		_scale.X, 0, 0, 0,
		0, _scale.Y, 0, 0,
		0, 0, _scale.Z, 0,
		0, 0, 0, 1);
}
Matrix4 Matrix4::CreateXRotationMatrix(float _rad)
{
	return Matrix4(
		1, 0, 0, 0,
		0, cos(_rad), -sin(_rad), 0,
		0, sin(_rad), cos(_rad), 0,
		0, 0, 0, 1
	);
}
Matrix4 Matrix4::CreateYRotationMatrix(float _rad)
{
	return Matrix4(
		cos(_rad), 0, sin(_rad), 0,
		0, 1, 0, 0,
		-sin(_rad), 0, cos(_rad), 0,
		0, 0, 0, 1
	);
}
Matrix4 Matrix4::CreateZRotationMatrix(float _rad)
{
	return Matrix4(
		cos(_rad), -sin(_rad), 0, 0,
		sin(_rad), cos(_rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}
float Matrix4::Determinant() const
{
	float det = 0.0f;
	for (int _col = 0; _col < 4; ++_col) {
		float sign = (_col % 2 == 0) ? 1.0f : -1.0f;
		det += sign * (*this)(0,_col) * GetSubMatrix3x3Determinant(0, _col);
	}
	return det;
}
Matrix4 Matrix4::Inverse() const 
{
	const float det = Determinant();

	if (std::abs(det) < 1e-8f) {
		throw std::runtime_error("Matrix is not invertible (det == 0)");
	}

	Matrix4 inv;
	const float invDet = 1.0f / det;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			const float sign = ((i + j) % 2 == 0) ? 1.0f : -1.0f;

			float subDet = GetSubMatrix3x3Determinant(i, j);

			inv(i,j) = sign * subDet * invDet;
		}
	}

	return inv;
}
float Matrix4::GetSubMatrix3x3Determinant(int _excludeRow, int _excludeCol) const
{
	float sub[4][4];
	int _index = 0;
	for (int _col = 0; _col < 4; ++_col) {
		if (_col == _excludeCol) continue;
		for (int _row = 0; _row < 4; ++_row) {
			if (_row == _excludeRow) continue;
			sub[_index / 3][_index % 3] = (*this)(_col,_row);
			++_index;
		}
	}
	return sub[0][0] * (sub[1][1] * sub[2][2] - sub[1][2] * sub[2][1]) -
		sub[0][1] * (sub[1][0] * sub[2][2] - sub[1][2] * sub[2][0]) +
		sub[0][2] * (sub[1][0] * sub[2][1] - sub[1][1] * sub[2][0]);
}
Matrix4 Matrix4::CreateTransformMatrix(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale)
{
	return CreateTranslationMatrix(_position) *
		(CreateXRotationMatrix(_rotation.X) *
			CreateYRotationMatrix(_rotation.Y) *
			CreateZRotationMatrix(_rotation.Z) *
			CreateScaleMatrix(_scale));
}
Matrix4 Matrix4::GetPerspectiveMatrix(unsigned int _width, unsigned int _height, float _near, float _far, float _fov)
{
	float aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
	float fovRad = _fov * (PI / 180.0f);
	float tanHalfFov = tan(fovRad / 2.0f);
	Matrix4 perspective;
	perspective(0,0) = 1.0f / (aspectRatio * tanHalfFov);
	perspective(1,1) = 1.0f / tanHalfFov;
	perspective(2,2) = -(_far + _near) / (_far - _near);
	perspective(3,2) = -(2.0f * _far * _near) / (_far - _near);
	perspective(2,3) = -1.0f;
	return perspective;
}
std::array<float, 16> Matrix4::ToArray() const
{
	return {
		(*this)(0,0), (*this)(0,1), (*this)(0,2), (*this)(0,3),
		(*this)(1,0), (*this)(1,1), (*this)(1,2), (*this)(1,3),
		(*this)(2,0), (*this)(2,1), (*this)(2,2), (*this)(2,3),
		(*this)(3,0), (*this)(3,1), (*this)(3,2), (*this)(3,3)
	};
}
void Matrix4::Print() const
{
	std::cout
		<< (*this)(0,0) << " " << (*this)(0,1) << " " << (*this)(0,2) << " " << (*this)(0,3) << "\n"
		<< (*this)(1,0) << " " << (*this)(1,1) << " " << (*this)(1,2) << " " << (*this)(1,3) << "\n"
		<< (*this)(2,0) << " " << (*this)(2,1) << " " << (*this)(2,2) << " " << (*this)(2,3) << "\n"
		<< (*this)(3,0) << " " << (*this)(3,1) << " " << (*this)(3,2) << " " << (*this)(3,3) << "\n";
}
Matrix4 Matrix4::operator*(const Matrix4& _other) const
{
	return ProductMatrix4(_other);
}
Matrix4 Matrix4::operator/(const Matrix4& _other) const
{
	return ProductMatrix4(_other.Inverse());
}
Matrix4 Matrix4::operator+(const Matrix4& _other) const
{
	return AddMatrix4(_other);
}
Matrix4 Matrix4::operator-(const Matrix4& _other) const
{
	return Matrix4();
}
Matrix4& Matrix4::operator*=(const Matrix4& _other)
{
	Matrix4 temps;
	temps(0,0) = (*this)(0,0) * _other(0,0) + (*this)(0,1) * _other(1,0) + (*this)(0,2) * _other(2,0) + (*this)(0,3) * _other(3,0);
	temps(0,1) = (*this)(0,0) * _other(0,1) + (*this)(0,1) * _other(1,1) + (*this)(0,2) * _other(2,1) + (*this)(0,3) * _other(3,1);
	temps(0,2) = (*this)(0,0) * _other(0,2) + (*this)(0,1) * _other(1,2) + (*this)(0,2) * _other(2,2) + (*this)(0,3) * _other(3,2);
	temps(0,3) = (*this)(0,0) * _other(0,3) + (*this)(0,1) * _other(1,3) + (*this)(0,2) * _other(2,3) + (*this)(0,3) * _other(3,3);
	temps(1,0) = (*this)(1,0) * _other(0,0) + (*this)(1,1) * _other(1,0) + (*this)(1,2) * _other(2,0) + (*this)(1,3) * _other(3,0);
	temps(1,1) = (*this)(1,0) * _other(0,1) + (*this)(1,1) * _other(1,1) + (*this)(1,2) * _other(2,1) + (*this)(1,3) * _other(3,1);
	temps(1,2) = (*this)(1,0) * _other(0,2) + (*this)(1,1) * _other(1,2) + (*this)(1,2) * _other(2,2) + (*this)(1,3) * _other(3,2);
	temps(1,3) = (*this)(1,0) * _other(0,3) + (*this)(1,1) * _other(1,3) + (*this)(1,2) * _other(2,3) + (*this)(1,3) * _other(3,3);
	temps(2,0) = (*this)(2,0) * _other(0,0) + (*this)(2,1) * _other(1,0) + (*this)(2,2) * _other(2,0) + (*this)(2,3) * _other(3,0);
	temps(2,1) = (*this)(2,0) * _other(0,1) + (*this)(2,1) * _other(1,1) + (*this)(2,2) * _other(2,1) + (*this)(2,3) * _other(3,1);
	temps(2,2) = (*this)(2,0) * _other(0,2) + (*this)(2,1) * _other(1,2) + (*this)(2,2) * _other(2,2) + (*this)(2,3) * _other(3,2);
	temps(2,3) = (*this)(2,0) * _other(0,3) + (*this)(2,1) * _other(1,3) + (*this)(2,2) * _other(2,3) + (*this)(2,3) * _other(3,3);
	temps(3,0) = (*this)(3,0) * _other(0,0) + (*this)(3,1) * _other(1,0) + (*this)(3,2) * _other(2,0) + (*this)(3,3) * _other(3,0);
	temps(3,1) = (*this)(3,0) * _other(0,1) + (*this)(3,1) * _other(1,1) + (*this)(3,2) * _other(2,1) + (*this)(3,3) * _other(3,1);
	temps(3,2) = (*this)(3,0) * _other(0,2) + (*this)(3,1) * _other(1,2) + (*this)(3,2) * _other(2,2) + (*this)(3,3) * _other(3,2);
	temps(3,3) = (*this)(3,0) * _other(0,3) + (*this)(3,1) * _other(1,3) + (*this)(3,2) * _other(2,3) + (*this)(3,3) * _other(3,3);

	(*this)(0,0) = temps(0,0);
	(*this)(0,1) = temps(0,1);
	(*this)(0,2) = temps(0,2);
	(*this)(0,3) = temps(0,3);
	(*this)(1,0) = temps(1,0);
	(*this)(1,1) = temps(1,1);
	(*this)(1,2) = temps(1,2);
	(*this)(1,3) = temps(1,3);
	(*this)(2,0) = temps(2,0);
	(*this)(2,1) = temps(2,1);
	(*this)(2,2) = temps(2,2);
	(*this)(2,3) = temps(2,3);
	(*this)(3,0) = temps(3,0);
	(*this)(3,1) = temps(3,1);
	(*this)(3,2) = temps(3,2);
	(*this)(3,3) = temps(3,3);

	return *this;
}
Matrix4& Matrix4::operator/=(const Matrix4& _other)
{
	*this *= _other.Inverse();
	return *this;
}
Matrix4& Matrix4::operator+=(const Matrix4& _other)
{
	(*this)(0,0) += _other(0,0);
	(*this)(0,1) += _other(0,1);
	(*this)(0,2) += _other(0,2);
	(*this)(0,3) += _other(0,3);
	(*this)(1,0) += _other(1,0);
	(*this)(1,1) += _other(1,1);
	(*this)(1,2) += _other(1,2);
	(*this)(1,3) += _other(1,3);
	(*this)(2,0) += _other(2,0);
	(*this)(2,1) += _other(2,1);
	(*this)(2,2) += _other(2,2);
	(*this)(2,3) += _other(2,3);
	(*this)(3,0) += _other(3,0);
	(*this)(3,1) += _other(3,1);
	(*this)(3,2) += _other(3,2);
	(*this)(3,3) += _other(3,3);

	return *this;
}
Matrix4& Matrix4::operator-=(const Matrix4& _other)
{
	(*this)(0,0) -= _other(0,0);
	(*this)(0,1) -= _other(0,1);
	(*this)(0,2) -= _other(0,2);
	(*this)(0,3) -= _other(0,3);
	(*this)(1,0) -= _other(1,0);
	(*this)(1,1) -= _other(1,1);
	(*this)(1,2) -= _other(1,2);
	(*this)(1,3) -= _other(1,3);
	(*this)(2,0) -= _other(2,0);
	(*this)(2,1) -= _other(2,1);
	(*this)(2,2) -= _other(2,2);
	(*this)(2,3) -= _other(2,3);
	(*this)(3,0) -= _other(3,0);
	(*this)(3,1) -= _other(3,1);
	(*this)(3,2) -= _other(3,2);
	(*this)(3,3) -= _other(3,3);

	return *this;
}
Matrix4 Matrix4::operator*(float _n)
{
	return ProductScalar(_n);
}
Vector3 Matrix4::operator*(const Vector4& _multiply) const
{
	float x = (*this)(0,0)*_multiply.X + (*this)(1,0)*_multiply.Y + (*this)(2,0)*_multiply.Z + (*this)(3,0)*_multiply.W;
	float y = (*this)(0,1)*_multiply.X + (*this)(1,1)*_multiply.Y + (*this)(2,1)*_multiply.Z + (*this)(3,1)*_multiply.W;
	float z = (*this)(0,2)*_multiply.X + (*this)(1,2)*_multiply.Y + (*this)(2,2)*_multiply.Z + (*this)(3,2)*_multiply.W;
	float w = (*this)(0,3)*_multiply.X + (*this)(1,3)*_multiply.Y + (*this)(2,3)*_multiply.Z + (*this)(3,3)*_multiply.W;
	if (std::abs(w) > EPSILON)
	{
		x /= w;
		y /= w;
		z /= w;
	}
	return Vector3(x, y, z);
}
bool Matrix4::operator==(const Matrix4& _other) const
{
	for (int col = 0; col < 4; ++col)
	{
		for (int row = 0; row < 4; ++row)
		{
			if (std::abs((*this)(col,row) - _other(col,row)) >= EPSILON)
				return false;
		}
	}
	return true;
}
Vector4 Matrix4::operator[](int _index) const
{
	assert(_index >= 0 && _index < 3 && "Index out of range");
	return Vector4((*this)(_index, 0), (*this)(_index, 1), (*this)(_index, 2), (*this)(_index,3));
}
const float& Matrix4::operator()(size_t _row, size_t _col) const
{
	assert(_row < 4 && _col < 4);
	return M[_col + _row * 4];
}
float& Matrix4::operator()(size_t _row, size_t _col)
{
	assert(_row < 4 && _col < 4);
	return M[_col + _row * 4];
}