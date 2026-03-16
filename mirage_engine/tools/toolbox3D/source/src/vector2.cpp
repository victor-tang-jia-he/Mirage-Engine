#include "vector2.h"
#include <cmath>
#include <iostream>
#include <assert.h>

using namespace MirageEngine::Core::Math;
Vector2 Vector2::Opposed() const
{
	return Vector2(-x, -y);
}
Vector2 Vector2::AddVector2(const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}
Vector2 Vector2::AddScalar(float scalar) const
{
	return Vector2(x + scalar, y + scalar);
}
Vector2 Vector2::ElementWiseProduct(const Vector2& other) const
{
	return Vector2(x * other.x, y * other.y);
}
Vector2 Vector2::ProductScalar(float scalar) const
{
	return Vector2(x * scalar, y * scalar);
}
bool Vector2::NearlyEqual(const Vector2& other, float epsilon) const
{
	return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon;
}
Vector2 Vector2::Mid(const Vector2& other) const
{
	return AddVector2(other).ProductScalar(1.f / 2.f);
}
float Vector2::Distance(const Vector2& other) const
{
	return sqrt(DistancePow(other));
}
float Vector2::DistancePow(const Vector2& other) const
{
	return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y);
}
float Vector2::Norm() const
{
	return sqrt(x * x + y * y);
}
float Vector2::SquareNorm() const
{
	return x * x + y * y;
}
Vector2 Vector2::Normalized() const
{
	Vector2 vec;
	float magnitude = Norm();

	if (magnitude == 0)
	{
		return Vector2(0, 0);
	}

	vec.x = x / magnitude;
	vec.y = y / magnitude;
	return vec;
}
float Vector2::Perspective(float t) const
{
	if (t <= 0) return 0;

	return SquareNorm() / t;
}
float Vector2::DotProductVector2(const Vector2& other) const
{
	return x * other.x + y * other.y;
}
float Vector2::CrossProductVector2(const Vector2& other) const
{
	return x * other.y - y * other.x;
}
float Vector2::Angle(const Vector2& v2) const
{
	float denom = Norm() * v2.Norm();
	if (denom == 0.0f) return 0.0f;

	return acosf(DotProductVector2(v2) / denom);
}
Vector2 Vector2::Rotate(float rad) const
{
	float cosA = cosf(rad);
	float sinA = sinf(rad);

	return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
}
void Vector2::Print() const
{
	std::cout << "x : " << x << " y : " << y << "\n";
}

Vector2 Vector2::operator+(const Vector2& vec) const
{
	return AddVector2(vec);
}
Vector2 Vector2::operator-(const Vector2& vec) const
{
	return AddVector2(vec.Opposed());
}
Vector2 Vector2::operator*(const Vector2& vec) const
{
	return ElementWiseProduct(vec);
}
Vector2 Vector2::operator/(const Vector2& vec) const
{
	assert(vec.x != 0 && vec.y != 0 && "Division by zero");
	return Vector2(x / vec.x, y / vec.y);
}
Vector2& Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}
Vector2& Vector2::operator-=(const Vector2& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}
Vector2& Vector2::operator*=(const Vector2& vec)
{
	x *= vec.x;
	y *= vec.y;
	return *this;
}
Vector2& Vector2::operator/=(const Vector2& vec)
{
	x /= vec.x;
	y /= vec.y;
	return *this;
}
Vector2 Vector2::operator+(float scalar) const
{
	return AddScalar(scalar);
}
Vector2 Vector2::operator-(float scalar) const
{
	return AddScalar(-scalar);
}
Vector2 Vector2::operator-() const
{
	return Vector2{ -x, -y };
}
Vector2 Vector2::operator*(float scalar) const
{
	return ProductScalar(scalar);
}
Vector2 Vector2::operator/(float scalar) const
{
	assert(scalar != 0.0f && "Division by zero");
	float invScalar = (scalar != 0.0f) ? 1.0f / scalar : 0.0f;
	return ProductScalar(invScalar);
}
Vector2& Vector2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}
Vector2& Vector2::operator/=(float scalar)
{
	assert(scalar != 0.0f && "Division by zero");
	float invScalar = (scalar != 0.0f) ? 1.0f / scalar : 0.0f;
	x *= invScalar;
	y *= invScalar;
	return *this;
}
float& Vector2::operator[](int index)
{
	assert(index >= 0 && index < 2 && "Index out of range");
	return (index == 0) ? x : y;
}
const float& Vector2::operator[](int index) const
{
	assert(index >= 0 && index < 2 && "Index out of range");
	return (index == 0) ? x : y;
}
bool MirageEngine::Core::Math::Vector2::operator==(const Vector2& other) const
{
	return x == other.x && y == other.y;
}
bool MirageEngine::Core::Math::Vector2::operator!=(const Vector2& other) const
{
	return !(*this == other);
}
Vector2 MirageEngine::Core::Math::operator*(float scalar, const Vector2& vec)
{
	return { vec.x * scalar, vec.y * scalar };
}
std::ostream& MirageEngine::Core::Math::operator<<(std::ostream& os, const Vector2& vec)
{
	return os << "(" << vec.x << ", " << vec.y << ")";
}