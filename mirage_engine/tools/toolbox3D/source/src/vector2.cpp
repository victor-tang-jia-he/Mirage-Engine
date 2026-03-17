#include "vector2.h"
#include <cmath>
#include <iostream>
#include <assert.h>

using namespace MirageEngine::Core::Math;
Vector2 Vector2::Opposed() const
{
	return Vector2(-X, -Y);
}
Vector2 Vector2::AddVector2(const Vector2& _other) const
{
	return Vector2(X + _other.X, Y + _other.Y);
}
Vector2 Vector2::AddScalar(float _scalar) const
{
	return Vector2(X + _scalar, Y + _scalar);
}
Vector2 Vector2::ElementWiseProduct(const Vector2& _other) const
{
	return Vector2(X * _other.X, Y * _other.Y);
}
Vector2 Vector2::ProductScalar(float _scalar) const
{
	return Vector2(X * _scalar, Y * _scalar);
}
bool Vector2::NearlyEqual(const Vector2& _other, float _epsilon) const
{
	return std::abs(X - _other.X) < _epsilon && std::abs(Y - _other.Y) < _epsilon;
}
Vector2 Vector2::Mid(const Vector2& _other) const
{
	return AddVector2(_other).ProductScalar(1.f / 2.f);
}
float Vector2::Distance(const Vector2& _other) const
{
	return sqrt(DistancePow(_other));
}
float Vector2::DistancePow(const Vector2& _other) const
{
	return (_other.X - X) * (_other.X - X) + (_other.Y - Y) * (_other.Y - Y);
}
float Vector2::Norm() const
{
	return sqrt(X * X + Y * Y);
}
float Vector2::SquareNorm() const
{
	return X * X + Y * Y;
}
Vector2 Vector2::Normalized() const
{
	Vector2 vec;
	float magnitude = Norm();

	if (magnitude == 0)
	{
		return Vector2(0, 0);
	}

	vec.X = X / magnitude;
	vec.Y = Y / magnitude;
	return vec;
}
float Vector2::Perspective(float _t) const
{
	if (_t <= 0) return 0;

	return SquareNorm() / _t;
}
float Vector2::DotProductVector2(const Vector2& _other) const
{
	return X * _other.X + Y * _other.Y;
}
float Vector2::CrossProductVector2(const Vector2& _other) const
{
	return X * _other.Y - Y * _other.X;
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

	return Vector2(X * cosA - Y * sinA, X * sinA + Y * cosA);
}
void Vector2::Print() const
{
	std::cout << "x : " << X << " y : " << Y << "\n";
}

Vector2 Vector2::operator+(const Vector2& _vec) const
{
	return AddVector2(_vec);
}
Vector2 Vector2::operator-(const Vector2& _vec) const
{
	return AddVector2(_vec.Opposed());
}
Vector2 Vector2::operator*(const Vector2& _vec) const
{
	return ElementWiseProduct(_vec);
}
Vector2 Vector2::operator/(const Vector2& _vec) const
{
	assert(_vec.X != 0 && _vec.Y != 0 && "Division by zero");
	return Vector2(X / _vec.X, Y / _vec.Y);
}
Vector2& Vector2::operator+=(const Vector2& _vec)
{
	X += _vec.X;
	Y += _vec.Y;
	return *this;
}
Vector2& Vector2::operator-=(const Vector2& _vec)
{
	X -= _vec.X;
	Y -= _vec.Y;
	return *this;
}
Vector2& Vector2::operator*=(const Vector2& _vec)
{
	X *= _vec.X;
	Y *= _vec.Y;
	return *this;
}
Vector2& Vector2::operator/=(const Vector2& _vec)
{
	X /= _vec.X;
	Y /= _vec.Y;
	return *this;
}
Vector2 Vector2::operator+(float _scalar) const
{
	return AddScalar(_scalar);
}
Vector2 Vector2::operator-(float _scalar) const
{
	return AddScalar(-_scalar);
}
Vector2 Vector2::operator-() const
{
	return Vector2{ -X, -Y };
}
Vector2 Vector2::operator*(float _scalar) const
{
	return ProductScalar(_scalar);
}
Vector2 Vector2::operator/(float _scalar) const
{
	assert(_scalar != 0.0f && "Division by zero");
	float invScalar = (_scalar != 0.0f) ? 1.0f / _scalar : 0.0f;
	return ProductScalar(invScalar);
}
Vector2& Vector2::operator*=(float _scalar)
{
	X *= _scalar;
	Y *= _scalar;
	return *this;
}
Vector2& Vector2::operator/=(float _scalar)
{
	assert(_scalar != 0.0f && "Division by zero");
	float invScalar = (_scalar != 0.0f) ? 1.0f / _scalar : 0.0f;
	X *= invScalar;
	Y *= invScalar;
	return *this;
}
Vector2& Vector2::operator+=(float _scalar)
{
	X += _scalar;
	Y += _scalar;
	return *this;
}
Vector2& Vector2::operator-=(float _scalar)
{
	return *this += -_scalar;
}
float& Vector2::operator[](int _index)
{
	assert(_index >= 0 && _index < 2 && "Index out of range");
	return (_index == 0) ? X : Y;
}
const float& Vector2::operator[](int _index) const
{
	assert(_index >= 0 && _index < 2 && "Index out of range");
	return (_index == 0) ? X : Y;
}
bool MirageEngine::Core::Math::Vector2::operator==(const Vector2& _other) const
{
	return X == _other.X && Y == _other.Y;
}
bool MirageEngine::Core::Math::Vector2::operator!=(const Vector2& _other) const
{
	return !(*this == _other);
}
Vector2 MirageEngine::Core::Math::operator*(float _scalar, const Vector2& _vec)
{
	return { _vec.X * _scalar, _vec.Y * _scalar };
}
std::ostream& MirageEngine::Core::Math::operator<<(std::ostream& _os, const Vector2& _vec)
{
	return _os << "(" << _vec.X << ", " << _vec.Y << ")";
}