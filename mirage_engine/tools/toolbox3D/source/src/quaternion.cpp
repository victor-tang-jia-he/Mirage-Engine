#include "Include/Matrix4.h"
#include "Quaternion.h"
#include <iostream>

using namespace MirageEngine::Core::Math;

Quaternion::Quaternion(float _num) noexcept
	:W(_num),
	X(_num),
	Y(_num),
	Z(_num)
{
}

Quaternion::Quaternion(float _w, float _x, float _y, float _z) noexcept
	:W(_w),
	X(_x),
	Y(_y),
	Z(_z)
{
}

Quaternion::Quaternion(const Quaternion& _q) noexcept
	:W(_q.W),
	X(_q.X),
	Y(_q.Y),
	Z(_q.Z)
{
}

Quaternion::Quaternion(Quaternion&& _q) noexcept
	:W(_q.W),
	X(_q.X),
	Y(_q.Y),
	Z(_q.Z)
{
	_q.W = _q.X = _q.Y = _q.Z = 0.f;
}

Quaternion::Quaternion(Vector3& _axis, float _angle)
{
	if (_axis.IsNull())
	{
		W = NAN;
		X = NAN;
		Y = NAN;
		Z = NAN;
	}

	else
	{
		_axis = _axis.Normalized();

		float halfAngle = _angle * 0.5f;
		float sinHalfAngle = std::sin(halfAngle);

		W = std::cos(halfAngle);
		X = _axis[0] * sinHalfAngle;
		Y = _axis[1] * sinHalfAngle;
		Z = _axis[2] * sinHalfAngle;
	}
}

Quaternion::Quaternion(float _angleX, float _angleY, float _angleZ)
{
	Vector3 vec(_angleX, _angleY, _angleZ);
	vec *= 0.5f;
	Vector3 c(cosf(vec.X), cosf(vec.Y), cosf(vec.Z));
	Vector3 s(sinf(vec.X), sinf(vec.Y), sinf(vec.Z));

	this->W = c.X * c.Y * c.Z + s.X * s.Y * s.Z;
	this->X = s.X * c.Y * c.Z - c.X * s.Y * s.Z;
	this->Y = c.X * s.Y * c.Z + s.X * c.Y * s.Z;
	this->Z = c.X * c.Y * s.Z - s.X * s.Y * c.Z;
}

Quaternion::Quaternion(Vector3& _angles)
{
	Vector3 vec = _angles * 0.5f;
	Vector3 c(cosf(vec.X), cosf(vec.Y), cosf(vec.Z));
	Vector3 s(sinf(vec.X), sinf(vec.Y), sinf(vec.Z));

	this->W = c.X * c.Y * c.Z + s.X * s.Y * s.Z;
	this->X = s.X * c.Y * c.Z - c.X * s.Y * s.Z;
	this->Y = c.X * s.Y * c.Z + s.X * c.Y * s.Z;
	this->Z = c.X * c.Y * s.Z - s.X * s.Y * c.Z;
}

Quaternion::Quaternion(float _x1, float _x2, float _x3, float _x4, float _y1, float _y2, float _y3, float _y4, float _z1, float z2, float _z3, float _z4, float _w1, float _w2, float _w3, float _w4)
{
	float trace = _x1 + _y2 + _z3;

	if (trace > 0)
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		W = 0.25f / s;
		X = (z2 - _y3) * s;
		Y = (_x3 - _z1) * s;
		Z = (_y1 - _x2) * s;
	}
	else
	{
		if (_x1 > _y2 && _x1 > _z3)
		{
			float s = 2.0f * sqrtf(1.0f + _x1 - _y2 - _z3);
			W = (z2 - _y3) / s;
			X = 0.25f * s;
			Y = (_x2 + _y1) / s;
			Z = (_x3 + _z1) / s;
		}

		else if (_y2 > _z3)
		{
			float s = 2.0f * sqrtf(1.0f + _y2 - _x1 - _z3);
			W = (_x3 - _z1) / s;
			X = (_x2 + _y1) / s;
			Y = 0.25f * s;
			Z = (_y3 + z2) / s;
		}

		else
		{
			float s = 2.0f * sqrtf(1.0f + _z3 - _x1 - _y2);
			W = (_y1 - _x2) / s;
			X = (_x3 + _z1) / s;
			Y = (_y3 + z2) / s;
			Z = 0.25f * s;
		}
	}
}

Quaternion::Quaternion(Matrix4 _m)
{
	W = sqrtf(1 + _m[0][0] + _m[1][1] + _m[2][2]) / 2.f;

	float a4 = 1 / (4 * W);
	X = (_m[2][1] - _m[1][2]) * a4;
	Y = (_m[0][2] - _m[2][0]) * a4;
	Z = (_m[1][0] - _m[0][1]) * a4;
}

float Quaternion::GetReal() const
{
	return W;
}

Vector3 Quaternion::GetImaginary() const
{
	return Vector3(X, Y, Z);
}

Vector4 Quaternion::GetRotationAndAxis() const
{
	float _angle = 2 * std::acos(W);
	float sin_half_angle = std::sqrt(1 - W * W);

	if (sin_half_angle < EPSILON)
	{
		return Vector4(0.f, 0.f, 0.f, 0.f);
	}

	else
	{
		return Vector4(X / sin_half_angle, Y / sin_half_angle, Z / sin_half_angle, _angle);
	}
}

Vector3 Quaternion::GetRotationAxis() const
{
	float sin_half_angle = std::sqrt(1 - W * W);

	if (sin_half_angle < EPSILON)
	{
		return Vector3(0.f, 0.f, 0.f);
	}

	else
	{
		return Vector3(X / sin_half_angle, Y / sin_half_angle, Z / sin_half_angle);
	}
}

float Quaternion::GetRotation() const
{
	return 2 * std::acos(W);
}

float* Quaternion::GetData()
{
	return &W;
}


bool Quaternion::IsNull() const
{
	return fabs(W) < EPSILON && fabs(X) < EPSILON && fabs(Y) < EPSILON && fabs(Z) < EPSILON;
}

bool Quaternion::IsUnit() const
{
	return std::fabs(Norm() - 1.0f) < EPSILON;
}

bool Quaternion::IsPureReal() const
{
	return fabs(W) >= EPSILON && fabs(X) < EPSILON && fabs(Y) < EPSILON && fabs(Z) < EPSILON;
}

bool Quaternion::IsPureImaginary() const
{
	return fabs(W) < EPSILON && (fabs(X) >= EPSILON || fabs(Y) >= EPSILON || fabs(Z) >= EPSILON);
}

Quaternion Quaternion::Opposite() const
{
	return Quaternion(-W, -X, -Y, -Z);
}

Quaternion Quaternion::Conjugation() const
{
	return Quaternion(W, -X, -Y, -Z);
}

Quaternion Quaternion::Inverse() const
{
	float squaredNorm = SquaredNorm();

	if (squaredNorm < EPSILON)
	{
		return Quaternion(NAN);
	}
	return Quaternion(W / squaredNorm, -X / squaredNorm, -Y / squaredNorm, -Z / squaredNorm);
}


Quaternion Quaternion::Normalize() const
{
	float myNorm = Norm();

	if (myNorm >= EPSILON)
	{
		float _a = W / myNorm;
		float _b = X / myNorm;
		float _c = Y / myNorm;
		float _d = Z / myNorm;

		return Quaternion(_a, _b, _c, _d);
	}

	return Quaternion(NAN);
}


float Quaternion::SquaredNorm() const
{
	return W * W + X * X + Y * Y + Z * Z;
}

float Quaternion::Norm() const
{
	return sqrtf(W * W + X * X + Y * Y + Z * Z);
}

float Quaternion::DotProduct(const Quaternion& _q) const
{
	return W * _q.W + X * _q.X + Y * _q.Y + Z * _q.Z;
}


Vector3 Quaternion::Rotate(const Vector3& p, const Quaternion& _q)
{
	return (_q * Quaternion(0.f, p[0], p[1], p[2]) * _q.Conjugation()).GetImaginary();
}

Vector3 Quaternion::RotateInverse(const Vector3& p, const Quaternion& _q)
{
	return (_q.Conjugation() * Quaternion(0.f, p[0], p[1], p[2]) * _q).GetImaginary();
}

Quaternion Quaternion::Slerp(const Quaternion& _q1, const Quaternion& _q2, const float _t)
{
	float dot = _q1.W * _q2.W + _q1.X * _q2.X + _q1.Y * _q2.Y + _q1.Z * _q2.Z;
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
		factor1 * _q1.W + factor2 * q2Copy.W,
		factor1 * _q1.X + factor2 * q2Copy.X,
		factor1 * _q1.Y + factor2 * q2Copy.Y,
		factor1 * _q1.Z + factor2 * q2Copy.Z);
}

Quaternion Quaternion::Nlerp(const Quaternion& _q1, const Quaternion& _q2, const float _t)
{
	Quaternion result(
		(1 - _t) * _q1.W + _t * _q2.W,
		(1 - _t) * _q1.X + _t * _q2.X,
		(1 - _t) * _q1.Y + _t * _q2.Y,
		(1 - _t) * _q1.Z + _t * _q2.Z);

	return result.Normalize();
}


Matrix4 Quaternion::ToMatrix() const
{
	float aa = W * W, bb = X * X, cc = Y * Y, dd = Z * Z;

	return Matrix4(
		aa + bb - cc - dd, 2.0f * (X * Y - W * Z), 2.0f * (X * Z + W * Y), 0.f,
		2.0f * (X * Y + W * Z), aa - bb + cc - dd, 2.0f * (Y * Z - W * X), 0.f,
		2.0f * (X * Z - W * Y), 2.0f * (Y * Z + W * X), aa - bb - cc + dd, 0.f,
		0.f, 0.f, 0.f, 1.f);
}

Vector3 Quaternion::ToEulerAngle() const
{
	Quaternion q = this->Normalize();

	float aa = q.W * q.W, bb = q.X * q.X, cc = q.Y * q.Y, dd = q.Z * q.Z;

	float m00 = aa + bb - cc - dd;
	float m01 = 2.f * (q.X * q.Y - q.W * q.Z);
	float m02 = 2.f * (q.X * q.Z + q.W * q.Y);
	float m11 = aa - bb + cc - dd;
	float m12 = 2.f * (q.Y * q.Z - q.W * q.X);
	float m21 = 2.f * (q.Y * q.Z + q.W * q.X);
	float m22 = aa - bb - cc + dd;

	if (fabs(m02) < 1.0f)
		return Vector3(atan2f(-m12, m22), asinf(m02), atan2f(-m01, m00));

	// Gimbal lock (Axe W)
	else
		return Vector3(atan2f(m21, m11), (m02 > 0.0f) ? PI / 2.f : -PI / 2, 0.0f);
}

void Quaternion::Print() const
{
	printf("X = %g\nY = %g\nZ = %g\nW = %g\n", W, X, Y, Z);
}


Quaternion Quaternion::operator+(const Quaternion& _q) const
{
	return Quaternion(W + _q[0], X + _q[1], Y + _q[2], Z + _q[3]);
}

Quaternion Quaternion::operator+(float _scale) const
{
	return Quaternion(W + _scale, X + _scale, Y + _scale, Z + _scale);
}

Quaternion Quaternion::operator-(const Quaternion& _q) const
{
	return Quaternion(W - _q[0], X - _q[1], Y - _q[2], Z - _q[3]);
}

Quaternion Quaternion::operator-(float _scale) const
{
	return Quaternion(W - _scale, X - _scale, Y - _scale, Z - _scale);
}

Quaternion Quaternion::operator*(const Quaternion& _q) const
{
	return Quaternion(
		W * _q.W - X * _q.X - Y * _q.Y - Z * _q.Z,
		W * _q.X + X * _q.W + Y * _q.Z - Z * _q.Y, 
		W * _q.Y + Y * _q.W + Z * _q.X - X * _q.Z, 
		W * _q.Z + Z * _q.W + X * _q.Y - Y * _q.X  
	);
}

Quaternion Quaternion::operator*(float _scale) const
{
	return Quaternion(W * _scale, X * _scale, Y * _scale, Z * _scale);
}

Quaternion Quaternion::operator/(const Quaternion& _q) const
{
	return Quaternion(*this * _q.Inverse());
}


Quaternion Quaternion::operator/(float _scale) const
{
	if (fabs(_scale) >= EPSILON)
		return Quaternion(W / _scale, X / _scale, Y / _scale, Z / _scale);

	return Quaternion(NAN);
}

void Quaternion::operator+=(const Quaternion& _q)
{
	W += _q[0];
	X += _q[1];
	Y += _q[2];
	Z += _q[3];
}

void Quaternion::operator+=(float _scale)
{
	W += _scale;
	X += _scale;
	Y += _scale;
	Z += _scale;
}

void Quaternion::operator-=(const Quaternion& _q)
{
	W -= _q[0];
	X -= _q[1];
	Y -= _q[2];
	Z -= _q[3];
}

void Quaternion::operator-=(float _scale)
{
	W -= _scale;
	X -= _scale;
	Y -= _scale;
	Z -= _scale;
}

void Quaternion::operator*=(const Quaternion& _q)
{
	float newA = W * _q[0] - X * _q[1] - Y * _q[2] - Z * _q[3];
	float newB = X * _q[0] + W * _q[1] - Z * _q[2] + Y * _q[3];
	float newC = Y * _q[0] + Z * _q[1] + W * _q[2] - X * _q[3];
	float newD = Z * _q[0] - Y * _q[1] + X * _q[2] + W * _q[3];

	W = newA;
	X = newB;
	Y = newC;
	Z = newD;
}

void Quaternion::operator*=(float _scale)
{
	W *= _scale;
	X *= _scale;
	Y *= _scale;
	Z *= _scale;
}

void Quaternion::operator/=(const Quaternion& _q)
{
	*this *= _q.Inverse();
}

void Quaternion::operator/=(float _scale)
{
	if (fabs(_scale) >= EPSILON)
	{
		W /= _scale;
		X /= _scale;
		Y /= _scale;
		Z /= _scale;
		return;
	}

	W = NAN;
	X = NAN;
	Y = NAN;
	Z = NAN;
}

bool Quaternion::operator==(const Quaternion& _q) const
{
	return fabs(W - _q[0]) < EPSILON && fabs(X - _q[1]) < EPSILON && fabs(Y - _q[2]) < EPSILON && fabs(Z - _q[3]) < EPSILON;
}

bool Quaternion::operator!=(const Quaternion& _q) const
{
	return fabs(W - _q[0]) >= EPSILON && fabs(X - _q[1]) >= EPSILON && fabs(Y - _q[2]) >= EPSILON && fabs(Z - _q[3]) >= EPSILON;
}

Quaternion& Quaternion::operator=(const Quaternion& _q)
{
	if (this != &_q)
	{
		W = _q.W;
		X = _q.X;
		Y = _q.Y;
		Z = _q.Z;
	}

	return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& _q) noexcept
{
	if (this != &_q)
	{
		W = _q.W;
		X = _q.X;
		Y = _q.Y;
		Z = _q.Z;
		_q.W = _q.X = _q.Y = _q.Z = 0.f;
	}

	return *this;
}


float Quaternion::operator[](int _index) const
{
	if (_index > -1 && _index < 4)
		return *(&W + _index);

	else
		return NAN;
}

std::ostream& MirageEngine::Core::Math::operator<<(std::ostream& _os, const Quaternion& _q)
{
	return _os << "a = " << _q[0] << ", b = " << _q[1] << ", c = " << _q[2] << ", d = " << _q[3] << std::endl;
}
