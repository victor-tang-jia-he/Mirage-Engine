#include "Include/Matrix4.h"
#include "Quaternion.h"
#include <iostream>

using namespace MirageEngine::Core::Math;

Quaternion::Quaternion(float _num) noexcept
	:X(_num),
	Y(_num),
	Z(_num),
	W(_num)
{
}

Quaternion::Quaternion(float _a, float _b, float _c, float _d) noexcept
	:X(_a),
	Y(_b),
	Z(_c),
	W(_d)
{
}

Quaternion::Quaternion(const Quaternion& _q) noexcept
	:X(_q.X),
	Y(_q.Y),
	Z(_q.Z),
	W(_q.W)
{
}

Quaternion::Quaternion(Quaternion&& _q) noexcept
	:X(_q.X),
	Y(_q.Y),
	Z(_q.Z),
	W(_q.W)
{
	_q.X = _q.Y = _q.Z = _q.W = 0.f;
}

Quaternion::Quaternion(Vector3& _axis, float _angle)
{
	if (_axis.IsNull())
	{
		X = NAN;
		Y = NAN;
		Z = NAN;
		W = NAN;
	}

	else
	{
		_axis = _axis.Normalized();

		float halfAngle = _angle * 0.5f;
		float sinHalfAngle = std::sin(halfAngle);

		X = std::cos(halfAngle);
		Y = _axis[0] * sinHalfAngle;
		Z = _axis[1] * sinHalfAngle;
		W = _axis[2] * sinHalfAngle;
	}
}

Quaternion::Quaternion(float _angleX, float _angleY, float _angleZ)
{
	float halfX = _angleX * 0.5f;
	float halfY = _angleY * 0.5f;
	float halfZ = _angleZ * 0.5f;

	float cosX = cosf(halfX);
	float sinX = sinf(halfX);
	float cosY = cosf(halfY);
	float sinY = sinf(halfY);
	float cosZ = cosf(halfZ);
	float sinZ = sinf(halfZ);

	X = cosX * cosY * cosZ - sinX * sinY * sinZ;
	Y = sinX * cosY * cosZ + cosX * sinY * sinZ;
	Z = cosX * sinY * cosZ - sinX * cosY * sinZ;
	W = cosX * cosY * sinZ + sinX * sinY * cosZ;
}

Quaternion::Quaternion(Vector3& _angles)
{
	float halfX = _angles[0] * 0.5f;
	float halfY = _angles[1] * 0.5f;
	float halfZ = _angles[2] * 0.5f;

	float cosX = cosf(halfX);
	float sinX = sinf(halfX);
	float cosY = cosf(halfY);
	float sinY = sinf(halfY);
	float cosZ = cosf(halfZ);
	float sinZ = sinf(halfZ);

	X = cosX * cosY * cosZ - sinX * sinY * sinZ;
	Y = sinX * cosY * cosZ + cosX * sinY * sinZ;
	Z = cosX * sinY * cosZ - sinX * cosY * sinZ;
	W = cosX * cosY * sinZ + sinX * sinY * cosZ;
}

Quaternion::Quaternion(float _x1, float _x2, float _x3, float _x4, float _y1, float _y2, float _y3, float _y4, float _z1, float z2, float _z3, float _z4, float _w1, float _w2, float _w3, float _w4)
{
	float trace = _x1 + _y2 + _z3;

	if (trace > 0)
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		X = 0.25f / s;
		Y = (z2 - _y3) * s;
		Z = (_x3 - _z1) * s;
		W = (_y1 - _x2) * s;
	}
	else
	{
		if (_x1 > _y2 && _x1 > _z3)
		{
			float s = 2.0f * sqrtf(1.0f + _x1 - _y2 - _z3);
			X = (z2 - _y3) / s;
			Y = 0.25f * s;
			Z = (_x2 + _y1) / s;
			W = (_x3 + _z1) / s;
		}

		else if (_y2 > _z3)
		{
			float s = 2.0f * sqrtf(1.0f + _y2 - _x1 - _z3);
			X = (_x3 - _z1) / s;
			Y = (_x2 + _y1) / s;
			Z = 0.25f * s;
			W = (_y3 + z2) / s;
		}

		else
		{
			float s = 2.0f * sqrtf(1.0f + _z3 - _x1 - _y2);
			X = (_y1 - _x2) / s;
			Y = (_x3 + _z1) / s;
			Z = (_y3 + z2) / s;
			W = 0.25f * s;
		}
	}
}

Quaternion::Quaternion(Matrix4 _m)
{
	X = sqrtf(1 + _m[0][0] + _m[1][1] + _m[2][2]) / 2.f;

	float a4 = 1 / (4 * X);
	Y = (_m[2][1] - _m[1][2]) * a4;
	Z = (_m[0][2] - _m[2][0]) * a4;
	W = (_m[1][0] - _m[0][1]) * a4;
}

float Quaternion::GetReal() const
{
	return X;
}

Vector3 Quaternion::GetImaginary() const
{
	return Vector3(Y, Z, W);
}

Vector4 Quaternion::GetRotationAndAxis() const
{
	float _angle = 2 * std::acos(X);
	float sin_half_angle = std::sqrt(1 - X * X);

	if (sin_half_angle < EPSILON)
	{
		return Vector4(0.f, 0.f, 0.f, 0.f);
	}

	else
	{
		return Vector4(Y / sin_half_angle, Z / sin_half_angle, W / sin_half_angle, _angle);
	}
}

Vector3 Quaternion::GetRotationAxis() const
{
	float sin_half_angle = std::sqrt(1 - X * X);

	if (sin_half_angle < EPSILON)
	{
		return Vector3(0.f, 0.f, 0.f);
	}

	else
	{
		return Vector3(Y / sin_half_angle, Z / sin_half_angle, W / sin_half_angle);
	}
}

float Quaternion::GetRotation() const
{
	return 2 * std::acos(X);
}

float* Quaternion::GetData()
{
	return &X;
}


bool Quaternion::IsNull() const
{
	return fabs(X) < EPSILON && fabs(Y) < EPSILON && fabs(Z) < EPSILON && fabs(W) < EPSILON;
}

bool Quaternion::IsUnit() const
{
	return std::fabs(Norm() - 1.0f) < EPSILON;
}

bool Quaternion::IsPureReal() const
{
	return fabs(X) >= EPSILON && fabs(Y) < EPSILON && fabs(Z) < EPSILON && fabs(W) < EPSILON;
}

bool Quaternion::IsPureImaginary() const
{
	return fabs(X) < EPSILON && (fabs(Y) >= EPSILON || fabs(Z) >= EPSILON || fabs(W) >= EPSILON);
}

Quaternion Quaternion::Opposite() const
{
	return Quaternion(-X, -Y, -Z, -W);
}

Quaternion Quaternion::Conjugation() const
{
	return Quaternion(X, -Y, -Z, -W);
}

Quaternion Quaternion::Inverse() const
{
	float squaredNorm = SquaredNorm();

	if (squaredNorm < EPSILON)
	{
		return Quaternion(NAN);
	}
	return Quaternion(X / squaredNorm, -Y / squaredNorm, -Z / squaredNorm, -W / squaredNorm);
}


Quaternion Quaternion::Normalize() const
{
	float myNorm = Norm();

	if (myNorm >= EPSILON)
	{
		float _x = X / myNorm;
		float _y = Y / myNorm;
		float _z = Z / myNorm;
		float _w = W / myNorm;

		return Quaternion(_x, _y, _z, _w);
	}

	return Quaternion(NAN);
}


float Quaternion::SquaredNorm() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

float Quaternion::Norm() const
{
	return sqrtf(X * X + Y * Y + Z * Z + W * W);
}

float Quaternion::DotProduct(const Quaternion& _q) const
{
	return X * _q.X + Y * _q.Y + Z * _q.Z + W * _q.W;
}


Vector3 Quaternion::Rotate(const Vector3& p, const Quaternion& _q)
{
	return (_q * Quaternion(0.f, p[0], p[1], p[2]) * _q.Conjugation()).GetImaginary();
}

Vector3 Quaternion::RotateInverse(const Vector3& p, const Quaternion& _q)
{
	return (_q.Conjugation() * Quaternion(0.f, p[0], p[1], p[2]) * _q).GetImaginary();
}


Quaternion Quaternion::Lerp(const Quaternion& _q1, const Quaternion& _q2, const float _t)
{
	return Quaternion(
		(1 - _t) * _q1.X + _t * _q2.X,
		(1 - _t) * _q1.Y + _t * _q2.Y,
		(1 - _t) * _q1.Z + _t * _q2.Z,
		(1 - _t) * _q1.W + _t * _q2.W);
}

Quaternion Quaternion::Slerp(const Quaternion& _q1, const Quaternion& _q2, const float _t)
{
	float dot = _q1.X * _q2.X + _q1.Y * _q2.Y + _q1.Z * _q2.Z + _q1.W * _q2.W;
	Quaternion q2Copy = _q2;

	if (dot < 0.f)
	{
		dot = -dot;
		q2Copy = _q2.Opposite();
	}

	if (dot > 0.9995f)
		return _q1 * (1.0f - _t) + q2Copy * _t;

	float theta = acosf(dot);
	float sinTheta = sqrtf(1.0f - dot * dot);

	float factor1 = sinf((1.0f - _t) * theta) / sinTheta;
	float factor2 = sinf(_t * theta) / sinTheta;

	return Quaternion(
		factor1 * _q1.X + factor2 * q2Copy.X,
		factor1 * _q1.Y + factor2 * q2Copy.Y,
		factor1 * _q1.Z + factor2 * q2Copy.Z,
		factor1 * _q1.W + factor2 * q2Copy.W);
}

Quaternion Quaternion::Nlerp(const Quaternion& _q1, const Quaternion& _q2, const float _t)
{
	Quaternion result(
		(1 - _t) * _q1.X + _t * _q2.X,
		(1 - _t) * _q1.Y + _t * _q2.Y,
		(1 - _t) * _q1.Z + _t * _q2.Z,
		(1 - _t) * _q1.W + _t * _q2.W);

	return result.Normalize();
}


Matrix4 Quaternion::ToMatrix() const
{
	float aa = X * X, bb = Y * Y, cc = Z * Z, dd = W * W;

	return Matrix4(
		aa + bb - cc - dd, 2.0f * (Y * Z - X * W), 2.0f * (Y * W + X * Z), 0.f,
		2.0f * (Y * Z + X * W), aa - bb + cc - dd, 2.0f * (Z * W - X * Y), 0.f,
		2.0f * (Y * W - X * Z), 2.0f * (Z * W + X * Y), aa - bb - cc + dd, 0.f,
		0.f, 0.f, 0.f, 1.f);
}

Vector3 Quaternion::ToEulerAngle() const
{
	Quaternion q = this->Normalize();

	float aa = q.X * q.X, bb = q.Y * q.Y, cc = q.Z * q.Z, dd = q.W * q.W;

	float m00 = aa + bb - cc - dd;
	float m01 = 2.f * (q.Y * q.Z - q.X * q.W);
	float m02 = 2.f * (q.Y * q.W + q.X * q.Z);
	float m11 = aa - bb + cc - dd;
	float m12 = 2.f * (q.Z * q.W - q.X * q.Y);
	float m21 = 2.f * (q.Z * q.W + q.X * q.Y);
	float m22 = aa - bb - cc + dd;

	if (fabs(m02) < 1.0f)
		return Vector3(atan2f(-m12, m22), asinf(m02), atan2f(-m01, m00));

	// Gimbal lock (Axe X)
	else
		return Vector3(atan2f(m21, m11), (m02 > 0.0f) ? PI / 2.f : -PI / 2, 0.0f);
}

void Quaternion::Print() const
{
	printf("X = %g\nY = %g\nZ = %g\nW = %g\n", X, Y, Z, W);
}


Quaternion Quaternion::operator+(const Quaternion& _q) const
{
	return Quaternion(X + _q[0], Y + _q[1], Z + _q[2], W + _q[3]);
}

Quaternion Quaternion::operator+(float _scale) const
{
	return Quaternion(X + _scale, Y + _scale, Z + _scale, W + _scale);
}

Quaternion Quaternion::operator-(const Quaternion& _q) const
{
	return Quaternion(X - _q[0], Y - _q[1], Z - _q[2], W - _q[3]);
}

Quaternion Quaternion::operator-(float _scale) const
{
	return Quaternion(X + _scale, Y + _scale, Z + _scale, W + _scale);
}

Quaternion Quaternion::operator*(const Quaternion& _q) const
{
	return Quaternion(
		X * _q[0] - Y * _q[1] - Z * _q[2] - W * _q[3],
		Y * _q[0] + X * _q[1] - W * _q[2] + Z * _q[3],
		Z * _q[0] + W * _q[1] + X * _q[2] - Y * _q[3],
		W * _q[0] - Z * _q[1] + Y * _q[2] + X * _q[3]
	);
}

Quaternion Quaternion::operator*(float _scale) const
{
	return Quaternion(X * _scale, Y * _scale, Z * _scale, W * _scale);
}

Quaternion Quaternion::operator/(const Quaternion& _q) const
{
	return Quaternion(*this * _q.Inverse());
}


Quaternion Quaternion::operator/(float _scale) const
{
	if (fabs(_scale) >= EPSILON)
		return Quaternion(X / _scale, Y / _scale, Z / _scale, W / _scale);

	return Quaternion(NAN);
}

void Quaternion::operator+=(const Quaternion& _q)
{
	X += _q[0];
	Y += _q[1];
	Z += _q[2];
	W += _q[3];
}

void Quaternion::operator+=(float _scale)
{
	X += _scale;
	Y += _scale;
	Z += _scale;
	W += _scale;
}

void Quaternion::operator-=(const Quaternion& _q)
{
	X -= _q[0];
	Y -= _q[1];
	Z -= _q[2];
	W -= _q[3];
}

void Quaternion::operator-=(float _scale)
{
	X -= _scale;
	Y -= _scale;
	Z -= _scale;
	W -= _scale;
}

void Quaternion::operator*=(const Quaternion& _q)
{
	float newA = X * _q[0] - Y * _q[1] - Z * _q[2] - W * _q[3];
	float newB = Y * _q[0] + X * _q[1] - W * _q[2] + Z * _q[3];
	float newC = Z * _q[0] + W * _q[1] + X * _q[2] - Y * _q[3];
	float newD = W * _q[0] - Z * _q[1] + Y * _q[2] + X * _q[3];

	X = newA;
	Y = newB;
	Z = newC;
	W = newD;
}

void Quaternion::operator*=(float _scale)
{
	X *= _scale;
	Y *= _scale;
	Z *= _scale;
	W *= _scale;
}

void Quaternion::operator/=(const Quaternion& _q)
{
	*this *= _q.Inverse();
}

void Quaternion::operator/=(float _scale)
{
	if (fabs(_scale) >= EPSILON)
	{
		X /= _scale;
		Y /= _scale;
		Z /= _scale;
		W /= _scale;
		return;
	}

	X = NAN;
	Y = NAN;
	Z = NAN;
	W = NAN;
}

bool Quaternion::operator==(const Quaternion& _q) const
{
	return fabs(X - _q[0]) < EPSILON && fabs(Y - _q[1]) < EPSILON && fabs(Z - _q[2]) < EPSILON && fabs(W - _q[3]) < EPSILON;
}

bool Quaternion::operator!=(const Quaternion& _q) const
{
	return fabs(X - _q[0]) >= EPSILON && fabs(Y - _q[1]) >= EPSILON && fabs(Z - _q[2]) >= EPSILON && fabs(W - _q[3]) >= EPSILON;
}

Quaternion& Quaternion::operator=(const Quaternion& _q)
{
	if (this != &_q)
	{
		X = _q.X;
		Y = _q.Y;
		Z = _q.Z;
		W = _q.W;
	}

	return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& _q) noexcept
{
	if (this != &_q)
	{
		X = _q.X;
		Y = _q.Y;
		Z = _q.Z;
		W = _q.W;
		_q.X = _q.Y = _q.Z = _q.W = 0.f;
	}

	return *this;
}


float Quaternion::operator[](int _index) const
{
	if (_index > -1 && _index < 4)
		return *(&X + _index);

	else
		return NAN;
}

std::ostream& MirageEngine::Core::Math::operator<<(std::ostream& _os, const Quaternion& _q)
{
	return _os << "a = " << _q[0] << ", b = " << _q[1] << ", c = " << _q[2] << ", d = " << _q[3] << std::endl;
}
