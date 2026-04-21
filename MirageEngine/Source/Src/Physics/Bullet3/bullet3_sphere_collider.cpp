#include <Physics/Bullet3/bullet3_sphere_collider.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>
using namespace MirageEngine::Core::Physics;

Bullet3SphereCollider::~Bullet3SphereCollider()
{
    if (m_CollisionShape)
    {
        delete m_CollisionShape;
        m_CollisionShape = nullptr;
    }
}

void Bullet3SphereCollider::Create()
{
    m_CollisionShape = new btSphereShape(m_Radius);
    if (m_Offset.IsNull() && m_Rot.IsNull()) return;

    m_LocalTransform.setIdentity();
    m_LocalTransform.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
    m_LocalTransform.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));
    m_CollisionShape->setLocalScaling(btVector3(m_Scale.X, m_Scale.Y, m_Scale.Z));
}

void Bullet3SphereCollider::SetOffset(Vector3 _offset, Quaternion _rotation)
{
    if (!m_CollisionShape) return;

    m_Rot      = _rotation;
    m_Offset   = _offset;
    m_bIsDirty = true;
}

void Bullet3SphereCollider::SetRadius(float _radius)
{
    if (m_Radius == _radius) return;
    m_Radius = _radius;
}

btTransform Bullet3SphereCollider::GetTransform()
{
    if (m_bIsDirty)
    {
        m_LocalTransform.setIdentity();
        m_LocalTransform.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
        m_LocalTransform.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));
    }
    return m_LocalTransform;
}
