#include "vector4.h"
#include <iostream>
#include <array>
#include <assert.h>
#include <cmath>

using namespace MirageEngine::Core::Math;

Vector4 Vector4::Opposed() const { return Vector4(-X, -Y, -Z, -W); }

Vector4 Vector4::AddVector4(const Vector4& _other) const { return Vector4(X + _other.X, Y + _other.Y, Z + _other.Z, W + _other.W); }

Vector4 Vector4::AddScalar(float _scalar) const { return Vector4(X + _scalar, Y + _scalar, Z + _scalar, W + _scalar); }

Vector4 Vector4::ElementWiseProduct(const Vector4& _other) const { return Vector4(X * _other.X, Y * _other.Y, Z * _other.Z, W * _other.W); }

Vector4 Vector4::ProductScalar(float _scalar) const { return Vector4(X * _scalar, Y * _scalar, Z * _scalar, W * _scalar); }

bool MirageEngine::Core::Math::Vector4::NearlyEqual(const Vector4& _other, float _epsilon) const
{
    return std::abs(X - _other.X) < _epsilon && std::abs(Y - _other.Y) < _epsilon && std::abs(Z - _other.Z) < _epsilon && std::abs(W - _other.W) < _epsilon;
}

Vector4 Vector4::Mid(const Vector4& _other) const { return AddVector4(_other).ProductScalar(1.f / 2.f); }

float Vector4::Distance(const Vector4& _other) const { return sqrt(DistancePow(_other)); }

float Vector4::DistancePow(const Vector4& _other) const
{
    return (_other.X - X) * (_other.X - X) + (_other.Y - Y) * (_other.Y - Y) + (_other.Z - Z) * (_other.Z - Z) + (_other.W - W) * (_other.W - W);
}

float Vector4::Norm() const { return sqrt(X * X + Y * Y + Z * Z + W * W); }

float Vector4::SquareNorm() const { return X * X + Y * Y + Z * Z + W * W; }

Vector4 Vector4::Normalized() const
{
    Vector4 vec;
    float magnitude = Norm();

    if (magnitude == 0)
    {
        return Vector4(0, 0, 0, 0);
    }

    vec.X = X / magnitude;
    vec.Y = Y / magnitude;
    vec.Z = Z / magnitude;
    vec.W = W / magnitude;
    return vec;
}

float Vector4::Perspective(float _t) const
{
    if (_t <= 0)
        return 0;

    return SquareNorm() / _t;
}

float Vector4::DotProductVector4(const Vector4& _other) const { return X * _other.X + Y * _other.Y + Z * _other.Z + W * _other.W; }

std::array<float, 4> Vector4::AsList() const { return { X, Y, Z, W }; }

void Vector4::Print() const { std::cout << "x : " << X << " y : " << Y << " z : " << Z << " w : " << W << "\n"; }

Vector4 Vector4::operator+(const Vector4& _vec) const { return AddVector4(_vec); }

Vector4 Vector4::operator-(const Vector4& _vec) const { return AddVector4(_vec.Opposed()); }

Vector4 Vector4::operator*(const Vector4& _vec) const { return ElementWiseProduct(_vec); }

Vector4 Vector4::operator/(const Vector4& _vec) const
{
    assert(_vec.X != 0 && _vec.Y != 0 && _vec.Z != 0 && _vec.W != 0 && "Division by zero");
    return Vector4(X / _vec.X, Y / _vec.Y, Z / _vec.Z, W / _vec.W);
}

Vector4& Vector4::operator+=(const Vector4& _vec)
{
    X += _vec.X;
    Y += _vec.Y;
    Z += _vec.Z;
    W += _vec.W;
    return *this;
}

Vector4& Vector4::operator-=(const Vector4& _vec)
{
    X -= _vec.X;
    Y -= _vec.Y;
    Z -= _vec.Z;
    W -= _vec.W;
    return *this;
}

Vector4& Vector4::operator*=(const Vector4& _vec)
{
    X *= _vec.X;
    Y *= _vec.Y;
    Z *= _vec.Z;
    W *= _vec.W;
    return *this;
}

Vector4& Vector4::operator/=(const Vector4& _vec)
{
    assert(_vec.X != 0 && _vec.Y != 0 && _vec.Z != 0 && _vec.W != 0 && "Division by zero");
    X /= _vec.X;
    Y /= _vec.Y;
    Z /= _vec.Z;
    W /= _vec.W;
    return *this;
}

Vector4 Vector4::operator+(float _scalar) const { return AddScalar(_scalar); }

Vector4 Vector4::operator-(float _scalar) const { return AddScalar(-_scalar); }

Vector4 Vector4::operator*(float _scalar) const { return ProductScalar(_scalar); }

Vector4 Vector4::operator/(float _scalar) const
{
    assert(_scalar != 0.0f && "Division by zero");
    float invScalar = (_scalar != 0.0f) ? 1.0f / _scalar : 0.0f;
    return ProductScalar(invScalar);
}

Vector4& Vector4::operator*=(float _scalar)
{
    X *= _scalar;
    Y *= _scalar;
    Z *= _scalar;
    W *= _scalar;
    return *this;
}

Vector4& Vector4::operator/=(float _scalar)
{
    assert(_scalar != 0.0f && "Division by zero");
    float invScalar = (_scalar != 0.0f) ? 1.0f / _scalar : 0.0f;
    X *= invScalar;
    Y *= invScalar;
    Z *= invScalar;
    W *= invScalar;
    return *this;
}

Vector4& Vector4::operator+=(float _scalar)
{
    X += _scalar;
    Y += _scalar;
    Z += _scalar;
    W += _scalar;
    return *this;
}

Vector4& Vector4::operator-=(float _scalar) { return *this += -_scalar; }

Vector4 Vector4::operator-() const { return Vector4{ -X, -Y, -Z, -W }; }

float& Vector4::operator[](int _index)
{
    assert(_index >= 0 && _index < 4 && "Index out of range");
    return (_index == 0) ? X : (_index == 1) ? Y : (_index == 2) ? Z : W;
}

const float& Vector4::operator[](int _index) const
{
    assert(_index >= 0 && _index < 4 && "Index out of range");
    return (_index == 0) ? X : (_index == 1) ? Y : (_index == 2) ? Z : W;
}

bool Vector4::operator==(const Vector4& _other) const { return X == _other.X && Y == _other.Y && Z == _other.Z && W == _other.W; }

bool Vector4::operator!=(const Vector4& _other) const { return !(*this == _other); }

Vector4 MirageEngine::Core::Math::operator*(float _scalar, const Vector4& _vec) { return { _vec.X * _scalar, _vec.Y * _scalar, _vec.Z * _scalar, _vec.W * _scalar }; }

Vector4::operator Vector3() const { return Vector3(X, Y, Z); }

Vector4::operator Vector2() const { return Vector2(X, Y); }

std::ostream& MirageEngine::Core::Math::operator<<(std::ostream& _os, const Vector4& _vec) { return _os << "(" << _vec.X << ", " << _vec.Y << ", " << _vec.Z << ", " << _vec.W << ")"; }
