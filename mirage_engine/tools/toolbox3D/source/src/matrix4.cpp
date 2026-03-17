#include "matrix4.h"
#include <iostream>
#include <array>
#include <assert.h>

using namespace MirageEngine::Core::Math;
Matrix4::Matrix4(const Matrix4& copy)
{
	(*this)(0,0) = copy(0,0); (*this)(0,1) = copy(0,1); (*this)(0,2) = copy(0,2); (*this)(0,3) = copy(0,3);
	(*this)(1,0) = copy(1,0); (*this)(1,1) = copy(1,1); (*this)(1,2) = copy(1,2); (*this)(1,3) = copy(1,3);
	(*this)(2,0) = copy(2,0); (*this)(2,1) = copy(2,1); (*this)(2,2) = copy(2,2); (*this)(2,3) = copy(2,3);
	(*this)(3,0) = copy(3,0); (*this)(3,1) = copy(3,1); (*this)(3,2) = copy(3,2); (*this)(3,3) = copy(3,3);
}
Matrix4::Matrix4(
	float m00 , float m01 , float m02 , float m03,
	float m10 , float m11 , float m12 , float m13,
	float m20 , float m21 , float m22 , float m23,
	float m30 , float m31 , float m32 , float m33)
{
	(*this)(0,0) = m00; (*this)(0,1) = m01; (*this)(0,2) = m02; (*this)(0,3) = m03;
	(*this)(1,0) = m10; (*this)(1,1) = m11; (*this)(1,2) = m12; (*this)(1,3) = m13;
	(*this)(2,0) = m20; (*this)(2,1) = m21; (*this)(2,2) = m22; (*this)(2,3) = m23;
	(*this)(3,0) = m30; (*this)(3,1) = m31; (*this)(3,2) = m32; (*this)(3,3) = m33;
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
	Vector4 vec = Diagonal();
	return vec.x + vec.y + vec.z + vec.w;
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
Matrix4 Matrix4::AddMatrix4(const Matrix4& other) const
{
	return	Matrix4(
		(*this)(0,0) + other(0,0), (*this)(0,1) + other(0,1), (*this)(0,2) + other(0,2), (*this)(0,3) + other(0,3),
		(*this)(1,0) + other(1,0), (*this)(1,1) + other(1,1), (*this)(1,2) + other(1,2), (*this)(1,3) + other(1,3),
		(*this)(2,0) + other(2,0), (*this)(2,1) + other(2,1), (*this)(2,2) + other(2,2), (*this)(2,3) + other(2,3),
		(*this)(3,0) + other(3,0), (*this)(3,1) + other(3,1), (*this)(3,2) + other(3,2), (*this)(3,3) + other(3,3));
}
Matrix4 Matrix4::ProductScalar(float n)
{
	return	Matrix4(
		(*this)(0,0) * n, (*this)(0,1) * n, (*this)(0,2) * n, (*this)(0,3) * n,
		(*this)(1,0) * n, (*this)(1,1) * n, (*this)(1,2) * n, (*this)(1,3) * n,
		(*this)(2,0) * n, (*this)(2,1) * n, (*this)(2,2) * n, (*this)(2,3) * n,
		(*this)(3,0) * n, (*this)(3,1) * n, (*this)(3,2) * n, (*this)(3,3) * n);
}
Matrix4 Matrix4::ProductMatrix4(const Matrix4& other) const
{
	return Matrix4(
		(*this)(0,0) * other(0,0) + (*this)(0,1) * other(1,0) + (*this)(0,2) * other(2,0) + (*this)(0,3) * other(3,0),
		(*this)(0,0) * other(0,1) + (*this)(0,1) * other(1,1) + (*this)(0,2) * other(2,1) + (*this)(0,3) * other(3,1),
		(*this)(0,0) * other(0,2) + (*this)(0,1) * other(1,2) + (*this)(0,2) * other(2,2) + (*this)(0,3) * other(3,2),
		(*this)(0,0) * other(0,3) + (*this)(0,1) * other(1,3) + (*this)(0,2) * other(2,3) + (*this)(0,3) * other(3,3),
		(*this)(1,0) * other(0,0) + (*this)(1,1) * other(1,0) + (*this)(1,2) * other(2,0) + (*this)(1,3) * other(3,0),
		(*this)(1,0) * other(0,1) + (*this)(1,1) * other(1,1) + (*this)(1,2) * other(2,1) + (*this)(1,3) * other(3,1),
		(*this)(1,0) * other(0,2) + (*this)(1,1) * other(1,2) + (*this)(1,2) * other(2,2) + (*this)(1,3) * other(3,2),
		(*this)(1,0) * other(0,3) + (*this)(1,1) * other(1,3) + (*this)(1,2) * other(2,3) + (*this)(1,3) * other(3,3),
		(*this)(2,0) * other(0,0) + (*this)(2,1) * other(1,0) + (*this)(2,2) * other(2,0) + (*this)(2,3) * other(3,0),
		(*this)(2,0) * other(0,1) + (*this)(2,1) * other(1,1) + (*this)(2,2) * other(2,1) + (*this)(2,3) * other(3,1),
		(*this)(2,0) * other(0,2) + (*this)(2,1) * other(1,2) + (*this)(2,2) * other(2,2) + (*this)(2,3) * other(3,2),
		(*this)(2,0) * other(0,3) + (*this)(2,1) * other(1,3) + (*this)(2,2) * other(2,3) + (*this)(2,3) * other(3,3),
		(*this)(3,0) * other(0,0) + (*this)(3,1) * other(1,0) + (*this)(3,2) * other(2,0) + (*this)(3,3) * other(3,0),
		(*this)(3,0) * other(0,1) + (*this)(3,1) * other(1,1) + (*this)(3,2) * other(2,1) + (*this)(3,3) * other(3,1),
		(*this)(3,0) * other(0,2) + (*this)(3,1) * other(1,2) + (*this)(3,2) * other(2,2) + (*this)(3,3) * other(3,2),
		(*this)(3,0) * other(0,3) + (*this)(3,1) * other(1,3) + (*this)(3,2) * other(2,3) + (*this)(3,3) * other(3,3));
}
Matrix4 Matrix4::CreateTranslationMatrix(const Vector3& translation)
{
	return Matrix4(
		1, 0, 0, translation.x,
		0, 1, 0, translation.y,
		0, 0, 1, translation.z,
		0, 0, 0, 1
	);
}
Matrix4 Matrix4::CreateScaleMatrix(const Vector3& scale)
{
	return Matrix4(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1);
}
Matrix4 Matrix4::CreateXRotationMatrix(float rad)
{
	return Matrix4(
		1, 0, 0, 0,
		0, cos(rad), -sin(rad), 0,
		0, sin(rad), cos(rad), 0,
		0, 0, 0, 1
	);
}
Matrix4 Matrix4::CreateYRotationMatrix(float rad)
{
	return Matrix4(
		cos(rad), 0, sin(rad), 0,
		0, 1, 0, 0,
		-sin(rad), 0, cos(rad), 0,
		0, 0, 0, 1
	);
}
Matrix4 Matrix4::CreateZRotationMatrix(float rad)
{
	return Matrix4(
		cos(rad), -sin(rad), 0, 0,
		sin(rad), cos(rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}
float Matrix4::Determinant() const
{
	float det = 0.0f;
	for (int col = 0; col < 4; ++col) {
		float sign = (col % 2 == 0) ? 1.0f : -1.0f;
		det += sign * (*this)(0,col) * GetSubMatrix3x3Determinant(0, col);
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
float Matrix4::GetSubMatrix3x3Determinant(int excludeRow, int excludeCol) const
{
	float sub[4][4];
	int index = 0;
	for (int col = 0; col < 4; ++col) {
		if (col == excludeCol) continue;
		for (int row = 0; row < 4; ++row) {
			if (row == excludeRow) continue;
			sub[index / 3][index % 3] = (*this)(col,row);
			++index;
		}
	}
	return sub[0][0] * (sub[1][1] * sub[2][2] - sub[1][2] * sub[2][1]) -
		sub[0][1] * (sub[1][0] * sub[2][2] - sub[1][2] * sub[2][0]) +
		sub[0][2] * (sub[1][0] * sub[2][1] - sub[1][1] * sub[2][0]);
}
Matrix4 Matrix4::CreateTransformMatrix(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	return CreateTranslationMatrix(position) *
		(CreateXRotationMatrix(rotation.x) *
			CreateYRotationMatrix(rotation.y) *
			CreateZRotationMatrix(rotation.z) *
			CreateScaleMatrix(scale));
}
Matrix4 Matrix4::GetPerspectiveMatrix(unsigned int width, unsigned int height, float near, float far, float fov)
{
	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	float fovRad = fov * (PI / 180.0f);
	float tanHalfFov = tan(fovRad / 2.0f);
	Matrix4 perspective;
	perspective(0,0) = 1.0f / (aspectRatio * tanHalfFov);
	perspective(1,1) = 1.0f / tanHalfFov;
	perspective(2,2) = -(far + near) / (far - near);
	perspective(3,2) = -(2.0f * far * near) / (far - near);
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
Matrix4 Matrix4::operator*(const Matrix4& other) const
{
	return ProductMatrix4(other);
}
Matrix4 Matrix4::operator/(const Matrix4& other) const
{
	return ProductMatrix4(other.Inverse());
}
Matrix4 Matrix4::operator+(const Matrix4& other) const
{
	return AddMatrix4(other);
}
Matrix4 Matrix4::operator-(const Matrix4& other) const
{
	return Matrix4();
}
Matrix4& Matrix4::operator*=(const Matrix4& other)
{
	Matrix4 temps;
	temps(0,0) = (*this)(0,0) * other(0,0) + (*this)(0,1) * other(1,0) + (*this)(0,2) * other(2,0) + (*this)(0,3) * other(3,0);
	temps(0,1) = (*this)(0,0) * other(0,1) + (*this)(0,1) * other(1,1) + (*this)(0,2) * other(2,1) + (*this)(0,3) * other(3,1);
	temps(0,2) = (*this)(0,0) * other(0,2) + (*this)(0,1) * other(1,2) + (*this)(0,2) * other(2,2) + (*this)(0,3) * other(3,2);
	temps(0,3) = (*this)(0,0) * other(0,3) + (*this)(0,1) * other(1,3) + (*this)(0,2) * other(2,3) + (*this)(0,3) * other(3,3);
	temps(1,0) = (*this)(1,0) * other(0,0) + (*this)(1,1) * other(1,0) + (*this)(1,2) * other(2,0) + (*this)(1,3) * other(3,0);
	temps(1,1) = (*this)(1,0) * other(0,1) + (*this)(1,1) * other(1,1) + (*this)(1,2) * other(2,1) + (*this)(1,3) * other(3,1);
	temps(1,2) = (*this)(1,0) * other(0,2) + (*this)(1,1) * other(1,2) + (*this)(1,2) * other(2,2) + (*this)(1,3) * other(3,2);
	temps(1,3) = (*this)(1,0) * other(0,3) + (*this)(1,1) * other(1,3) + (*this)(1,2) * other(2,3) + (*this)(1,3) * other(3,3);
	temps(2,0) = (*this)(2,0) * other(0,0) + (*this)(2,1) * other(1,0) + (*this)(2,2) * other(2,0) + (*this)(2,3) * other(3,0);
	temps(2,1) = (*this)(2,0) * other(0,1) + (*this)(2,1) * other(1,1) + (*this)(2,2) * other(2,1) + (*this)(2,3) * other(3,1);
	temps(2,2) = (*this)(2,0) * other(0,2) + (*this)(2,1) * other(1,2) + (*this)(2,2) * other(2,2) + (*this)(2,3) * other(3,2);
	temps(2,3) = (*this)(2,0) * other(0,3) + (*this)(2,1) * other(1,3) + (*this)(2,2) * other(2,3) + (*this)(2,3) * other(3,3);
	temps(3,0) = (*this)(3,0) * other(0,0) + (*this)(3,1) * other(1,0) + (*this)(3,2) * other(2,0) + (*this)(3,3) * other(3,0);
	temps(3,1) = (*this)(3,0) * other(0,1) + (*this)(3,1) * other(1,1) + (*this)(3,2) * other(2,1) + (*this)(3,3) * other(3,1);
	temps(3,2) = (*this)(3,0) * other(0,2) + (*this)(3,1) * other(1,2) + (*this)(3,2) * other(2,2) + (*this)(3,3) * other(3,2);
	temps(3,3) = (*this)(3,0) * other(0,3) + (*this)(3,1) * other(1,3) + (*this)(3,2) * other(2,3) + (*this)(3,3) * other(3,3);

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
Matrix4& Matrix4::operator/=(const Matrix4& other)
{
	*this *= other.Inverse();
	return *this;
}
Matrix4& Matrix4::operator+=(const Matrix4& other)
{
	(*this)(0,0) += other(0,0);
	(*this)(0,1) += other(0,1);
	(*this)(0,2) += other(0,2);
	(*this)(0,3) += other(0,3);
	(*this)(1,0) += other(1,0);
	(*this)(1,1) += other(1,1);
	(*this)(1,2) += other(1,2);
	(*this)(1,3) += other(1,3);
	(*this)(2,0) += other(2,0);
	(*this)(2,1) += other(2,1);
	(*this)(2,2) += other(2,2);
	(*this)(2,3) += other(2,3);
	(*this)(3,0) += other(3,0);
	(*this)(3,1) += other(3,1);
	(*this)(3,2) += other(3,2);
	(*this)(3,3) += other(3,3);

	return *this;
}
Matrix4& Matrix4::operator-=(const Matrix4& other)
{
	(*this)(0,0) -= other(0,0);
	(*this)(0,1) -= other(0,1);
	(*this)(0,2) -= other(0,2);
	(*this)(0,3) -= other(0,3);
	(*this)(1,0) -= other(1,0);
	(*this)(1,1) -= other(1,1);
	(*this)(1,2) -= other(1,2);
	(*this)(1,3) -= other(1,3);
	(*this)(2,0) -= other(2,0);
	(*this)(2,1) -= other(2,1);
	(*this)(2,2) -= other(2,2);
	(*this)(2,3) -= other(2,3);
	(*this)(3,0) -= other(3,0);
	(*this)(3,1) -= other(3,1);
	(*this)(3,2) -= other(3,2);
	(*this)(3,3) -= other(3,3);

	return *this;
}
Matrix4 Matrix4::operator*(float n)
{
	return ProductScalar(n);
}
Vector3 Matrix4::operator*(const Vector4& mult) const
{
	float x = (*this)(0,0)*mult.x + (*this)(1,0)*mult.y + (*this)(2,0)*mult.z + (*this)(3,0)*mult.w;
	float y = (*this)(0,1)*mult.x + (*this)(1,1)*mult.y + (*this)(2,1)*mult.z + (*this)(3,1)*mult.w;
	float z = (*this)(0,2)*mult.x + (*this)(1,2)*mult.y + (*this)(2,2)*mult.z + (*this)(3,2)*mult.w;
	float w = (*this)(0,3)*mult.x + (*this)(1,3)*mult.y + (*this)(2,3)*mult.z + (*this)(3,3)*mult.w;
	if (std::abs(w) > EPSILON)
	{
		x /= w;
		y /= w;
		z /= w;
	}
	return Vector3(x, y, z);
}
bool Matrix4::operator==(const Matrix4& other) const
{
	for (int col = 0; col < 4; ++col)
	{
		for (int row = 0; row < 4; ++row)
		{
			if (std::abs((*this)(col,row) - other(col,row)) >= EPSILON)
				return false;
		}
	}
	return true;
}
Vector4 Matrix4::operator[](int index) const
{
	assert(index >= 0 && index < 3 && "Index out of range");
	return Vector4((*this)(index, 0), (*this)(index, 1), (*this)(index, 2), (*this)(index,3));
}
const float& Matrix4::operator()(size_t row, size_t col) const
{
	assert(row < 4 && col < 4);
	return m[col + row * 4];
}
float& Matrix4::operator()(size_t row, size_t col)
{
	assert(row < 4 && col < 4);
	return m[col + row * 4];
}