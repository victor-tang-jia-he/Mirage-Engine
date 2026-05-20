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
    if (p_Offset.IsNull() && p_Rot.IsNull())
        return;

    m_LocalTransform.setIdentity();
    m_LocalTransform.setOrigin(btVector3(p_Offset.X, p_Offset.Y, p_Offset.Z));
    m_LocalTransform.setRotation(btQuaternion(p_Rot.X, p_Rot.Y, p_Rot.Z, p_Rot.W));
    m_CollisionShape->setLocalScaling(btVector3(p_Scale.X, p_Scale.Y, p_Scale.Z));
}

void Bullet3SphereCollider::SetOffset(Vector3 _offset, Quaternion _rotation)
{
    if (!m_CollisionShape)
        return;

    p_Rot = _rotation;
    p_Offset = _offset;
    p_bIsDirty = true;
}

void Bullet3SphereCollider::SetRadius(float _radius)
{
    if (m_Radius == _radius)
        return;
    m_Radius = _radius;
}

btTransform Bullet3SphereCollider::GetTransform()
{
    if (p_bIsDirty)
    {
        m_LocalTransform.setIdentity();
        m_LocalTransform.setOrigin(btVector3(p_Offset.X, p_Offset.Y, p_Offset.Z));
        m_LocalTransform.setRotation(btQuaternion(p_Rot.X, p_Rot.Y, p_Rot.Z, p_Rot.W));
    }
    return m_LocalTransform;
}
