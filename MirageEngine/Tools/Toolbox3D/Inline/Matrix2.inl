#include "../Include/Matrix2.h"

using namespace Bwt::Core::Maths;

inline Matrix2::Matrix2(const Matrix2& copy)
{
	m[0][0] = copy.m[0][0];
	m[1][0] = copy.m[1][0];
	m[0][1] = copy.m[0][1];
	m[1][1] = copy.m[1][1];
}
inline Matrix2::Matrix2(float m00, float m01, float m10, float m11)
{
	m[0][0] = m00; m[0][1] = m01;
	m[1][0] = m10; m[1][1] = m11;
}
inline Matrix2 Matrix2::Identity()
{
	return Matrix2(1, 0, 0, 1);
}
inline Vector2 Matrix2::Diagonal() const
{
	return Vector2(m[0][0], m[1][1]);
}
inline float Matrix2::Trace() const
{
	return m[0][0] + m[1][1];
}
inline Matrix2 Matrix2::Opposed() const
{
	return	Matrix2(-m[0][0], -m[0][1], -m[1][0], -m[1][1]);
}
inline Matrix2 Matrix2::Transpose() const
{
	return Matrix2(
		m[0][0], m[0][1],
		m[1][0], m[1][1]);
}
inline Matrix2 Matrix2::AddMatrix2(const Matrix2& other) const
{
	return Matrix2(
		m[0][0] + other.m[0][0], m[0][1] + other.m[0][1],
		m[1][0] + other.m[1][0], m[1][1] + other.m[1][1]
	);
}
inline Matrix2 Matrix2::ProductScalar(float n) const
{
	return Matrix2(m[0][0] * n, m[0][1] * n,
		m[1][0] * n, m[1][1] * n);
}
inline Matrix2 Matrix2::ProductMatrix2(const Matrix2& other) const
{
	return Matrix2(
		m[0][0] * other.m[0][0] + m[1][0] * other.m[0][1],
		m[0][1] * other.m[0][0] + m[1][1] * other.m[0][1],

		m[0][0] * other.m[1][0] + m[1][0] * other.m[1][1],
		m[0][1] * other.m[1][0] + m[1][1] * other.m[1][1]
	);

}
inline float Matrix2::Determinant() const
{
	return m[0][0] * m[1][1] - m[1][0] * m[0][1];
}
inline Matrix2 Matrix2::Inverse() const
{
	float det = Determinant();

	if (std::abs(det) < 1e-6f)
	{
		throw std::runtime_error("Matrix is not invertible (det == 0)");
	}

	return Matrix2(
		m[1][1] / det, -m[0][1] / det,
		-m[1][0] / det, m[0][0] / det
	);
}
inline Matrix2 Matrix2::Rotation(float angle)
{
	return Matrix2(cos(angle), sin(angle), -sin(angle), cos(angle));
}
inline void Matrix2::Print() const
{
	std::cout
		<< m[0][0] << " " << m[0][1] << "\n"
		<< m[1][0] << " " << m[1][1] << "\n";
}
inline Matrix2 Matrix2::operator*(const Matrix2& other)const
{
	return ProductMatrix2(other);
}
inline Matrix2 Matrix2::operator/(const Matrix2& other)const
{
	return ProductMatrix2(other.Inverse());
}
inline Matrix2 Matrix2::operator+(const Matrix2& other) const
{
	return AddMatrix2(other);
}
inline Matrix2 Matrix2::operator-(const Matrix2& other) const
{
	return AddMatrix2(other.Opposed());
}
inline Matrix2& Matrix2::operator*=(const Matrix2& other)
{
	float m00 = m[0][0] * other.m[0][0] + m[1][0] * other.m[0][1];
	float m01 = m[0][1] * other.m[0][0] + m[1][1] * other.m[0][1];
	float m10 = m[0][0] * other.m[1][0] + m[1][0] * other.m[1][1];
	float m11 = m[0][1] * other.m[1][0] + m[1][1] * other.m[1][1];

	m[0][0] = m00;
	m[0][1] = m01;
	m[1][0] = m10;
	m[1][1] = m11;

	return *this;
}
inline Matrix2& Matrix2::operator/=(const Matrix2& other)
{
	*this *= other.Inverse();
	return *this;
}
inline Matrix2& Matrix2::operator+=(const Matrix2& other)
{
	m[0][0] += other.m[0][0];
	m[0][1] += other.m[0][1];
	m[1][0] += other.m[1][0];
	m[1][1] += other.m[1][1];

	return *this;
}
inline Matrix2& Matrix2::operator-=(const Matrix2& other)
{
	m[0][0] -= other.m[0][0];
	m[0][1] -= other.m[0][1];
	m[1][0] -= other.m[1][0];
	m[1][1] -= other.m[1][1];

	return *this;
}
inline Matrix2 Matrix2::operator*(float n) const
{
	return ProductScalar(n);
}
inline Matrix2& Matrix2::operator*=(float n)
{
	m[0][0] = m[0][0] * n;
	m[0][1] = m[0][1] * n;
	m[1][0] = m[1][0] * n;
	m[1][1] = m[1][1] * n;
	return *this;
}
inline bool Matrix2::operator==(const Matrix2& other) const
{
	return 
		std::abs(m[0][0] - other.m[0][0]) < 1e-6f &&
		std::abs(m[0][1] - other.m[0][1]) < 1e-6f &&
		std::abs(m[1][0] - other.m[1][0]) < 1e-6f &&
		std::abs(m[1][1] - other.m[1][1]) < 1e-6f;
}
