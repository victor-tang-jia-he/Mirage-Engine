#include "vector2.h"
#include <cmath>
#include <iostream>
#include <assert.h>

using namespace MirageEngine::Core::Maths;
Vector2 Vector2::GetOpposed() const
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
Vector2 Vector2::GetMid(const Vector2& other) const
{
	return AddVector2(other).ProductScalar(1.f / 2.f);
}
float Vector2::GetDistance(const Vector2& other) const
{
	return sqrt(GetDistancePow(other));
}
float Vector2::GetDistancePow(const Vector2& other) const
{
	return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y);
}
float Vector2::GetNorm() const
{
	return sqrt(x * x + y * y);
}
float Vector2::GetSquareNorm() const
{
	return x * x + y * y;
}
Vector2 Vector2::Normalized() const
{
	Vector2 vec;
	float magnitude = GetNorm();

	if (magnitude == 0)
	{
		return Vector2(0, 0);
	}

	vec.x = x / magnitude;
	vec.y = y / magnitude;
	return vec;
}
float Vector2::GetPerspective(float t) const
{
	if (t <= 0) return 0;

	return GetSquareNorm() / t;
}
float Vector2::DotProductVector2(const Vector2& other) const
{
	return x * other.x + y * other.y;
}
float Vector2::CrossProductVector2(const Vector2& other) const
{
	return x * other.y - y * other.x;
}
float Vector2::GetAngle(const Vector2& v2) const
{
	float denom = GetNorm() * v2.GetNorm();
	if (denom == 0.0f) return 0.0f;

	return acosf(DotProductVector2(v2) / denom) * (180.f / PI);
}
void Vector2::Rotation(float angle)
{
	float rad = ToRad(angle);
	float cosA = cosf(rad);
	float sinA = sinf(rad);

	float px = x * cosA - y * sinA;
	float py = x * sinA + y * cosA;

	x = px;
	y = py;
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
	return AddVector2(vec.GetOpposed());
}
Vector2 Vector2::operator*(const Vector2& vec) const
{
	return ElementWiseProduct(vec);
}
Vector2 Vector2::operator/(const Vector2& vec) const
{
	assert(vec.x != 0 && vec.y != 0 && "Division by zero in Vector2D");
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
	assert(scalar != 0.0f && "Division par z¨¦ro");
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
	assert(scalar != 0.0f && "Division par z¨¦ro");
	float invScalar = (scalar != 0.0f) ? 1.0f / scalar : 0.0f;
	x *= invScalar;
	y *= invScalar;
	return *this;
}
float& Vector2::operator[](int index)
{
	assert(index >= 0 && index < 2 && "Index out of range in Vector2D");
	return (index == 0) ? x : y;
}
const float& Vector2::operator[](int index) const
{
	assert(index >= 0 && index < 2 && "Index out of range in Vector2D");
	return (index == 0) ? x : y;
}
bool MirageEngine::Core::Maths::Vector2::operator==(const Vector2& other) const
{
	return x == other.x && y == other.y;
}
bool MirageEngine::Core::Maths::Vector2::operator!=(const Vector2& other) const
{
	return !(*this == other);
}
Vector2 MirageEngine::Core::Maths::operator*(float scalar, const Vector2& vec)
{
	return { vec.x * scalar, vec.y * scalar };
}
std::ostream& MirageEngine::Core::Maths::operator<<(std::ostream& os, const Vector2& vec)
{
	return os << "(" << vec.x << ", " << vec.y << ")";
}