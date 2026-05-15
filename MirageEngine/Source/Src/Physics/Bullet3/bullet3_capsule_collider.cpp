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
    p_bIsDirty = false;
    m_CollisionShape = new btCapsuleShape(m_Radius, m_Height);
    if (p_Offset.IsNull() && p_Rot.IsNull())
        return;

    m_LocalTransform.setIdentity();
    m_LocalTransform.setOrigin(btVector3(p_Offset.X, p_Offset.Y, p_Offset.Z));
    m_LocalTransform.setRotation(btQuaternion(p_Rot.X, p_Rot.Y, p_Rot.Z, p_Rot.W));
    m_CollisionShape->setLocalScaling(btVector3(p_Scale.X, p_Scale.Y, p_Scale.Z));
}

void Bullet3CapsuleCollider::SetOffset(Vector3 offset, Quaternion _rotation)
{
    p_Offset = offset;
    p_Rot = _rotation;
    p_bIsDirty = true;
}

void Bullet3CapsuleCollider::SetRadius(float _radius)
{
    if (m_Radius == _radius)
        return;
    m_Radius = _radius;
    p_bIsDirty = true;
}

void Bullet3CapsuleCollider::SetHeight(float _height)
{
    if (m_Height == _height)
        return;
    m_Height = _height;
    p_bIsDirty = true;
}

btTransform Bullet3CapsuleCollider::GetTransform()
{
    if (p_bIsDirty)
    {
        m_LocalTransform.setIdentity();
        m_LocalTransform.setOrigin(btVector3(p_Offset.X, p_Offset.Y, p_Offset.Z));
        m_LocalTransform.setRotation(btQuaternion(p_Rot.X, p_Rot.Y, p_Rot.Z, p_Rot.W));
    }
    return m_LocalTransform;
}
