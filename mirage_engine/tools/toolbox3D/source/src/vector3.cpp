#include "vector3.h"
#include "matrix3.h"
#include <iostream>
#include <array>
#include <assert.h>

using namespace MirageEngine::Core::Math;
Vector3 Vector3::Opposed() const
{
	return Vector3(-X, -Y, -Z);
}
Vector3 Vector3::AddVector3(const Vector3& _other) const
{
	return Vector3(X + _other.X, Y + _other.Y, Z + _other.Z);
}
Vector3 Vector3::AddScalar(float _scalar) const
{
	return Vector3(X + _scalar, Y + _scalar, Z + _scalar);
}
Vector3 Vector3::ElementWiseProduct(const Vector3& _other) const
{
	return Vector3(X * _other.X, Y * _other.Y, Z * _other.Z);
}
Vector3 Vector3::ProductScalar(float _scalar) const
{
	return Vector3(X * _scalar, Y * _scalar, Z * _scalar);
}
bool Vector3::NearlyEqual(const Vector3& _other, float _epsilon) const
{
	return std::abs(X - _other.X) < _epsilon && std::abs(Y - _other.Y) < _epsilon && std::abs(Z - _other.Z) < _epsilon;
}
bool Vector3::IsNull() const
{
	return X == 0 && Y == 0 && Z == 0;
}
Vector3 Vector3::Mid(const Vector3& _other) const
{
	return AddVector3(_other).ProductScalar(1.f / 2.f);
}
float Vector3::Distance(const Vector3& _other) const
{
	return sqrt(DistancePow(_other));
}
float Vector3::DistancePow(const Vector3& _other) const
{
	float dx = _other.X - X;
	float dy = _other.Y - Y;
	float dz = _other.Z - Z;
	return dx * dx + dy * dy + dz * dz;
}
float Vector3::Norm() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}
float Vector3::SquareNorm() const
{
	return X * X + Y * Y + Z * Z;
}
Vector3 Vector3::Normalized() const
{
	Vector3 _vec;
	float magnitude = Norm();

	if (magnitude == 0)
	{
		return Vector3(0, 0, 0);
	}

	_vec.X = X / magnitude;
	_vec.Y = Y / magnitude;
	_vec.Z = Z / magnitude;
	return _vec;
}
float Vector3::Perspective(float _t) const
{
	if (_t <= 0) return 0;

	return SquareNorm() / _t;
}
Vector3 Vector3::Reflect(Vector3 I, Vector3 N)
{
	return I - N.Normalized() * (2.0f * I.DotProductVector3(N.Normalized()));
}
float Vector3::DotProductVector3(const Vector3& _other) const
{
	return X * _other.X + Y * _other.Y + Z * _other.Z;
}
Vector3 Vector3::CrossProductVector3(const Vector3& _other) const
{
	return Vector3(
		Y * _other.Z - Z * _other.Y,
		Z * _other.X - X * _other.Z,
		X * _other.Y - Y * _other.X
	);
}
float Vector3::Angle(const Vector3& v2) const
{
	float denom = Norm() * v2.Norm();
	if (denom == 0.0f) return 0.0f;

	return acosf(DotProductVector3(v2) / denom);
}
Vector3 Vector3::Rotate(float angleX, float angleY, float angleZ) const {
	float cosX = cosf(angleX), sinX = sinf(angleX);
	float cosY = cosf(angleY), sinY = sinf(angleY);
	float cosZ = cosf(angleZ), sinZ = sinf(angleZ);

	float x0 = X, y0 = Y, z0 = Z;

	float y1 = y0 * cosX - z0 * sinX;
	float z1 = y0 * sinX + z0 * cosX;

	float x2 = x0 * cosY + z1 * sinY;
	float z2 = -x0 * sinY + z1 * cosY;
	return Vector3(
		x2 * cosZ - y1 * sinZ,
		x2 * sinZ + y1 * cosZ,
		z2
	);
}
void Vector3::Print() const
{
	std::cout << "x : " << X << " y : " << Y << " z : " << Z << "\n";
}
std::array<float, 3> Vector3::AsList() const
{
	return { X, Y, Z };
}
Vector3 Vector3::operator+(const Vector3& _vec) const
{
	return AddVector3(_vec);
}
Vector3 Vector3::operator-(const Vector3& _vec) const
{
	return AddVector3(_vec.Opposed());
}
Vector3 Vector3::operator*(const Vector3& _vec) const
{
	return ElementWiseProduct(_vec);
}
Vector3 Vector3::operator/(const Vector3& _vec) const
{
	assert(_vec.X != 0 && _vec.Y != 0 && _vec.Z != 0 && "Division by zero");
	return Vector3(X / _vec.X, Y / _vec.Y, Z / _vec.Z);
}
Vector3& Vector3::operator+=(const Vector3& _vec)
{
	X += _vec.X;
	Y += _vec.Y;
	Z += _vec.Z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& _vec)
{
	X -= _vec.X;
	Y -= _vec.Y;
	Z -= _vec.Z;
	return *this;
}
Vector3& Vector3::operator*=(const Vector3& _vec)
{
	X *= _vec.X;
	Y *= _vec.Y;
	Z *= _vec.Z;
	return *this;
}
Vector3& Vector3::operator/=(const Vector3& _vec)
{
	assert(_vec.X != 0 && _vec.Y != 0 && _vec.Z != 0 && "Division by zero");
	X /= _vec.X;
	Y /= _vec.Y;
	Z /= _vec.Z;
	return *this;
}
Vector3 Vector3::operator+(float _scalar) const
{
	return AddScalar(_scalar);
}
Vector3 Vector3::operator-(float _scalar) const
{
	return AddScalar(-_scalar);
}
Vector3 Vector3::operator*(float _scalar) const
{
	return ProductScalar(_scalar);
}
Vector3 Vector3::operator/(float _scalar) const
{
	assert(_scalar != 0.0f && "Division by zero");

	float invScalar = (_scalar != 0.0f) ? 1.0f / _scalar : 0.0f;
	return ProductScalar(invScalar);
}
Vector3& Vector3::operator*=(float _scalar)
{
	X *= _scalar;
	Y *= _scalar;
	Z *= _scalar;
	return *this;
}
Vector3& Vector3::operator/=(float _scalar)
{
	assert(_scalar != 0.0f && "Division by zero");
	float invScalar = (_scalar != 0.0f) ? 1.0f / _scalar : 0.0f;
	X *= invScalar;
	Y *= invScalar;
	Z *= invScalar;
	return *this;
}
Vector3& Vector3::operator+=(float _scalar)
{
	X += _scalar;
	Y += _scalar;
	Z += _scalar;
	return *this;
}
Vector3& Vector3::operator-=(float _scalar)
{
	return *this += -_scalar;
}
Vector3 Vector3::operator-() const
{
	return Vector3{ -X, -Y, -Z };
}
float& Vector3::operator[](int _index)
{
	assert(_index >= 0 && _index < 3 && "Index out of range");
	return (_index == 0) ? X : (_index == 1) ? Y : Z;
}
const float& Vector3::operator[](int _index) const
{
	assert(_index >= 0 && _index < 3 && "Index out of range");
	return (_index == 0) ? X : (_index == 1) ? Y : Z;
}
Vector3::operator Vector2() const
{
	return Vector2(X, Y);
}
bool MirageEngine::Core::Math::Vector3::operator==(const Vector3& _other) const
{
	return X == _other.X && Y == _other.Y && Z == _other.Z;
}
bool MirageEngine::Core::Math::Vector3::operator!=(const Vector3& _other) const
{
	return !(*this == _other);
}
Vector3 MirageEngine::Core::Math::operator*(float _scalar, const Vector3& _vec)
{
	return { _vec.X * _scalar, _vec.Y * _scalar, _vec.Z * _scalar };
}
std::ostream& MirageEngine::Core::Math::operator<<(std::ostream& _oS, const Vector3& _vec)
{
	return _os << "(" << _vec.X << ", " << _vec.Y << ", " << _vec.Y << ")";
}
