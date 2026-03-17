#pragma once

#include <ostream>

#include "matrix4.h"

namespace MirageEngine
{
	namespace Core
	{
		namespace Math
		{
			struct Quaternion
			{
				float x = 0.f;
				float y = 0.f;
				float z = 0.f;
				float w = 0.f;

				// Constructor
				Quaternion(float num = 0.f) noexcept;
				Quaternion(float _a, float _b, float _c, float _d) noexcept;
				Quaternion(const Quaternion& q) noexcept;
				Quaternion(Quaternion&& q) noexcept;
				Quaternion(Vector3& axis, float angle);
				Quaternion(float angleX, float angleY, float angleZ);
				Quaternion(Vector3& angles);
				Quaternion(float x1, float x2, float x3, float x4,
					float y1, float y2, float y3, float y4,
					float z1, float z2, float z3, float z4,
					float w1, float w2, float w3, float w4);
				Quaternion(Matrix4 m);
				~Quaternion() = default;

				// Getter
				float GetReal() const;
				Vector3 GetImaginary() const;
				Vector4 GetRotationAndAxis() const;
				Vector3 GetRotationAxis() const;
				float GetRotation() const;
				float* GetData();

				bool IsNull() const;
				bool IsUnit() const;
				bool IsPureReal() const;
				bool IsPureImaginary() const;

				Quaternion Opposite() const;
				Quaternion Conjugation() const;
				Quaternion Inverse() const;
				Quaternion Normalize() const;

				float SquaredNorm() const;
				float Norm() const;
				float DotProduct(const Quaternion& q) const;

				static Vector3 Rotate(const Vector3& p = Vector3(0.f, 0.f, 0.f), const Quaternion& q = Quaternion(1.f, 0.f, 0.f, 0.f));
				static Vector3 RotateInverse(const Vector3& p = Vector3(0.f, 0.f, 0.f), const Quaternion& q = Quaternion(1.f, 0.f, 0.f, 0.f));

				static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, const float t);
				static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, const float t);
				static Quaternion Nlerp(const Quaternion& q1, const Quaternion& q2, const float t);

				Matrix4 ToMatrix() const;
				Vector3 ToEulerAngle() const;

				void Print() const;

				Quaternion operator+(const Quaternion& q) const;
				Quaternion operator+(float scal) const;
				Quaternion operator-(const Quaternion& q) const;
				Quaternion operator-(float scal) const;
				Quaternion operator*(const Quaternion& q) const;
				Quaternion operator*(float scal) const;
				Quaternion operator/(const Quaternion& q) const;
				Quaternion operator/(float scal) const;

				void operator+=(const Quaternion& q);
				void operator+=(float scal);
				void operator-=(const Quaternion& q);
				void operator-=(float scal);
				void operator*=(const Quaternion& q);
				void operator*=(float scal);
				void operator/=(const Quaternion& q);
				void operator/=(float scal);

				bool operator==(const Quaternion& q) const;
				bool operator!=(const Quaternion& q) const;

				Quaternion& operator=(const Quaternion& q);
				Quaternion& operator=(Quaternion&& q) noexcept;
				float operator[](int index) const;
			};

			std::ostream& operator<<(std::ostream& os, const Quaternion& q);
		}
	}
}