#include <Physics/Bullet3/bullet3_box_collider.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>
using namespace MirageEngine::Core::Physics;

Bullet3BoxCollider::~Bullet3BoxCollider()
{
    if (m_CollisionShape)
    {
        delete m_CollisionShape;
        m_CollisionShape = nullptr;
    }
}

void Bullet3BoxCollider::Create()
{
    m_bIsDirty       = false;
    m_CollisionShape = new btBoxShape(btVector3(m_HalfExtents.X, m_HalfExtents.Y, m_HalfExtents.Z));
    if (m_Offset.IsNull() && m_Rot.IsNull()) return;

    m_LocalTransform.setIdentity();
    m_LocalTransform.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
    m_LocalTransform.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));
    m_CollisionShape->setLocalScaling(btVector3(m_Scale.X, m_Scale.Y, m_Scale.Z));
}

void Bullet3BoxCollider::SetOffset(Vector3 _offset, Quaternion _rotation)
{
    m_Offset   = _offset;
    m_Rot      = _rotation;
    m_bIsDirty = true;
}

void Bullet3BoxCollider::SetHalfExtents(const Vector3& halfExtents)
{
    if (m_HalfExtents == halfExtents) return;

    m_HalfExtents = halfExtents;
    m_bIsDirty    = true;
}

Vector3 Bullet3BoxCollider::GetHalfExtents() const { return m_HalfExtents; }

btTransform Bullet3BoxCollider::GetTransform()
{
    if (m_bIsDirty)
    {
        m_LocalTransform.setIdentity();
        m_LocalTransform.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
        m_LocalTransform.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));
    }
    return m_LocalTransform;
}
