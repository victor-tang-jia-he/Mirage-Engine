#include "Include/Matrix4.h"
#include "Quaternion.h"
#include <iostream>

using namespace MirageEngine::Core::Math;

Quaternion::Quaternion(float num) noexcept
	:x(num),
	y(num),
	z(num),
	w(num)
{
}

Quaternion::Quaternion(float _a, float _b, float _c, float _d) noexcept
	:x(_a),
	y(_b),
	z(_c),
	w(_d)
{
}

Quaternion::Quaternion(const Quaternion& q) noexcept
	:x(q.x),
	y(q.y),
	z(q.z),
	w(q.w)
{
}

Quaternion::Quaternion(Quaternion&& q) noexcept
	:x(q.x),
	y(q.y),
	z(q.z),
	w(q.w)
{
	q.x = q.y = q.z = q.w = 0.f;
}

Quaternion::Quaternion(Vector3& axis, float angle)
{
	if (axis.IsNull())
	{
		x = NAN;
		y = NAN;
		z = NAN;
		w = NAN;
	}

	else
	{
		axis = axis.Normalized();

		float halfAngle = angle * 0.5f;
		float sinHalfAngle = std::sin(halfAngle);

		x = std::cos(halfAngle);
		y = axis[0] * sinHalfAngle;
		z = axis[1] * sinHalfAngle;
		w = axis[2] * sinHalfAngle;
	}
}

Quaternion::Quaternion(float angleX, float angleY, float angleZ)
{
	float halfX = angleX * 0.5f;
	float halfY = angleY * 0.5f;
	float halfZ = angleZ * 0.5f;

	float cosX = cosf(halfX);
	float sinX = sinf(halfX);
	float cosY = cosf(halfY);
	float sinY = sinf(halfY);
	float cosZ = cosf(halfZ);
	float sinZ = sinf(halfZ);

	x = cosX * cosY * cosZ - sinX * sinY * sinZ;
	y = sinX * cosY * cosZ + cosX * sinY * sinZ;
	z = cosX * sinY * cosZ - sinX * cosY * sinZ;
	w = cosX * cosY * sinZ + sinX * sinY * cosZ;
}

Quaternion::Quaternion(Vector3& angles)
{
	float halfX = angles[0] * 0.5f;
	float halfY = angles[1] * 0.5f;
	float halfZ = angles[2] * 0.5f;

	float cosX = cosf(halfX);
	float sinX = sinf(halfX);
	float cosY = cosf(halfY);
	float sinY = sinf(halfY);
	float cosZ = cosf(halfZ);
	float sinZ = sinf(halfZ);

	x = cosX * cosY * cosZ - sinX * sinY * sinZ;
	y = sinX * cosY * cosZ + cosX * sinY * sinZ;
	z = cosX * sinY * cosZ - sinX * cosY * sinZ;
	w = cosX * cosY * sinZ + sinX * sinY * cosZ;
}

Quaternion::Quaternion(float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, float z1, float z2, float z3, float z4, float w1, float w2, float w3, float w4)
{
	float trace = x1 + y2 + z3;

	if (trace > 0)
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		x = 0.25f / s;
		y = (z2 - y3) * s;
		z = (x3 - z1) * s;
		w = (y1 - x2) * s;
	}
	else
	{
		if (x1 > y2 && x1 > z3)
		{
			float s = 2.0f * sqrtf(1.0f + x1 - y2 - z3);
			x = (z2 - y3) / s;
			y = 0.25f * s;
			z = (x2 + y1) / s;
			w = (x3 + z1) / s;
		}

		else if (y2 > z3)
		{
			float s = 2.0f * sqrtf(1.0f + y2 - x1 - z3);
			x = (x3 - z1) / s;
			y = (x2 + y1) / s;
			z = 0.25f * s;
			w = (y3 + z2) / s;
		}

		else
		{
			float s = 2.0f * sqrtf(1.0f + z3 - x1 - y2);
			x = (y1 - x2) / s;
			y = (x3 + z1) / s;
			z = (y3 + z2) / s;
			w = 0.25f * s;
		}
	}
}

Quaternion::Quaternion(Matrix4 m)
{
	x = sqrtf(1 + m[0][0] + m[1][1] + m[2][2]) / 2.f;

	float a4 = 1 / (4 * x);
	y = (m[2][1] - m[1][2]) * a4;
	z = (m[0][2] - m[2][0]) * a4;
	w = (m[1][0] - m[0][1]) * a4;
}

float Quaternion::GetReal() const
{
	return x;
}

Vector3 Quaternion::GetImaginary() const
{
	return Vector3(y, z, w);
}

Vector4 Quaternion::GetRotationAndAxis() const
{
	float angle = 2 * std::acos(x);
	float sin_half_angle = std::sqrt(1 - x * x);

	if (sin_half_angle < EPSILON)
	{
		return Vector4(0.f, 0.f, 0.f, 0.f);
	}

	else
	{
		return Vector4(y / sin_half_angle, z / sin_half_angle, w / sin_half_angle, angle);
	}
}

Vector3 Quaternion::GetRotationAxis() const
{
	float sin_half_angle = std::sqrt(1 - x * x);

	if (sin_half_angle < EPSILON)
	{
		return Vector3(0.f, 0.f, 0.f);
	}

	else
	{
		return Vector3(y / sin_half_angle, z / sin_half_angle, w / sin_half_angle);
	}
}

float Quaternion::GetRotation() const
{
	return 2 * std::acos(x);
}

float* Quaternion::GetData()
{
	return &x;
}


bool Quaternion::IsNull() const
{
	return fabs(x) < EPSILON && fabs(y) < EPSILON && fabs(z) < EPSILON && fabs(w) < EPSILON;
}

bool Quaternion::IsUnit() const
{
	return std::fabs(Norm() - 1.0f) < EPSILON;
}

bool Quaternion::IsPureReal() const
{
	return fabs(x) >= EPSILON && fabs(y) < EPSILON && fabs(z) < EPSILON && fabs(w) < EPSILON;
}

bool Quaternion::IsPureImaginary() const
{
	return fabs(x) < EPSILON && (fabs(y) >= EPSILON || fabs(z) >= EPSILON || fabs(w) >= EPSILON);
}

Quaternion Quaternion::Opposite() const
{
	return Quaternion(-x, -y, -z, -w);
}

Quaternion Quaternion::Conjugation() const
{
	return Quaternion(x, -y, -z, -w);
}

Quaternion Quaternion::Inverse() const
{
	float squaredNorm = SquaredNorm();

	if (squaredNorm < EPSILON)
	{
		return Quaternion(NAN);
	}
	return Quaternion(x / squaredNorm, -y / squaredNorm, -z / squaredNorm, -w / squaredNorm);
}


Quaternion Quaternion::Normalize() const
{
	float myNorm = Norm();

	if (myNorm >= EPSILON)
	{
		float _x = x / myNorm;
		float _y = y / myNorm;
		float _z = z / myNorm;
		float _w = w / myNorm;

		return Quaternion(_x, _y, _z, _w);
	}

	return Quaternion(NAN);
}


float Quaternion::SquaredNorm() const
{
	return x * x + y * y + z * z + w * w;
}

float Quaternion::Norm() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

float Quaternion::DotProduct(const Quaternion& q) const
{
	return x * q.x + y * q.y + z * q.z + w * q.w;
}


Vector3 Quaternion::Rotate(const Vector3& p, const Quaternion& q)
{
	return (q * Quaternion(0.f, p[0], p[1], p[2]) * q.Conjugation()).GetImaginary();
}

Vector3 Quaternion::RotateInverse(const Vector3& p, const Quaternion& q)
{
	return (q.Conjugation() * Quaternion(0.f, p[0], p[1], p[2]) * q).GetImaginary();
}


Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, const float t)
{
	return Quaternion(
		(1 - t) * q1.x + t * q2.x,
		(1 - t) * q1.y + t * q2.y,
		(1 - t) * q1.z + t * q2.z,
		(1 - t) * q1.w + t * q2.w);
}

Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, const float t)
{
	float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	Quaternion q2Copy = q2;

	if (dot < 0.f)
	{
		dot = -dot;
		q2Copy = q2.Opposite();
	}

	if (dot > 0.9995f)
		return q1 * (1.0f - t) + q2Copy * t;

	float theta = acosf(dot);
	float sinTheta = sqrtf(1.0f - dot * dot);

	float factor1 = sinf((1.0f - t) * theta) / sinTheta;
	float factor2 = sinf(t * theta) / sinTheta;

	return Quaternion(
		factor1 * q1.x + factor2 * q2Copy.x,
		factor1 * q1.y + factor2 * q2Copy.y,
		factor1 * q1.z + factor2 * q2Copy.z,
		factor1 * q1.w + factor2 * q2Copy.w);
}

Quaternion Quaternion::Nlerp(const Quaternion& q1, const Quaternion& q2, const float t)
{
	Quaternion result(
		(1 - t) * q1.x + t * q2.x,
		(1 - t) * q1.y + t * q2.y,
		(1 - t) * q1.z + t * q2.z,
		(1 - t) * q1.w + t * q2.w);

	return result.Normalize();
}


Matrix4 Quaternion::ToMatrix() const
{
	float aa = x * x, bb = y * y, cc = z * z, dd = w * w;

	return Matrix4(
		aa + bb - cc - dd, 2.0f * (y * z - x * w), 2.0f * (y * w + x * z), 0.f,
		2.0f * (y * z + x * w), aa - bb + cc - dd, 2.0f * (z * w - x * y), 0.f,
		2.0f * (y * w - x * z), 2.0f * (z * w + x * y), aa - bb - cc + dd, 0.f,
		0.f, 0.f, 0.f, 1.f);
}

Vector3 Quaternion::ToEulerAngle() const
{
	Quaternion q = this->Normalize();

	float aa = q.x * q.x, bb = q.y * q.y, cc = q.z * q.z, dd = q.w * q.w;

	float m00 = aa + bb - cc - dd;
	float m01 = 2.f * (q.y * q.z - q.x * q.w);
	float m02 = 2.f * (q.y * q.w + q.x * q.z);
	float m11 = aa - bb + cc - dd;
	float m12 = 2.f * (q.z * q.w - q.x * q.y);
	float m21 = 2.f * (q.z * q.w + q.x * q.y);
	float m22 = aa - bb - cc + dd;

	if (fabs(m02) < 1.0f)
		return Vector3(atan2f(-m12, m22), asinf(m02), atan2f(-m01, m00));

	// Gimbal lock (Axe X)
	else
		return Vector3(atan2f(m21, m11), (m02 > 0.0f) ? PI / 2.f : -PI / 2, 0.0f);
}

void Quaternion::Print() const
{
	printf("a = %g\nb = %g\nc = %g\nd = %g\n", x, y, z, w);
}


Quaternion Quaternion::operator+(const Quaternion& q) const
{
	return Quaternion(x + q[0], y + q[1], z + q[2], w + q[3]);
}

Quaternion Quaternion::operator+(float scal) const
{
	return Quaternion(x + scal, y + scal, z + scal, w + scal);
}

Quaternion Quaternion::operator-(const Quaternion& q) const
{
	return Quaternion(x - q[0], y - q[1], z - q[2], w - q[3]);
}

Quaternion Quaternion::operator-(float scal) const
{
	return Quaternion(x + scal, y + scal, z + scal, w + scal);
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	return Quaternion(
		x * q[0] - y * q[1] - z * q[2] - w * q[3],
		y * q[0] + x * q[1] - w * q[2] + z * q[3],
		z * q[0] + w * q[1] + x * q[2] - y * q[3],
		w * q[0] - z * q[1] + y * q[2] + x * q[3]
	);
}

Quaternion Quaternion::operator*(float scal) const
{
	return Quaternion(x * scal, y * scal, z * scal, w * scal);
}

Quaternion Quaternion::operator/(const Quaternion& q) const
{
	return Quaternion(*this * q.Inverse());
}


Quaternion Quaternion::operator/(float scal) const
{
	if (fabs(scal) >= EPSILON)
		return Quaternion(x / scal, y / scal, z / scal, w / scal);

	return Quaternion(NAN);
}

void Quaternion::operator+=(const Quaternion& q)
{
	x += q[0];
	y += q[1];
	z += q[2];
	w += q[3];
}

void Quaternion::operator+=(float scal)
{
	x += scal;
	y += scal;
	z += scal;
	w += scal;
}

void Quaternion::operator-=(const Quaternion& q)
{
	x -= q[0];
	y -= q[1];
	z -= q[2];
	w -= q[3];
}

void Quaternion::operator-=(float scal)
{
	x -= scal;
	y -= scal;
	z -= scal;
	w -= scal;
}

void Quaternion::operator*=(const Quaternion& q)
{
	float newA = x * q[0] - y * q[1] - z * q[2] - w * q[3];
	float newB = y * q[0] + x * q[1] - w * q[2] + z * q[3];
	float newC = z * q[0] + w * q[1] + x * q[2] - y * q[3];
	float newD = w * q[0] - z * q[1] + y * q[2] + x * q[3];

	x = newA;
	y = newB;
	z = newC;
	w = newD;
}

void Quaternion::operator*=(float scal)
{
	x *= scal;
	y *= scal;
	z *= scal;
	w *= scal;
}

void Quaternion::operator/=(const Quaternion& q)
{
	*this *= q.Inverse();
}

void Quaternion::operator/=(float scal)
{
	if (fabs(scal) >= EPSILON)
	{
		x /= scal;
		y /= scal;
		z /= scal;
		w /= scal;
		return;
	}

	x = NAN;
	y = NAN;
	z = NAN;
	w = NAN;
}

bool Quaternion::operator==(const Quaternion& q) const
{
	return fabs(x - q[0]) < EPSILON && fabs(y - q[1]) < EPSILON && fabs(z - q[2]) < EPSILON && fabs(w - q[3]) < EPSILON;
}

bool Quaternion::operator!=(const Quaternion& q) const
{
	return fabs(x - q[0]) >= EPSILON && fabs(y - q[1]) >= EPSILON && fabs(z - q[2]) >= EPSILON && fabs(w - q[3]) >= EPSILON;
}

Quaternion& Quaternion::operator=(const Quaternion& q)
{
	if (this != &q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}

	return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& q) noexcept
{
	if (this != &q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
		q.x = q.y = q.z = q.w = 0.f;
	}

	return *this;
}


float Quaternion::operator[](int index) const
{
	if (index > -1 && index < 4)
		return *(&x + index);

	else
		return NAN;
}

std::ostream& MirageEngine::Core::Math::operator<<(std::ostream& os, const Quaternion& q)
{
	return os << "a = " << q[0] << ", b = " << q[1] << ", c = " << q[2] << ", d = " << q[3] << std::endl;
}
