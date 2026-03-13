#include "matrix4.h"

using namespace Bwt::Core::Maths;
inline Matrix4::Matrix4(const Matrix4& copy)
{
	m[0][0] = copy[0][0]; m[0][1] = copy[0][1]; m[0][2] = copy[0][2]; m[0][3] = copy[0][3];
	m[1][0] = copy[1][0]; m[1][1] = copy[1][1]; m[1][2] = copy[1][2]; m[1][3] = copy[1][3];
	m[2][0] = copy[2][0]; m[2][1] = copy[2][1]; m[2][2] = copy[2][2]; m[2][3] = copy[2][3];
	m[3][0] = copy[3][0]; m[3][1] = copy[3][1]; m[3][2] = copy[3][2]; m[3][3] = copy[3][3];
}
inline Matrix4 Matrix4::Identity()
{
	return Matrix4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}
inline Vector4 Matrix4::Diagonal()
{
	return Vector4(m[0][0], m[1][1], m[2][2], m[3][3]);
}
inline float Matrix4::Trace()
{
	Vector4 vec = Diagonal();
	return vec.x + vec.y + vec.z + vec.w;
}
inline Matrix4 Matrix4::Opposed() const
{
	return	Matrix4(
		-m[0][0], -m[0][1], -m[0][2], -m[0][3],
		-m[1][0], -m[1][1], -m[1][2], -m[1][3],
		-m[2][0], -m[2][1], -m[2][2], -m[2][3],
		-m[3][0], -m[3][1], -m[3][2], -m[3][3]);
}
inline Matrix4 Matrix4::Transpose() const
{
	return Matrix4(
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]);
}
inline Matrix4 Matrix4::AddMatrix4(const Matrix4& other) const
{
	return	Matrix4(
		m[0][0] + other[0][0], m[0][1] + other[0][1], m[0][2] + other[0][2], m[0][3] + other[0][3],
		m[1][0] + other[1][0], m[1][1] + other[1][1], m[1][2] + other[1][2], m[1][3] + other[1][3],
		m[2][0] + other[2][0], m[2][1] + other[2][1], m[2][2] + other[2][2], m[2][3] + other[2][3],
		m[3][0] + other[3][0], m[3][1] + other[3][1], m[3][2] + other[3][2], m[3][3] + other[3][3]);
}
inline Matrix4 Matrix4::ProductScalar(float n)
{
	return	Matrix4(
		m[0][0] * n, m[0][1] * n, m[0][2] * n, m[0][3] * n,
		m[1][0] * n, m[1][1] * n, m[1][2] * n, m[1][3] * n,
		m[2][0] * n, m[2][1] * n, m[2][2] * n, m[2][3] * n,
		m[3][0] * n, m[3][1] * n, m[3][2] * n, m[3][3] * n);
}
inline Matrix4 Matrix4::ProductMatrix4(const Matrix4& other) const
{
	return Matrix4(
		m[0][0] * other[0][0] + m[0][1] * other[1][0] + m[0][2] * other[2][0] + m[0][3] * other[3][0],
		m[0][0] * other[0][1] + m[0][1] * other[1][1] + m[0][2] * other[2][1] + m[0][3] * other[3][1],
		m[0][0] * other[0][2] + m[0][1] * other[1][2] + m[0][2] * other[2][2] + m[0][3] * other[3][2],
		m[0][0] * other[0][3] + m[0][1] * other[1][3] + m[0][2] * other[2][3] + m[0][3] * other[3][3],
		m[1][0] * other[0][0] + m[1][1] * other[1][0] + m[1][2] * other[2][0] + m[1][3] * other[3][0],
		m[1][0] * other[0][1] + m[1][1] * other[1][1] + m[1][2] * other[2][1] + m[1][3] * other[3][1],
		m[1][0] * other[0][2] + m[1][1] * other[1][2] + m[1][2] * other[2][2] + m[1][3] * other[3][2],
		m[1][0] * other[0][3] + m[1][1] * other[1][3] + m[1][2] * other[2][3] + m[1][3] * other[3][3],
		m[2][0] * other[0][0] + m[2][1] * other[1][0] + m[2][2] * other[2][0] + m[2][3] * other[3][0],
		m[2][0] * other[0][1] + m[2][1] * other[1][1] + m[2][2] * other[2][1] + m[2][3] * other[3][1],
		m[2][0] * other[0][2] + m[2][1] * other[1][2] + m[2][2] * other[2][2] + m[2][3] * other[3][2],
		m[2][0] * other[0][3] + m[2][1] * other[1][3] + m[2][2] * other[2][3] + m[2][3] * other[3][3],
		m[3][0] * other[0][0] + m[3][1] * other[1][0] + m[3][2] * other[2][0] + m[3][3] * other[3][0],
		m[3][0] * other[0][1] + m[3][1] * other[1][1] + m[3][2] * other[2][1] + m[3][3] * other[3][1],
		m[3][0] * other[0][2] + m[3][1] * other[1][2] + m[3][2] * other[2][2] + m[3][3] * other[3][2],
		m[3][0] * other[0][3] + m[3][1] * other[1][3] + m[3][2] * other[2][3] + m[3][3] * other[3][3]);
}
inline Matrix4 Matrix4::CreateTranslationMatrix(const Vector3& translation)
{
	return Matrix4(
		1, 0, 0, translation.x,
		0, 1, 0, translation.y,
		0, 0, 1, translation.z,
		0, 0, 0, 1
	);
}
inline Matrix4 Matrix4::CreateScaleMatrix(const Vector3& scale)
{
	return Matrix4(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1);
}
inline Matrix4 Matrix4::CreateXRotationMatrix(float rad)
{
	return Matrix4(
		1, 0, 0, 0,
		0, cos(rad), -sin(rad), 0,
		0, sin(rad), cos(rad), 0,
		0, 0, 0, 1
	);
}
inline Matrix4 Matrix4::CreateYRotationMatrix(float rad)
{
	return Matrix4(
		cos(rad), 0, sin(rad), 0,
		0, 1, 0, 0,
		-sin(rad), 0, cos(rad), 0,
		0, 0, 0, 1
	);
}
inline Matrix4 Matrix4::CreateZRotationMatrix(float rad)
{
	return Matrix4(
		cos(rad), -sin(rad), 0, 0,
		sin(rad), cos(rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}
inline Matrix4 Matrix4::operator*(const Matrix4& other) const
{
	return ProductMatrix4(other);
}
inline Matrix4 Matrix4::operator/(const Matrix4& other)const
{
	return ProductMatrix4(other.Inverse());
}
inline Matrix4 Matrix4::operator+(const Matrix4& other) const
{
	return AddMatrix4(other);
}
inline Matrix4 Matrix4::operator-(const Matrix4& other) const
{
	return Matrix4();
}
inline Matrix4& Matrix4::operator*=(const Matrix4& other)
{
	Matrix4 temps;
	temps[0][0] = m[0][0] * other[0][0] + m[0][1] * other[1][0] + m[0][2] * other[2][0] + m[0][3] * other[3][0];
	temps[0][1] = m[0][0] * other[0][1] + m[0][1] * other[1][1] + m[0][2] * other[2][1] + m[0][3] * other[3][1];
	temps[0][2] = m[0][0] * other[0][2] + m[0][1] * other[1][2] + m[0][2] * other[2][2] + m[0][3] * other[3][2];
	temps[0][3] = m[0][0] * other[0][3] + m[0][1] * other[1][3] + m[0][2] * other[2][3] + m[0][3] * other[3][3];
	temps[1][0] = m[1][0] * other[0][0] + m[1][1] * other[1][0] + m[1][2] * other[2][0] + m[1][3] * other[3][0];
	temps[1][1] = m[1][0] * other[0][1] + m[1][1] * other[1][1] + m[1][2] * other[2][1] + m[1][3] * other[3][1];
	temps[1][2] = m[1][0] * other[0][2] + m[1][1] * other[1][2] + m[1][2] * other[2][2] + m[1][3] * other[3][2];
	temps[1][3] = m[1][0] * other[0][3] + m[1][1] * other[1][3] + m[1][2] * other[2][3] + m[1][3] * other[3][3];
	temps[2][0] = m[2][0] * other[0][0] + m[2][1] * other[1][0] + m[2][2] * other[2][0] + m[2][3] * other[3][0];
	temps[2][1] = m[2][0] * other[0][1] + m[2][1] * other[1][1] + m[2][2] * other[2][1] + m[2][3] * other[3][1];
	temps[2][2] = m[2][0] * other[0][2] + m[2][1] * other[1][2] + m[2][2] * other[2][2] + m[2][3] * other[3][2];
	temps[2][3] = m[2][0] * other[0][3] + m[2][1] * other[1][3] + m[2][2] * other[2][3] + m[2][3] * other[3][3];
	temps[3][0] = m[3][0] * other[0][0] + m[3][1] * other[1][0] + m[3][2] * other[2][0] + m[3][3] * other[3][0];
	temps[3][1] = m[3][0] * other[0][1] + m[3][1] * other[1][1] + m[3][2] * other[2][1] + m[3][3] * other[3][1];
	temps[3][2] = m[3][0] * other[0][2] + m[3][1] * other[1][2] + m[3][2] * other[2][2] + m[3][3] * other[3][2];
	temps[3][3] = m[3][0] * other[0][3] + m[3][1] * other[1][3] + m[3][2] * other[2][3] + m[3][3] * other[3][3];

	m[0][0] = temps[0][0];
	m[0][1] = temps[0][1];
	m[0][2] = temps[0][2];
	m[0][3] = temps[0][3];
	m[1][0] = temps[1][0];
	m[1][1] = temps[1][1];
	m[1][2] = temps[1][2];
	m[1][3] = temps[1][3];
	m[2][0] = temps[2][0];
	m[2][1] = temps[2][1];
	m[2][2] = temps[2][2];
	m[2][3] = temps[2][3];
	m[3][0] = temps[3][0];
	m[3][1] = temps[3][1];
	m[3][2] = temps[3][2];
	m[3][3] = temps[3][3];

	return *this;
}
inline Matrix4& Matrix4::operator/=(const Matrix4& other)
{
	*this *= other.Inverse();
	return *this;
}
inline Matrix4& Matrix4::operator+=(const Matrix4& other)
{
	m[0][0] += other[0][0];
	m[0][1] += other[0][1];
	m[0][2] += other[0][2];
	m[0][3] += other[0][3];
	m[1][0] += other[1][0];
	m[1][1] += other[1][1];
	m[1][2] += other[1][2];
	m[1][3] += other[1][3];
	m[2][0] += other[2][0];
	m[2][1] += other[2][1];
	m[2][2] += other[2][2];
	m[2][3] += other[2][3];
	m[3][0] += other[3][0];
	m[3][1] += other[3][1];
	m[3][2] += other[3][2];
	m[3][3] += other[3][3];

	return *this;
}
inline Matrix4& Matrix4::operator-=(const Matrix4& other)
{
	m[0][0] -= other[0][0];
	m[0][1] -= other[0][1];
	m[0][2] -= other[0][2];
	m[0][3] -= other[0][3];
	m[1][0] -= other[1][0];
	m[1][1] -= other[1][1];
	m[1][2] -= other[1][2];
	m[1][3] -= other[1][3];
	m[2][0] -= other[2][0];
	m[2][1] -= other[2][1];
	m[2][2] -= other[2][2];
	m[2][3] -= other[2][3];
	m[3][0] -= other[3][0];
	m[3][1] -= other[3][1];
	m[3][2] -= other[3][2];
	m[3][3] -= other[3][3];

	return *this;
}
inline Matrix4 Matrix4::operator*(float n)
{
	return ProductScalar(n);
}