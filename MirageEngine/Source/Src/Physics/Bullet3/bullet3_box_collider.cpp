#include <Physics/Bullet3/bullet3_box_collider.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>
using namespace MirageEngine::Core::Physics;

Bullet3BoxCollider::~Bullet3BoxCollider()
{
    if (m_CollisionShape)
    {
        delete m_CollisionShape;
    }
}

void Bullet3BoxCollider::Create()
{
    p_bIsDirty = false;
    m_CollisionShape = new btBoxShape(btVector3(m_HalfExtents.X, m_HalfExtents.Y, m_HalfExtents.Z));
    if (p_Offset.IsNull() && p_Rot.IsNull())
        return;

    m_LocalTransform.setIdentity();
    m_LocalTransform.setOrigin(btVector3(p_Offset.X, p_Offset.Y, p_Offset.Z));
    m_LocalTransform.setRotation(btQuaternion(p_Rot.X, p_Rot.Y, p_Rot.Z, p_Rot.W));
    m_CollisionShape->setLocalScaling(btVector3(p_Scale.X, p_Scale.Y, p_Scale.Z));
}

void Bullet3BoxCollider::SetOffset(Vector3 _offset, Quaternion _rotation)
{
    p_Offset = _offset;
    p_Rot = _rotation;
    p_bIsDirty = true;
}

void Bullet3BoxCollider::SetHalfExtents(const Vector3& halfExtents)
{
    if (m_HalfExtents == halfExtents)
        return;

    m_HalfExtents = halfExtents;
    p_bIsDirty = true;
}

Vector3 Bullet3BoxCollider::GetHalfExtents() const { return m_HalfExtents; }

btTransform Bullet3BoxCollider::GetTransform()
{
    if (p_bIsDirty)
    {
        m_LocalTransform.setIdentity();
        m_LocalTransform.setOrigin(btVector3(p_Offset.X, p_Offset.Y, p_Offset.Z));
        m_LocalTransform.setRotation(btQuaternion(p_Rot.X, p_Rot.Y, p_Rot.Z, p_Rot.W));
    }
    return m_LocalTransform;
}
