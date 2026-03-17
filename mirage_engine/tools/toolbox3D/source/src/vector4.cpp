#include "vector4.h"
#include <iostream>
#include <array>
#include <assert.h>

using namespace MirageEngine::Core::Math;
Vector4 Vector4::Opposed() const
{
	return Vector4(-x, -y, -z, -w);
}
Vector4 Vector4::AddVector4(const Vector4& other) const
{
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}
Vector4 Vector4::AddScalar(float scalar) const
{
	return Vector4(x + scalar, y + scalar, z + scalar, w + scalar);
}
Vector4 Vector4::ElementWiseProduct(const Vector4& other) const
{
	return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}
Vector4 Vector4::ProductScalar(float scalar) const
{
	return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}
bool MirageEngine::Core::Math::Vector4::NearlyEqual(const Vector4& other, float epsilon) const
{
	return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon && std::abs(z - other.z) < epsilon && std::abs(w - other.w) < epsilon;
}
Vector4 Vector4::Mid(const Vector4& other) const
{
	return AddVector4(other).ProductScalar(1.f / 2.f);
}
float Vector4::Distance(const Vector4& other) const
{
	return sqrt(DistancePow(other));
}
float Vector4::DistancePow(const Vector4& other) const
{
	return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y) + (other.z - z) * (other.z - z) + (other.w - w) * (other.w - w);
}
float Vector4::Norm() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}
float Vector4::SquareNorm() const
{
	return x * x + y * y + z * z + w * w;
}
Vector4 Vector4::Normalized() const
{
	Vector4 vec;
	float magnitude = Norm();

	if (magnitude == 0)
	{
		return Vector4(0, 0, 0, 0);
	}

	vec.x = x / magnitude;
	vec.y = y / magnitude;
	vec.z = z / magnitude;
	vec.w = w / magnitude;
	return vec;
}
float Vector4::Perspective(float t) const
{
	if (t <= 0) return 0;

	return SquareNorm() / t;
}
float Vector4::DotProductVector4(const Vector4& other) const
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}
std::array<float, 4> Vector4::AsList() const
{
	return { x, y, z, w };
}
void Vector4::Print() const
{
	std::cout << "x : " << x << " y : " << y << " z : " << z << " w : " << w << "\n";
}

Vector4 Vector4::operator+(const Vector4& vec) const
{
	return AddVector4(vec);
}
Vector4 Vector4::operator-(const Vector4& vec) const
{
	return AddVector4(vec.Opposed());
}
Vector4 Vector4::operator*(const Vector4& vec) const
{
	return ElementWiseProduct(vec);
}
Vector4 Vector4::operator/(const Vector4& vec) const
{
	assert(vec.x != 0 && vec.y != 0 && vec.z != 0 && vec.w != 0 && "Division by zero");
	return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
}
Vector4& Vector4::operator += (const Vector4& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}
Vector4& Vector4::operator -= (const Vector4& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}
Vector4& Vector4::operator *= (const Vector4& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	w *= vec.w;
	return *this;
}
Vector4& Vector4::operator/=(const Vector4& vec)
{
	assert(vec.x != 0 && vec.y != 0 && vec.z != 0 && vec.w != 0 && "Division by zero");
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	w /= vec.w;
	return *this;
}
Vector4 Vector4::operator+(float scalar) const
{
	return AddScalar(scalar);
}
Vector4 Vector4::operator-(float scalar) const
{
	return AddScalar(-scalar);
}
Vector4 Vector4::operator*(float scalar) const
{
	return ProductScalar(scalar);
}
Vector4 Vector4::operator/(float scalar) const
{
	assert(scalar != 0.0f && "Division by zero");
	float invScalar = (scalar != 0.0f) ? 1.0f / scalar : 0.0f;
	return ProductScalar(invScalar);
}
Vector4& Vector4::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}
Vector4& Vector4::operator/=(float scalar)
{
	assert(scalar != 0.0f && "Division by zero");
	float invScalar = (scalar != 0.0f) ? 1.0f / scalar : 0.0f;
	x *= invScalar;
	y *= invScalar;
	z *= invScalar;
	w *= invScalar;
	return *this;
}
Vector4 Vector4::operator-() const
{
	return Vector4{ -x, -y, -z, -w };
}
float& Vector4::operator[](int index)
{
	assert(index >= 0 && index < 4 && "Index out of range");
	return (index == 0) ? x : (index == 1) ? y : (index == 2) ? z : w;
}
const float& Vector4::operator[](int index) const
{
	assert(index >= 0 && index < 4 && "Index out of range");
	return (index == 0) ? x : (index == 1) ? y : (index == 2) ? z : w;
}
bool Vector4::operator==(const Vector4& other) const
{
	return x == other.x && y == other.y && z == other.z && w == other.w;
}
bool Vector4::operator!=(const Vector4& other) const
{
	return !(*this == other);
}
Vector3 MirageEngine::Core::Math::Vector4::operator=(const Vector4& vec4)
{
	return Vector3(x, y, z);
}
Vector4 MirageEngine::Core::Math::operator*(float scalar, const Vector4& vec)
{
	return { vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar };
}
Vector4::operator Vector3() const
{
	return Vector3(x, y, z);
}
Vector4::operator Vector2() const
{
	return Vector2(x, y);
}
std::ostream& MirageEngine::Core::Math::operator<<(std::ostream& os, const Vector4& vec)
{
	return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
}
