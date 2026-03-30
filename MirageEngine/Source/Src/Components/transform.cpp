#include <Components/transform.h>
#include <matrix4.h>
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Math;

Transform::Transform(Vector3 _position, Quaternion _rotation, Vector3 _scale)
	: m_LocalPosition(_position), m_LocalRotation(_rotation), m_LocalScale(_scale)
{
	Init();
}

void Transform::Init()
{
	GetParent()->GetComponent("Transform");
}

void Transform::Update()
{
	while (true)
	{
		break;
	}
}

Matrix4 Transform::GetMatrix()
{
	if (m_bDirty)
	{
		m_LocalMatrix = Matrix4::CreateTransformMatrix(m_LocalPosition, m_LocalRotation.ToEulerAngle(), m_LocalScale);
		m_bDirty = false;
	}
	return m_LocalMatrix;
}

Vector3 Transform::Rotate(const Quaternion& q, const Vector3& v) const
{
	Quaternion qv(0.f, v.X, v.Y, v.Z);
	Quaternion res = q * qv * q.Conjugation();
	return Vector3(res.X, res.Y, res.Z);
}

void Transform::LookAt(const Vector3& target)
{
	Vector3 f = (target - m_LocalPosition).Normalized();
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

	m_LocalRotation = Quaternion(result);
}

Vector3 Transform::Forward() const
{
	return Rotate(m_LocalRotation, Vector3(0.f, 0.f, 1.f));
}

Vector3 Transform::Up() const
{
	return Rotate(m_LocalRotation, Vector3(0.f, 1.f, 0.f));
}

Vector3 Transform::Right() const
{
	return Rotate(m_LocalRotation, Vector3(1.f, 0.f, 0.f));
}

Vector3 Transform::Left() const
{
	return -Right();
}

void Transform::SetPosition(Vector3 _pos)
{
	m_bDirty = true;
	m_LocalPosition = _pos;
}

void Transform::SetScale(Vector3 _scale)
{
	m_bDirty = true;
	m_LocalScale = _scale;
}

void Transform::SetRotation(Quaternion _rotation)
{
	m_bDirty = true;
	m_LocalRotation = _rotation;
}

