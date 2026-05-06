#include <Physics/Bullet3/bullet3_capsule_collider.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>

using namespace MirageEngine::Core::Physics;

Bullet3CapsuleCollider::~Bullet3CapsuleCollider()
{
    if (m_CollisionShape)
    {
        delete m_CollisionShape;
        m_CollisionShape = nullptr;
    }
}

void Bullet3CapsuleCollider::Create()
{
    m_bIsDirty = false;
    m_CollisionShape = new btCapsuleShape(m_Radius, m_Height);
    if (m_Offset.IsNull() && m_Rot.IsNull())
        return;

    m_LocalTransform.setIdentity();
    m_LocalTransform.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
    m_LocalTransform.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));
    m_CollisionShape->setLocalScaling(btVector3(m_Scale.X, m_Scale.Y, m_Scale.Z));
}

void Bullet3CapsuleCollider::SetOffset(Vector3 offset, Quaternion _rotation)
{
    m_Offset = offset;
    m_Rot = _rotation;
    m_bIsDirty = true;
}

void Bullet3CapsuleCollider::SetRadius(float _radius)
{
    if (m_Radius == _radius)
        return;
    m_Radius = _radius;
    m_bIsDirty = true;
}

void Bullet3CapsuleCollider::SetHeight(float _height)
{
    if (m_Height == _height)
        return;
    m_Height = _height;
    m_bIsDirty = true;
}

btTransform Bullet3CapsuleCollider::GetTransform()
{
    if (m_bIsDirty)
    {
        m_LocalTransform.setIdentity();
        m_LocalTransform.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
        m_LocalTransform.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));
    }
    return m_LocalTransform;
}
