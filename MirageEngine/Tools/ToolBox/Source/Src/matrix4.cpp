#include "matrix4.h"
#include <iostream>
#include <array>
#include <assert.h>
#include <quaternion.h>

using namespace MirageEngine::Core::Math;

// ============================================================
// Constructors
// ============================================================

Matrix4::Matrix4(const Matrix4& _copy) noexcept
{
	(*this)(0, 0) = _copy(0, 0);
	(*this)(0, 1) = _copy(0, 1);
	(*this)(0, 2) = _copy(0, 2);
	(*this)(0, 3) = _copy(0, 3);
	(*this)(1, 0) = _copy(1, 0);
	(*this)(1, 1) = _copy(1, 1);
	(*this)(1, 2) = _copy(1, 2);
	(*this)(1, 3) = _copy(1, 3);
	(*this)(2, 0) = _copy(2, 0);
	(*this)(2, 1) = _copy(2, 1);
	(*this)(2, 2) = _copy(2, 2);
	(*this)(2, 3) = _copy(2, 3);
	(*this)(3, 0) = _copy(3, 0);
	(*this)(3, 1) = _copy(3, 1);
	(*this)(3, 2) = _copy(3, 2);
	(*this)(3, 3) = _copy(3, 3);
}

Matrix4::Matrix4(
	float _m00, float _m01, float _m02, float _m03, float _m10, float _m11, float _m12, float _m13, float _m20, float _m21, float _m22, float _m23, float _m30, float _m31, float _m32,
	float _m33) noexcept
{
	(*this)(0, 0) = _m00; (*this)(0, 1) = _m01; (*this)(0, 2) = _m02; (*this)(0, 3) = _m03;
	(*this)(1, 0) = _m10; (*this)(1, 1) = _m11; (*this)(1, 2) = _m12; (*this)(1, 3) = _m13;
	(*this)(2, 0) = _m20; (*this)(2, 1) = _m21; (*this)(2, 2) = _m22; (*this)(2, 3) = _m23;
	(*this)(3, 0) = _m30; (*this)(3, 1) = _m31; (*this)(3, 2) = _m32; (*this)(3, 3) = _m33;
}
Matrix4::Matrix4(float* _m) noexcept
{
	(*this)(0, 0) = _m[0];
	(*this)(0, 1) = _m[1];
	(*this)(0, 2) = _m[2];
	(*this)(0, 3) = _m[3];
	(*this)(1, 0) = _m[4];
	(*this)(1, 1) = _m[5];
	(*this)(1, 2) = _m[6];
	(*this)(1, 3) = _m[7];
	(*this)(2, 0) = _m[8];
	(*this)(2, 1) = _m[9];
	(*this)(2, 2) = _m[10];
	(*this)(2, 3) = _m[11];
	(*this)(3, 0) = _m[12];
	(*this)(3, 1) = _m[13];
	(*this)(3, 2) = _m[14];
	(*this)(3, 3) = _m[15];
}

// ============================================================
// Basic operations
// ============================================================

Matrix4 Matrix4::Identity() { return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); }

Vector4 Matrix4::Diagonal() { return Vector4((*this)(0, 0), (*this)(1, 1), (*this)(2, 2), (*this)(3, 3)); }

float Matrix4::Trace()
{
	Vector4 _vec = Diagonal();
	return _vec.X + _vec.Y + _vec.Z + _vec.W;
}

Matrix4 Matrix4::Opposed() const
{
	return Matrix4(
		-(*this)(0, 0), -(*this)(0, 1), -(*this)(0, 2), -(*this)(0, 3), -(*this)(1, 0), -(*this)(1, 1), -(*this)(1, 2), -(*this)(1, 3), -(*this)(2, 0), -(*this)(2, 1), -(*this)(2, 2), -(*this)(2, 3),
		-(*this)(3, 0), -(*this)(3, 1), -(*this)(3, 2), -(*this)(3, 3));
}

Matrix4 Matrix4::Transpose() const
{
	return Matrix4(
		(*this)(0, 0), (*this)(1, 0), (*this)(2, 0), (*this)(3, 0), (*this)(0, 1), (*this)(1, 1), (*this)(2, 1), (*this)(3, 1), (*this)(0, 2), (*this)(1, 2), (*this)(2, 2), (*this)(3, 2),
		(*this)(0, 3), (*this)(1, 3), (*this)(2, 3), (*this)(3, 3));
}

// ============================================================
// Arithmetic
// ============================================================

Matrix4 Matrix4::AddMatrix4(const Matrix4& _other) const
{
	return Matrix4(
		(*this)(0, 0) + _other(0, 0), (*this)(0, 1) + _other(0, 1), (*this)(0, 2) + _other(0, 2), (*this)(0, 3) + _other(0, 3), (*this)(1, 0) + _other(1, 0), (*this)(1, 1) + _other(1, 1),
		(*this)(1, 2) + _other(1, 2), (*this)(1, 3) + _other(1, 3), (*this)(2, 0) + _other(2, 0), (*this)(2, 1) + _other(2, 1), (*this)(2, 2) + _other(2, 2), (*this)(2, 3) + _other(2, 3),
		(*this)(3, 0) + _other(3, 0), (*this)(3, 1) + _other(3, 1), (*this)(3, 2) + _other(3, 2), (*this)(3, 3) + _other(3, 3));
}

Matrix4 Matrix4::ProductScalar(float _n) const
{
	return Matrix4(
		(*this)(0, 0) * _n, (*this)(0, 1) * _n, (*this)(0, 2) * _n, (*this)(0, 3) * _n, (*this)(1, 0) * _n, (*this)(1, 1) * _n, (*this)(1, 2) * _n, (*this)(1, 3) * _n, (*this)(2, 0) * _n,
		(*this)(2, 1) * _n, (*this)(2, 2) * _n, (*this)(2, 3) * _n, (*this)(3, 0) * _n, (*this)(3, 1) * _n, (*this)(3, 2) * _n, (*this)(3, 3) * _n);
}

Matrix4 Matrix4::ProductMatrix4(const Matrix4& _other) const
{
	return Matrix4(
		(*this)(0, 0) * _other(0, 0) + (*this)(0, 1) * _other(1, 0) + (*this)(0, 2) * _other(2, 0) + (*this)(0, 3) * _other(3, 0),
		(*this)(0, 0) * _other(0, 1) + (*this)(0, 1) * _other(1, 1) + (*this)(0, 2) * _other(2, 1) + (*this)(0, 3) * _other(3, 1),
		(*this)(0, 0) * _other(0, 2) + (*this)(0, 1) * _other(1, 2) + (*this)(0, 2) * _other(2, 2) + (*this)(0, 3) * _other(3, 2),
		(*this)(0, 0) * _other(0, 3) + (*this)(0, 1) * _other(1, 3) + (*this)(0, 2) * _other(2, 3) + (*this)(0, 3) * _other(3, 3),
		(*this)(1, 0) * _other(0, 0) + (*this)(1, 1) * _other(1, 0) + (*this)(1, 2) * _other(2, 0) + (*this)(1, 3) * _other(3, 0),
		(*this)(1, 0) * _other(0, 1) + (*this)(1, 1) * _other(1, 1) + (*this)(1, 2) * _other(2, 1) + (*this)(1, 3) * _other(3, 1),
		(*this)(1, 0) * _other(0, 2) + (*this)(1, 1) * _other(1, 2) + (*this)(1, 2) * _other(2, 2) + (*this)(1, 3) * _other(3, 2),
		(*this)(1, 0) * _other(0, 3) + (*this)(1, 1) * _other(1, 3) + (*this)(1, 2) * _other(2, 3) + (*this)(1, 3) * _other(3, 3),
		(*this)(2, 0) * _other(0, 0) + (*this)(2, 1) * _other(1, 0) + (*this)(2, 2) * _other(2, 0) + (*this)(2, 3) * _other(3, 0),
		(*this)(2, 0) * _other(0, 1) + (*this)(2, 1) * _other(1, 1) + (*this)(2, 2) * _other(2, 1) + (*this)(2, 3) * _other(3, 1),
		(*this)(2, 0) * _other(0, 2) + (*this)(2, 1) * _other(1, 2) + (*this)(2, 2) * _other(2, 2) + (*this)(2, 3) * _other(3, 2),
		(*this)(2, 0) * _other(0, 3) + (*this)(2, 1) * _other(1, 3) + (*this)(2, 2) * _other(2, 3) + (*this)(2, 3) * _other(3, 3),
		(*this)(3, 0) * _other(0, 0) + (*this)(3, 1) * _other(1, 0) + (*this)(3, 2) * _other(2, 0) + (*this)(3, 3) * _other(3, 0),
		(*this)(3, 0) * _other(0, 1) + (*this)(3, 1) * _other(1, 1) + (*this)(3, 2) * _other(2, 1) + (*this)(3, 3) * _other(3, 1),
		(*this)(3, 0) * _other(0, 2) + (*this)(3, 1) * _other(1, 2) + (*this)(3, 2) * _other(2, 2) + (*this)(3, 3) * _other(3, 2),
		(*this)(3, 0) * _other(0, 3) + (*this)(3, 1) * _other(1, 3) + (*this)(3, 2) * _other(2, 3) + (*this)(3, 3) * _other(3, 3));
}

// ============================================================
// Matrix properties
// ============================================================

float Matrix4::Determinant() const
{
	float det = 0.0f;
	for (int col = 0; col < 4; ++col)
	{
		float sign = (col % 2 == 0) ? 1.0f : -1.0f;
		det += sign * (*this)(0, col) * GetSubMatrix3x3Determinant(0, col);
	}
	return det;
}

Matrix4 Matrix4::Inverse() const
{
	const float det = Determinant();

	if (std::abs(det) < 1e-8f) throw std::runtime_error("Matrix is not invertible (det == 0)");

	Matrix4     inv;
	const float invDet = 1.0f / det;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			const float sign = ((i + j) % 2 == 0) ? 1.0f : -1.0f;
			float       subDet = GetSubMatrix3x3Determinant(i, j);
			inv(j, i) = sign * subDet * invDet;
		}
	}

	return inv;
}

Matrix3 Matrix4::GetSubMatrix(int _excludeRow, int _excludeCol) const
{
	Matrix3 sub;
	int     subCol = 0;
	for (int col = 0; col < 4; ++col)
	{
		if (col == _excludeCol) continue;
		int subRow = 0;
		for (int row = 0; row < 4; ++row)
		{
			if (row == _excludeRow) continue;
			sub(subRow, subCol) = (*this)(col, row);
			++subRow;
		}
		++subCol;
	}
	return sub;
}

float Matrix4::GetSubMatrix3x3Determinant(int _excludeRow, int _excludeCol) const
{
	float sub[3][3];
	int   index = 0;
	for (int col = 0; col < 4; ++col)
	{
		if (col == _excludeCol) continue;
		for (int row = 0; row < 4; ++row)
		{
			if (row == _excludeRow) continue;
			sub[index / 3][index % 3] = (*this)(row, col);
			++index;
		}
	}
	return sub[0][0] * (sub[1][1] * sub[2][2] - sub[1][2] * sub[2][1]) - sub[0][1] * (sub[1][0] * sub[2][2] - sub[1][2] * sub[2][0]) + sub[0][2] * (sub[1][0] * sub[2][1] - sub[1][1] * sub[2][0]);
}

// ============================================================
// Transform / Projection
// ============================================================

Matrix4 Matrix4::CreateTranslationMatrix(const Vector3& _translate) { return Matrix4(1, 0, 0, _translate.X, 0, 1, 0, _translate.Y, 0, 0, 1, _translate.Z, 0, 0, 0, 1); }

Matrix4 Matrix4::CreateScaleMatrix(const Vector3& _scale) { return Matrix4(_scale.X, 0, 0, 0, 0, _scale.Y, 0, 0, 0, 0, _scale.Z, 0, 0, 0, 0, 1); }

Matrix4 Matrix4::CreateXRotationMatrix(float _rad) { return Matrix4(1, 0, 0, 0, 0, cosf(_rad), -sinf(_rad), 0, 0, sinf(_rad), cosf(_rad), 0, 0, 0, 0, 1); }

Matrix4 Matrix4::CreateYRotationMatrix(float _rad) { return Matrix4(cosf(_rad), 0, sinf(_rad), 0, 0, 1, 0, 0, -sinf(_rad), 0, cosf(_rad), 0, 0, 0, 0, 1); }

Matrix4 Matrix4::CreateZRotationMatrix(float _rad) { return Matrix4(cosf(_rad), -sinf(_rad), 0, 0, sinf(_rad), cosf(_rad), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); }

Matrix4 Matrix4::CreateTransformMatrix(const Vector3& _position, const Vector3& _rotation, const Vector3& _scale)
{
	return CreateTranslationMatrix(_position) * (CreateXRotationMatrix(_rotation.X) * CreateYRotationMatrix(_rotation.Y) * CreateZRotationMatrix(_rotation.Z)) * CreateScaleMatrix(_scale);
}

Matrix4 Matrix4::CreateTransformMatrix(const Vector3& _position, const Quaternion& _rotation, const Vector3& _scale)
{
	Matrix4 rotMat = _rotation.ToMatrix();
	return Matrix4(
		rotMat(0, 0) * _scale.X, rotMat(0, 1) * _scale.Y, rotMat(0, 2) * _scale.Z, _position.X, rotMat(1, 0) * _scale.X, rotMat(1, 1) * _scale.Y, rotMat(1, 2) * _scale.Z, _position.Y,
		rotMat(2, 0) * _scale.X, rotMat(2, 1) * _scale.Y, rotMat(2, 2) * _scale.Z, _position.Z, 0.f, 0.f, 0.f, 1.f);
}

Matrix4 Matrix4::GetPerspectiveMatrix(unsigned int _width, unsigned int _height, float _near, float _far, float _fov)
{
	float aspectRatio = static_cast<float>(_width) / static_cast<float>(_height);
	float tanHalfFov = tanf((_fov * 180 / PI) / 2.0f);

	Matrix4 perspective;
	perspective(0, 0) = 1.0f / (aspectRatio * tanHalfFov);
	perspective(1, 1) = 1.0f / tanHalfFov;
	perspective(2, 2) = -(_far + _near) / (_far - _near);
	perspective(2, 3) = -(2.0f * _far * _near) / (_far - _near);
	perspective(3, 2) = -1.0f;
	return perspective;
}

Matrix4 Matrix4::LookAt(Vector3 eye, Vector3 center, Vector3 up)
{
	Vector3 f = (center - eye).Normalized();
	Vector3 s = f.CrossProductVector3(up).Normalized();
	Vector3 u = s.CrossProductVector3(f);

	Matrix4 result = Matrix4::Identity();
	result(0, 0) = s.X;
	result(0, 1) = s.Y;
	result(0, 2) = s.Z;
	result(1, 0) = u.X;
	result(1, 1) = u.Y;
	result(1, 2) = u.Z;
	result(2, 0) = -f.X;
	result(2, 1) = -f.Y;
	result(2, 2) = -f.Z;
	result(0, 3) = -(s.DotProductVector3(eye));
	result(1, 3) = -(u.DotProductVector3(eye));
	result(2, 3) = f.DotProductVector3(eye);
	return result;
}
Vector3 Matrix4::GetTranslation() const
{
	return Vector3((*this)(0, 3), (*this)(1, 3), (*this)(2, 3));
}
Vector3 Matrix4::GetScale() const
{
	float sx = Vector3((*this)(0, 0), (*this)(1, 0), (*this)(2, 0)).Norm();
	float sy = Vector3((*this)(0, 1), (*this)(1, 1), (*this)(2, 1)).Norm();
	float sz = Vector3((*this)(0, 2), (*this)(1, 2), (*this)(2, 2)).Norm();

	return Vector3(sx, sy, sz);
}
Quaternion Matrix4::GetRotation() const {
	Vector3 scale = GetScale();

	float m00 = (*this)(0, 0) / scale.X;
	float m11 = (*this)(1, 1) / scale.Y;
	float m22 = (*this)(2, 2) / scale.Z;

	float trace = m00 + m11 + m22;
	float x, y, z, w;

	if (trace > 0.0f) {
		float s = 0.5f / sqrtf(trace + 1.0f);
		w = 0.25f / s;
		x = ((*this)(2, 1) / scale.Y - (*this)(1, 2) / scale.Z) * s;
		y = ((*this)(0, 2) / scale.Z - (*this)(2, 0) / scale.X) * s;
		z = ((*this)(1, 0) / scale.X - (*this)(0, 1) / scale.Y) * s;
	}
	else {
		if (m00 > m11 && m00 > m22) {
			float s = 2.0f * sqrtf(1.0f + m00 - m11 - m22);
			w = ((*this)(2, 1) / scale.Y - (*this)(1, 2) / scale.Z) / s;
			x = 0.25f * s;
			y = ((*this)(0, 1) / scale.Y + (*this)(1, 0) / scale.X) / s;
			z = ((*this)(0, 2) / scale.Z + (*this)(2, 0) / scale.X) / s;
		}
		else if (m11 > m22) {
			float s = 2.0f * sqrtf(1.0f + m11 - m00 - m22);
			w = ((*this)(0, 2) / scale.Z - (*this)(2, 0) / scale.X) / s;
			x = ((*this)(0, 1) / scale.Y + (*this)(1, 0) / scale.X) / s;
			y = 0.25f * s;
			z = ((*this)(1, 2) / scale.Z + (*this)(2, 1) / scale.Y) / s;
		}
		else {
			float s = 2.0f * sqrtf(1.0f + m22 - m00 - m11);
			w = ((*this)(1, 0) / scale.X - (*this)(0, 1) / scale.Y) / s;
			x = ((*this)(0, 2) / scale.Z + (*this)(2, 0) / scale.X) / s;
			y = ((*this)(1, 2) / scale.Z + (*this)(2, 1) / scale.Y) / s;
			z = 0.25f * s;
		}
	}
	return Quaternion(w, x, y, z);
}

// ============================================================
// Display / Utility
// ============================================================

std::array<float, 16> Matrix4::ToArray() const
{
	return { M[0],  M[1],  M[2],  M[3],
			 M[4],  M[5],  M[6],  M[7],
			 M[8],  M[9],  M[10], M[11],
			 M[12], M[13], M[14], M[15] };
}

void Matrix4::Print() const
{
	std::cout << (*this)(0, 0) << " " << (*this)(0, 1) << " " << (*this)(0, 2) << " " << (*this)(0, 3) << "\n"
		<< (*this)(1, 0) << " " << (*this)(1, 1) << " " << (*this)(1, 2) << " " << (*this)(1, 3) << "\n"
		<< (*this)(2, 0) << " " << (*this)(2, 1) << " " << (*this)(2, 2) << " " << (*this)(2, 3) << "\n"
		<< (*this)(3, 0) << " " << (*this)(3, 1) << " " << (*this)(3, 2) << " " << (*this)(3, 3) << "\n";
}

// ============================================================
// Operators
// ============================================================

Matrix4 Matrix4::operator*(const Matrix4& _other) const { return ProductMatrix4(_other); }

Matrix4 Matrix4::operator/(const Matrix4& _other) const { return ProductMatrix4(_other.Inverse()); }

Matrix4 Matrix4::operator+(const Matrix4& _other) const { return AddMatrix4(_other); }

// FIX: was returning Matrix4() (zero matrix), now correctly subtracts
Matrix4 Matrix4::operator-(const Matrix4& _other) const { return AddMatrix4(_other.Opposed()); }

Matrix4& Matrix4::operator*=(const Matrix4& _other)
{
	*this = ProductMatrix4(_other);
	return *this;
}

Matrix4& Matrix4::operator/=(const Matrix4& _other)
{
	*this *= _other.Inverse();
	return *this;
}

Matrix4& Matrix4::operator+=(const Matrix4& _other)
{
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c) (*this)(r, c) += _other(r, c);
	return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& _other)
{
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c) (*this)(r, c) -= _other(r, c);
	return *this;
}

Matrix4 Matrix4::operator*(float _n) const { return ProductScalar(_n); }

Matrix4& Matrix4::operator*=(float _n)
{
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c) (*this)(r, c) *= _n;
	return *this;
}

Vector3 Matrix4::operator*(const Vector4& _multiply) const
{
	float x = (*this)(0, 0) * _multiply.X + (*this)(0, 1) * _multiply.Y + (*this)(0, 2) * _multiply.Z + (*this)(0, 3) * _multiply.W;
	float y = (*this)(1, 0) * _multiply.X + (*this)(1, 1) * _multiply.Y + (*this)(1, 2) * _multiply.Z + (*this)(1, 3) * _multiply.W;
	float z = (*this)(2, 0) * _multiply.X + (*this)(2, 1) * _multiply.Y + (*this)(2, 2) * _multiply.Z + (*this)(2, 3) * _multiply.W;
	float w = (*this)(3, 0) * _multiply.X + (*this)(3, 1) * _multiply.Y + (*this)(3, 2) * _multiply.Z + (*this)(3, 3) * _multiply.W;
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
	for (int r = 0; r < 4; ++r)
		for (int c = 0; c < 4; ++c)
			if (std::abs((*this)(r, c) - _other(r, c)) >= EPSILON) return false;
	return true;
}

float& Matrix4::operator()(size_t _row, size_t _col)
{
	assert(_row < 4 && _col < 4);
	return M[_row + _col * 4];
}

const float& Matrix4::operator()(size_t _row, size_t _col) const
{
	assert(_row < 4 && _col < 4);
	return M[_row + _col * 4];
}
