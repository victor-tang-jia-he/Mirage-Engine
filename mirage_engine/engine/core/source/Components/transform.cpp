#include <transform.h>
#include <matrix4.h>
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Math;

Matrix4 Transform::GetMatrix()
{
	if (dirty)
	{
		localMatrix = Matrix4::CreateTransformMatrix(position, rotation.ToEulerAngle(), scale);
	}
	return localMatrix;
}

Vector3 Transform::Rotate(const Quaternion& q, const Vector3& v) const
{
	Quaternion qv(v.X, v.Y, v.Z, 0.f);
	Quaternion res = q * qv * q.Conjugation();
	return Vector3(res.X, res.Y, res.Z);
}

void Transform::LookAt(const Vector3& target)
{
	Vector3 f = (target - position).Normalized();
	Vector3 s = Vector3(0.f, 1.f, 0.f).CrossProductVector3(f).Normalized();
	Vector3 u = f.CrossProductVector3(s);

	Matrix4 result = Matrix4::Identity();

	result(0, 0) = s.X;
	result(1, 0) = s.Y;
	result(2, 0) = s.Z;

	result(0, 1) = u.X;
	result(1, 1) = u.Y;
	result(2, 1) = u.Z;

	result(0, 2) = f.X;
	result(1, 2) = f.Y;
	result(2, 2) = f.Z;

	rotation = Quaternion(result);
}

Vector3 Transform::Forward() const
{
	return Rotate(rotation, Vector3(0.f, 0.f, 1.f));
}

Vector3 Transform::Up() const
{
	return Rotate(rotation, Vector3(0.f, 1.f, 0.f));
}

Vector3 Transform::Right() const
{
	return Rotate(rotation, Vector3(1.f, 0.f, 0.f));
}

Vector3 Transform::Left() const
{
	return -Right();
}

void Transform::SetPosition(Vector3 _pos)
{
	dirty = true;
	position = _pos;
}

void Transform::SetScale(Vector3 _scale)
{
	dirty = true;
	scale = _scale;
}

void Transform::SetRotation(Quaternion _rotation)
{
	dirty = true;
	rotation = _rotation;
}

