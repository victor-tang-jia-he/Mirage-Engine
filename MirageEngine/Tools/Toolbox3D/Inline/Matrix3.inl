#include "../Include/Matrix3.h"

using namespace Bwt::Core::Maths;
inline Matrix3::Matrix3(const Matrix3& copy)
{
	m[0][0] = copy.m[0][0]; m[0][1] = copy.m[0][1]; m[0][2] = copy.m[0][2];
	m[1][0] = copy.m[1][0]; m[1][1] = copy.m[1][1]; m[1][2] = copy.m[1][2];
	m[2][0] = copy.m[2][0]; m[2][1] = copy.m[2][1]; m[2][2] = copy.m[2][2];
};
inline Matrix3 Matrix3::Identity()
{
	return Matrix3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f);
}
inline Vector3 Matrix3::Diagonal() const
{
	return Vector3(m[0][0], m[1][1], m[2][2]);
}
inline float Matrix3::Trace() const
{
	return m[0][0] + m[1][1] + m[2][2];
}
inline Matrix3 Matrix3::Opposed() const
{
	return Matrix3(
		-m[0][0], -m[0][1], -m[0][2],
		-m[1][0], -m[1][1], -m[1][2],
		-m[2][0], -m[2][1], -m[2][2]);
}
inline Matrix3 Matrix3::Transpose() const
{
	return Matrix3(
		m[0][0], m[1][0], m[2][0],
		m[0][1], m[1][1], m[2][1],
		m[0][2], m[1][2], m[2][2]);
}
inline Matrix3 Matrix3::AddMatrix3(const Matrix3& other) const
{
	return Matrix3(
		m[0][0] + other.m[0][0],
		m[0][1] + other.m[0][1],
		m[0][2] + other.m[0][2],
		m[1][0] + other.m[1][0],
		m[1][1] + other.m[1][1],
		m[1][2] + other.m[1][2],
		m[2][0] + other.m[2][0],
		m[2][1] + other.m[2][1],
		m[2][2] + other.m[2][2]);
}
inline Matrix3 Matrix3::ProductScalar(float n) const
{
	return Matrix3(
		m[0][0] * n,
		m[0][1] * n,
		m[0][2] * n,
		m[1][0] * n,
		m[1][1] * n,
		m[1][2] * n,
		m[2][0] * n,
		m[2][1] * n,
		m[2][2] * n);
}
inline Matrix3 Matrix3::ProductMatrix3(const Matrix3& other) const
{
	return Matrix3(
		// Colonne 1
		m[0][0] * other.m[0][0] + m[0][1] * other.m[1][0] + m[0][2] * other.m[2][0], // row 0
		m[0][0] * other.m[0][1] + m[0][1] * other.m[1][1] + m[0][2] * other.m[2][1], // row 0
		m[0][0] * other.m[0][2] + m[0][1] * other.m[1][2] + m[0][2] * other.m[2][2], // row 0

		// Colonne 2
		m[1][0] * other.m[0][0] + m[1][1] * other.m[1][0] + m[1][2] * other.m[2][0], // row 1
		m[1][0] * other.m[0][1] + m[1][1] * other.m[1][1] + m[1][2] * other.m[2][1], // row 1
		m[1][0] * other.m[0][2] + m[1][1] * other.m[1][2] + m[1][2] * other.m[2][2], // row 1

		// Colonne 3
		m[2][0] * other.m[0][0] + m[2][1] * other.m[1][0] + m[2][2] * other.m[2][0], // row 2
		m[2][0] * other.m[0][1] + m[2][1] * other.m[1][1] + m[2][2] * other.m[2][1], // row 2
		m[2][0] * other.m[0][2] + m[2][1] * other.m[1][2] + m[2][2] * other.m[2][2]  // row 2
	);
}
inline float Matrix3::Determinant() const
{
	return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
		m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
		m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

}
inline Matrix3 Matrix3::RotateMatrix(float _x, float _y, float _z)
{
	Matrix3 rotX = CreateXRotationMatrix(_x);
	Matrix3 rotY = CreateYRotationMatrix(_y);
	Matrix3 rotZ = CreateZRotationMatrix(_z);
	return rotZ * rotY * rotX;
}
inline Matrix3 Matrix3::CreateXRotationMatrix(float rad)
{
	return Matrix3(1.f, 0.f, 0.f,
		0.f, cos(rad), -sin(rad),
		0.f, sin(rad), cos(rad));
}
inline Matrix3 Matrix3::CreateYRotationMatrix(float rad)
{
	return Matrix3(
		cos(rad), 0.f, sin(rad),
		0.f, 1.f, 0.f,
		-sin(rad), 0.f, cos(rad));
}
inline Matrix3 Matrix3::CreateZRotationMatrix(float rad)
{
	return Matrix3(cos(rad), -sin(rad), 0.f,
		sin(rad), cos(rad), 0.f,
		0, 0, 1.f);
}
inline void Matrix3::Print() const
{
	std::cout
		<< m[0][0] << " " << m[0][1] << " " << m[0][2] << "\n"
		<< m[1][0] << " " << m[1][1] << " " << m[1][2] << "\n"
		<< m[2][0] << " " << m[2][1] << " " << m[2][2] << "\n";
}
inline Matrix3 Matrix3::operator*(const Matrix3& other) const
{
	return ProductMatrix3(other);
}
inline Matrix3 Matrix3::operator/(const Matrix3& other) const
{
	return ProductMatrix3(other.Inverse());
}
inline Matrix3 Matrix3::operator+(const Matrix3& other) const
{
	return AddMatrix3(other);
}
inline Matrix3 Matrix3::operator-(const Matrix3& other) const
{
	return AddMatrix3(other.Opposed());
}
inline Matrix3& Matrix3::operator*=(const Matrix3& other)
{
	float m1 = m[0][0] * other.m[0][0] + m[0][1] * other.m[1][0] + m[0][2] * other.m[2][0];
	float m2 = m[0][0] * other.m[0][1] + m[0][1] * other.m[1][1] + m[0][2] * other.m[2][1];
	float m3 = m[0][0] * other.m[0][2] + m[0][1] * other.m[1][2] + m[0][2] * other.m[2][2];
	float m4 = m[1][0] * other.m[0][0] + m[1][1] * other.m[1][0] + m[1][2] * other.m[2][0];
	float m5 = m[1][0] * other.m[0][1] + m[1][1] * other.m[1][1] + m[1][2] * other.m[2][1];
	float m6 = m[1][0] * other.m[0][2] + m[1][1] * other.m[1][2] + m[1][2] * other.m[2][2];
	float m7 = m[2][0] * other.m[0][0] + m[2][1] * other.m[1][0] + m[2][2] * other.m[2][0];
	float m8 = m[2][0] * other.m[0][1] + m[2][1] * other.m[1][1] + m[2][2] * other.m[2][1];
	float m9 = m[2][0] * other.m[0][2] + m[2][1] * other.m[1][2] + m[2][2] * other.m[2][2];

	m[0][0] = m1;
	m[0][1] = m2;
	m[0][2] = m3;
	m[1][0] = m4;
	m[1][1] = m5;
	m[1][2] = m6;
	m[2][0] = m7;
	m[2][1] = m8;
	m[2][2] = m9;

	return *this;
}
inline Matrix3& Matrix3::operator/=(const Matrix3& other)
{
	*this *= other.Inverse();
	return *this;
}
inline Matrix3& Matrix3::operator+=(const Matrix3& other)
{
	m[0][0] += other.m[0][0];
	m[0][1] += other.m[0][1];
	m[0][2] += other.m[0][2];
	m[1][0] += other.m[1][0];
	m[1][1] += other.m[1][1];
	m[1][2] += other.m[1][2];
	m[2][0] += other.m[2][0];
	m[2][1] += other.m[2][1];
	m[2][2] += other.m[2][2];

	return *this;
}
inline Matrix3& Matrix3::operator-=(const Matrix3& other)
{
	m[0][0] -= other.m[0][0];
	m[0][1] -= other.m[0][1];
	m[0][2] -= other.m[0][2];
	m[1][0] -= other.m[1][0];
	m[1][1] -= other.m[1][1];
	m[1][2] -= other.m[1][2];
	m[2][0] -= other.m[2][0];
	m[2][1] -= other.m[2][1];
	m[2][2] -= other.m[2][2];

	return *this;
}
inline Matrix3 Matrix3::operator*(float n)
{
	return ProductScalar(n);
}
inline Matrix3 Matrix3::operator*=(float n)
{
	m[0][0] *= n;
	m[0][1] *= n;
	m[0][2] *= n;
	m[1][0] *= n;
	m[1][1] *= n;
	m[1][2] *= n;
	m[2][0] *= n;
	m[2][1] *= n;
	m[2][2] *= n;

	return *this;
}

inline Vector3 Matrix3::operator*(const Vector3& v)
{
	return Vector3(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
	);
}
inline bool Matrix3::operator==(const Matrix3& other) const
{
	return
		std::abs(m[0][0] - other.m[0][0]) < EPSILON &&
		std::abs(m[0][1] - other.m[0][1]) < EPSILON &&
		std::abs(m[0][2] - other.m[0][2]) < EPSILON &&
		std::abs(m[1][0] - other.m[1][0]) < EPSILON &&
		std::abs(m[1][1] - other.m[1][1]) < EPSILON &&
		std::abs(m[1][2] - other.m[1][2]) < EPSILON &&
		std::abs(m[2][0] - other.m[2][0]) < EPSILON &&
		std::abs(m[2][1] - other.m[2][1]) < EPSILON &&
		std::abs(m[2][2] - other.m[2][2]) < EPSILON;
}
//inline Vector3 Matrix3::operator[](int index) const
//{
//	assert(index >= 0 && index < 3 && "Index out of range");
//	return Vector3(m[index * 3], m[index * 3 + 1], m[index * 3 + 2]);
//}
//inline Vector3& Matrix3::operator[](int index)
//{
//	assert(index >= 0 && index < 3 && "Index out of range");
//	return *reinterpret_cast<Vector3*>(&m[index * 3]);
//}