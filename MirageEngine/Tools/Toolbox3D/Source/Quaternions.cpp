#include <Quaternions.h>
#include <math.h>

void Quaternion::ToAxisAngle(const float* q, float* axis, float& angle)
{
	float sinHalfAngle = sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]);
	angle = 2.0f * atan2f(sinHalfAngle, q[3]);
	if (sinHalfAngle > 0.0001f) {
		axis[0] = q[0] / sinHalfAngle;
		axis[1] = q[1] / sinHalfAngle;
		axis[2] = q[2] / sinHalfAngle;
	} else {
		axis[0] = 1.0f;
		axis[1] = 0.0f;
		axis[2] = 0.0f;
	}
}

void Quaternion::FromAxisAngle(const float* axis, float angle, float* q)
{
	float halfAngle = angle * 0.5f;
	float sinHalfAngle = sinf(halfAngle);
	q[0] = axis[0] * sinHalfAngle;
	q[1] = axis[1] * sinHalfAngle;
	q[2] = axis[2] * sinHalfAngle;
	q[3] = cosf(halfAngle);
}

void Quaternion::ToEuler(const float* q, float& roll, float& pitch, float& yaw)
{
	roll = atan2f(2.0f * (q[3] * q[0] + q[1] * q[2]), 1.0f - 2.0f * (q[0] * q[0] + q[1] * q[1]));
	pitch = asinf(2.0f * (q[3] * q[1] - q[2] * q[0]));
	yaw = atan2f(2.0f * (q[3] * q[2] + q[0] * q[1]), 1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]));
}

void Quaternion::FromEuler(float roll, float pitch, float yaw, float* q)
{
	float cy = cosf(yaw * 0.5f);
	float sy = sinf(yaw * 0.5f);
	float cp = cosf(pitch * 0.5f);
	float sp = sinf(pitch * 0.5f);
	float cr = cosf(roll * 0.5f);
	float sr = sinf(roll * 0.5f);
	q[0] = sr * cp * cy - cr * sp * sy;
	q[1] = cr * sp * cy + sr * cp * sy;
	q[2] = cr * cp * sy - sr * sp * cy;
	q[3] = cr * cp * cy + sr * sp * sy;
}

void Quaternion::Normalize(float* q)
{
	float length = sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	if (length > 0.0001f) {
		q[0] /= length;
		q[1] /= length;
		q[2] /= length;
		q[3] /= length;
	}
}

void Quaternion::Conjugate(const float* q, float* result)
{
	result[0] = -q[0];
	result[1] = -q[1];
	result[2] = -q[2];
	result[3] = q[3];
}

void Quaternion::Multiply(const float* q1, const float* q2, float* result)
{
	result[0] = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
	result[1] = q1[3] * q2[1] - q1[0] * q2[2] + q1[1] * q2[3] + q1[2] * q2[0];
	result[2] = q1[3] * q2[2] + q1[0] * q2[1] - q1[1] * q2[0] + q1[2] * q2[3];
	result[3] = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
}


