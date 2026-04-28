#include <Components/transform.h>
#include <matrix4.h>

#include <GameObject/game_object.h>

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Math;

Transform::Transform(Vector3 _position, Quaternion _rotation, Vector3 _scale)
    : m_LocalPosition(_position)
    , m_LocalRotation(_rotation)
    , m_LocalScale(_scale)
{
    Init();
}

void Transform::Init()
{
}

void Transform::Update()
{

}

Matrix4 Transform::GetMatrix()
{
    if (m_bDirty)
    {
        m_LocalMatrix = Matrix4::CreateTransformMatrix(m_LocalPosition, m_LocalRotation, m_LocalScale);
        m_bDirty      = false;
    }
    return m_LocalMatrix;
}

Vector3 Transform::Rotate(const Quaternion& _q, const Vector3& _v) const
{
    // Encode the 3D vector as a pure quaternion (real part = 0)
    // qv = (0, vx, vy, vz)
    Quaternion qv(0.f, _v.X, _v.Y, _v.Z);

    // Quaternion rotation formula : v' = q * v * q-1
    // For a unit quaternion, q-1 == conjugate(q) = (w, -x, -y, -z)
    // The result is a pure quaternion whose imaginary part is the rotated vector
    Quaternion res = _q * qv * _q.Conjugation();

    // Extract the imaginary part, this is the rotated vector
    return Vector3(res.X, res.Y, res.Z);
}

void Transform::LookAt(const Vector3& _target)
{
    Vector3 f = (_target - m_LocalPosition).Normalized();
    Vector3 s = f.CrossProductVector3(Vector3(0.f, 1.f, 0.f)).Normalized();
    Vector3 u = s.CrossProductVector3(f);

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

Vector3 Transform::Forward() const { return Rotate(m_LocalRotation, Vector3(0.f, 0.f, -1.f)); }

Vector3 Transform::Up() const { return Rotate(m_LocalRotation, Vector3(0.f, 1.f, 0.f)); }

Vector3 Transform::Right() const { return Rotate(m_LocalRotation, Vector3(-1.f, 0.f, 0.f)); }

Vector3 Transform::Left() const { return -Right(); }

void Transform::SetPosition(Vector3 _pos)
{
    m_bDirty        = true;
    m_LocalPosition = _pos;
}

void Transform::SetScale(Vector3 _scale)
{
    m_bDirty     = true;
    m_LocalScale = _scale;
}

void Transform::SetRotation(Quaternion _rotation)
{
    m_bDirty        = true;
    m_LocalRotation = _rotation;
}

void Transform::SetLocalMatrix(Math::Matrix4 _mat)
{
	m_LocalPosition = _mat.GetTranslation();
	m_LocalScale = _mat.GetScale();
	m_LocalRotation = _mat.GetRotation();
	m_bDirty = true;
}
