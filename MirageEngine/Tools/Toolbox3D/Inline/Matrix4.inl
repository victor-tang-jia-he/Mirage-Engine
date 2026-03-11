#include "../Include/Matrix4.h"

using namespace Bwt::Core::Maths;
inline Matrix4::Matrix4(const Matrix4& copy)
{
	tab[0][0] = copy.tab[0][0]; tab[0][1] = copy.tab[0][1]; tab[0][2] = copy.tab[0][2]; tab[0][3] = copy.tab[0][3];
	tab[1][0] = copy.tab[1][0]; tab[1][1] = copy.tab[1][1]; tab[1][2] = copy.tab[1][2]; tab[1][3] = copy.tab[1][3];
	tab[2][0] = copy.tab[2][0]; tab[2][1] = copy.tab[2][1]; tab[2][2] = copy.tab[2][2]; tab[2][3] = copy.tab[2][3];
	tab[3][0] = copy.tab[3][0]; tab[3][1] = copy.tab[3][1]; tab[3][2] = copy.tab[3][2]; tab[3][3] = copy.tab[3][3];
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
	return Vector4(tab[0][0], tab[1][1], tab[2][2], tab[3][3]);
}
inline float Matrix4::Trace()
{
	Vector4 vec = Diagonal();
	return vec.x + vec.y + vec.z + vec.w;
}
inline Matrix4 Matrix4::Opposed() const
{
	return	Matrix4(
		-tab[0][0], -tab[0][1], -tab[0][2], -tab[0][3],
		-tab[1][0], -tab[1][1], -tab[1][2], -tab[1][3],
		-tab[2][0], -tab[2][1], -tab[2][2], -tab[2][3],
		-tab[3][0], -tab[3][1], -tab[3][2], -tab[3][3]);
}
inline Matrix4 Matrix4::Transpose() const
{
	return Matrix4(
		tab[0][0], tab[1][0], tab[2][0], tab[3][0],
		tab[0][1], tab[1][1], tab[2][1], tab[3][1],
		tab[0][2], tab[1][2], tab[2][2], tab[3][2],
		tab[0][3], tab[1][3], tab[2][3], tab[3][3]);
}
inline Matrix4 Matrix4::AddMatrix4(const Matrix4& other) const
{
	return	Matrix4(
		tab[0][0] + other.tab[0][0], tab[0][1] + other.tab[0][1], tab[0][2] + other.tab[0][2], tab[0][3] + other.tab[0][3],
		tab[1][0] + other.tab[1][0], tab[1][1] + other.tab[1][1], tab[1][2] + other.tab[1][2], tab[1][3] + other.tab[1][3],
		tab[2][0] + other.tab[2][0], tab[2][1] + other.tab[2][1], tab[2][2] + other.tab[2][2], tab[2][3] + other.tab[2][3],
		tab[3][0] + other.tab[3][0], tab[3][1] + other.tab[3][1], tab[3][2] + other.tab[3][2], tab[3][3] + other.tab[3][3]);
}
inline Matrix4 Matrix4::ProductScalar(float n)
{
	return	Matrix4(
		tab[0][0] * n, tab[0][1] * n, tab[0][2] * n, tab[0][3] * n,
		tab[1][0] * n, tab[1][1] * n, tab[1][2] * n, tab[1][3] * n,
		tab[2][0] * n, tab[2][1] * n, tab[2][2] * n, tab[2][3] * n,
		tab[3][0] * n, tab[3][1] * n, tab[3][2] * n, tab[3][3] * n);
}
inline Matrix4 Matrix4::ProductMatrix4(const Matrix4& other) const
{
	return Matrix4(
		tab[0][0] * other.tab[0][0] + tab[0][1] * other.tab[1][0] + tab[0][2] * other.tab[2][0] + tab[0][3] * other.tab[3][0],
		tab[0][0] * other.tab[0][1] + tab[0][1] * other.tab[1][1] + tab[0][2] * other.tab[2][1] + tab[0][3] * other.tab[3][1],
		tab[0][0] * other.tab[0][2] + tab[0][1] * other.tab[1][2] + tab[0][2] * other.tab[2][2] + tab[0][3] * other.tab[3][2],
		tab[0][0] * other.tab[0][3] + tab[0][1] * other.tab[1][3] + tab[0][2] * other.tab[2][3] + tab[0][3] * other.tab[3][3],
		tab[1][0] * other.tab[0][0] + tab[1][1] * other.tab[1][0] + tab[1][2] * other.tab[2][0] + tab[1][3] * other.tab[3][0],
		tab[1][0] * other.tab[0][1] + tab[1][1] * other.tab[1][1] + tab[1][2] * other.tab[2][1] + tab[1][3] * other.tab[3][1],
		tab[1][0] * other.tab[0][2] + tab[1][1] * other.tab[1][2] + tab[1][2] * other.tab[2][2] + tab[1][3] * other.tab[3][2],
		tab[1][0] * other.tab[0][3] + tab[1][1] * other.tab[1][3] + tab[1][2] * other.tab[2][3] + tab[1][3] * other.tab[3][3],
		tab[2][0] * other.tab[0][0] + tab[2][1] * other.tab[1][0] + tab[2][2] * other.tab[2][0] + tab[2][3] * other.tab[3][0],
		tab[2][0] * other.tab[0][1] + tab[2][1] * other.tab[1][1] + tab[2][2] * other.tab[2][1] + tab[2][3] * other.tab[3][1],
		tab[2][0] * other.tab[0][2] + tab[2][1] * other.tab[1][2] + tab[2][2] * other.tab[2][2] + tab[2][3] * other.tab[3][2],
		tab[2][0] * other.tab[0][3] + tab[2][1] * other.tab[1][3] + tab[2][2] * other.tab[2][3] + tab[2][3] * other.tab[3][3],
		tab[3][0] * other.tab[0][0] + tab[3][1] * other.tab[1][0] + tab[3][2] * other.tab[2][0] + tab[3][3] * other.tab[3][0],
		tab[3][0] * other.tab[0][1] + tab[3][1] * other.tab[1][1] + tab[3][2] * other.tab[2][1] + tab[3][3] * other.tab[3][1],
		tab[3][0] * other.tab[0][2] + tab[3][1] * other.tab[1][2] + tab[3][2] * other.tab[2][2] + tab[3][3] * other.tab[3][2],
		tab[3][0] * other.tab[0][3] + tab[3][1] * other.tab[1][3] + tab[3][2] * other.tab[2][3] + tab[3][3] * other.tab[3][3]);
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
	temps.tab[0][0] = tab[0][0] * other.tab[0][0] + tab[0][1] * other.tab[1][0] + tab[0][2] * other.tab[2][0] + tab[0][3] * other.tab[3][0];
	temps.tab[0][1] = tab[0][0] * other.tab[0][1] + tab[0][1] * other.tab[1][1] + tab[0][2] * other.tab[2][1] + tab[0][3] * other.tab[3][1];
	temps.tab[0][2] = tab[0][0] * other.tab[0][2] + tab[0][1] * other.tab[1][2] + tab[0][2] * other.tab[2][2] + tab[0][3] * other.tab[3][2];
	temps.tab[0][3] = tab[0][0] * other.tab[0][3] + tab[0][1] * other.tab[1][3] + tab[0][2] * other.tab[2][3] + tab[0][3] * other.tab[3][3];
	temps.tab[1][0] = tab[1][0] * other.tab[0][0] + tab[1][1] * other.tab[1][0] + tab[1][2] * other.tab[2][0] + tab[1][3] * other.tab[3][0];
	temps.tab[1][1] = tab[1][0] * other.tab[0][1] + tab[1][1] * other.tab[1][1] + tab[1][2] * other.tab[2][1] + tab[1][3] * other.tab[3][1];
	temps.tab[1][2] = tab[1][0] * other.tab[0][2] + tab[1][1] * other.tab[1][2] + tab[1][2] * other.tab[2][2] + tab[1][3] * other.tab[3][2];
	temps.tab[1][3] = tab[1][0] * other.tab[0][3] + tab[1][1] * other.tab[1][3] + tab[1][2] * other.tab[2][3] + tab[1][3] * other.tab[3][3];
	temps.tab[2][0] = tab[2][0] * other.tab[0][0] + tab[2][1] * other.tab[1][0] + tab[2][2] * other.tab[2][0] + tab[2][3] * other.tab[3][0];
	temps.tab[2][1] = tab[2][0] * other.tab[0][1] + tab[2][1] * other.tab[1][1] + tab[2][2] * other.tab[2][1] + tab[2][3] * other.tab[3][1];
	temps.tab[2][2] = tab[2][0] * other.tab[0][2] + tab[2][1] * other.tab[1][2] + tab[2][2] * other.tab[2][2] + tab[2][3] * other.tab[3][2];
	temps.tab[2][3] = tab[2][0] * other.tab[0][3] + tab[2][1] * other.tab[1][3] + tab[2][2] * other.tab[2][3] + tab[2][3] * other.tab[3][3];
	temps.tab[3][0] = tab[3][0] * other.tab[0][0] + tab[3][1] * other.tab[1][0] + tab[3][2] * other.tab[2][0] + tab[3][3] * other.tab[3][0];
	temps.tab[3][1] = tab[3][0] * other.tab[0][1] + tab[3][1] * other.tab[1][1] + tab[3][2] * other.tab[2][1] + tab[3][3] * other.tab[3][1];
	temps.tab[3][2] = tab[3][0] * other.tab[0][2] + tab[3][1] * other.tab[1][2] + tab[3][2] * other.tab[2][2] + tab[3][3] * other.tab[3][2];
	temps.tab[3][3] = tab[3][0] * other.tab[0][3] + tab[3][1] * other.tab[1][3] + tab[3][2] * other.tab[2][3] + tab[3][3] * other.tab[3][3];

	tab[0][0] = temps.tab[0][0];
	tab[0][1] = temps.tab[0][1];
	tab[0][2] = temps.tab[0][2];
	tab[0][3] = temps.tab[0][3];
	tab[1][0] = temps.tab[1][0];
	tab[1][1] = temps.tab[1][1];
	tab[1][2] = temps.tab[1][2];
	tab[1][3] = temps.tab[1][3];
	tab[2][0] = temps.tab[2][0];
	tab[2][1] = temps.tab[2][1];
	tab[2][2] = temps.tab[2][2];
	tab[2][3] = temps.tab[2][3];
	tab[3][0] = temps.tab[3][0];
	tab[3][1] = temps.tab[3][1];
	tab[3][2] = temps.tab[3][2];
	tab[3][3] = temps.tab[3][3];

	return *this;
}
inline Matrix4& Matrix4::operator/=(const Matrix4& other)
{
	*this *= other.Inverse();
	return *this;
}
inline Matrix4& Matrix4::operator+=(const Matrix4& other)
{
	tab[0][0] += other.tab[0][0];
	tab[0][1] += other.tab[0][1];
	tab[0][2] += other.tab[0][2];
	tab[0][3] += other.tab[0][3];
	tab[1][0] += other.tab[1][0];
	tab[1][1] += other.tab[1][1];
	tab[1][2] += other.tab[1][2];
	tab[1][3] += other.tab[1][3];
	tab[2][0] += other.tab[2][0];
	tab[2][1] += other.tab[2][1];
	tab[2][2] += other.tab[2][2];
	tab[2][3] += other.tab[2][3];
	tab[3][0] += other.tab[3][0];
	tab[3][1] += other.tab[3][1];
	tab[3][2] += other.tab[3][2];
	tab[3][3] += other.tab[3][3];

	return *this;
}
inline Matrix4& Matrix4::operator-=(const Matrix4& other)
{
	tab[0][0] -= other.tab[0][0];
	tab[0][1] -= other.tab[0][1];
	tab[0][2] -= other.tab[0][2];
	tab[0][3] -= other.tab[0][3];
	tab[1][0] -= other.tab[1][0];
	tab[1][1] -= other.tab[1][1];
	tab[1][2] -= other.tab[1][2];
	tab[1][3] -= other.tab[1][3];
	tab[2][0] -= other.tab[2][0];
	tab[2][1] -= other.tab[2][1];
	tab[2][2] -= other.tab[2][2];
	tab[2][3] -= other.tab[2][3];
	tab[3][0] -= other.tab[3][0];
	tab[3][1] -= other.tab[3][1];
	tab[3][2] -= other.tab[3][2];
	tab[3][3] -= other.tab[3][3];

	return *this;
}
inline Matrix4 Matrix4::operator*(float n)
{
	return ProductScalar(n);
}