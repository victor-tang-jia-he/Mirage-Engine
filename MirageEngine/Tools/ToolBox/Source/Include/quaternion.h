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
				float W = 0.f;
				float X = 0.f;
				float Y = 0.f;
				float Z = 0.f;

				// Constructor
				Quaternion(float _num = 0.f) noexcept;
				Quaternion(float _a, float _b, float _c, float _d) noexcept;
				Quaternion(const Quaternion& _q) noexcept;
				Quaternion(Quaternion&& _q) noexcept;
				Quaternion(Vector3& _axis, float _angle);
				Quaternion(float _angleX, float _angleY, float _angleZ);
				Quaternion(Vector3& angles);
				Quaternion(float _x1, float _x2, float _x3, float _x4,
					float _y1, float _y2, float _y3, float _y4,
					float _z1, float z2, float _z3, float _z4,
					float _w1, float _w2, float _w3, float _w4);
				Quaternion(Matrix4 _m);
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

				static Quaternion Identity() { return Quaternion(1.f, 0.f, 0.f, 0.f); }

				float SquaredNorm() const;
				float Norm() const;
				float DotProduct(const Quaternion& _q) const;

				static Vector3 Rotate(const Vector3& p = Vector3(0.f, 0.f, 0.f), const Quaternion& _q = Quaternion(1.f, 0.f, 0.f, 0.f));
				static Vector3 RotateInverse(const Vector3& p = Vector3(0.f, 0.f, 0.f), const Quaternion& _q = Quaternion(1.f, 0.f, 0.f, 0.f));

				static Quaternion Lerp(const Quaternion& _q1, const Quaternion& _q2, const float _t);
				static Quaternion Slerp(const Quaternion& _q1, const Quaternion& _q2, const float _t);
				static Quaternion Nlerp(const Quaternion& _q1, const Quaternion& _q2, const float _t);

				Matrix4 ToMatrix() const;
				Vector3 ToEulerAngle() const;

				void Print() const;

				Quaternion operator+(const Quaternion& _q) const;
				Quaternion operator+(float _scale) const;
				Quaternion operator-(const Quaternion& _q) const;
				Quaternion operator-(float _scale) const;
				Quaternion operator*(const Quaternion& _q) const;
				Quaternion operator*(float _scale) const;
				Quaternion operator/(const Quaternion& _q) const;
				Quaternion operator/(float _scale) const;

				void operator+=(const Quaternion& _q);
				void operator+=(float _scale);
				void operator-=(const Quaternion& _q);
				void operator-=(float _scale);
				void operator*=(const Quaternion& _q);
				void operator*=(float _scale);
				void operator/=(const Quaternion& _q);
				void operator/=(float _scale);

				bool operator==(const Quaternion& _q) const;
				bool operator!=(const Quaternion& _q) const;

				Quaternion& operator=(const Quaternion& _q);
				Quaternion& operator=(Quaternion&& _q) noexcept;
				float operator[](int _index) const;
			};

			std::ostream& operator<<(std::ostream& _os, const Quaternion& _q);
		}
	}
}