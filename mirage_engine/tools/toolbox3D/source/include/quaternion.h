#pragma once

class Quaternion
{
	public:
	static void ToAxisAngle(const float* q, float* axis, float& angle);
	static void FromAxisAngle(const float* axis, float angle, float* q);
	static void ToEuler(const float* q, float& roll, float& pitch, float& yaw);
	static void FromEuler(float roll, float pitch, float yaw, float* q);
	static void Normalize(float* q);
	static void Conjugate(const float* q, float* result);
	static void Multiply(const float* q1, const float* q2, float* result);
};