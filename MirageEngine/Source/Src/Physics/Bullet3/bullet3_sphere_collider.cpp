#include <Physics/Bullet3/bullet3_sphere_collider.h>
using namespace MirageEngine::Core::Physics;

MirageEngine::Core::Physics::Bullet3SphereCollider::~Bullet3SphereCollider()
{
    if (m_Compound)
    {
        m_Compound->removeChildShape(m_CollisionShape);
        delete m_Compound;
    }
    if (m_CollisionShape)
    {
        delete m_CollisionShape;
        m_CollisionShape = nullptr;
    }
}

bool Bullet3SphereCollider::Create()
{
    m_CollisionShape = new btSphereShape(m_Radius);
    if (m_CollisionShape == nullptr) return false;
    if (m_Offset.IsNull() && m_Rot.IsNull()) return true;
    m_Compound = new btCompoundShape();
    btTransform local;
    local.setIdentity();
    local.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
    local.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));
    m_CollisionShape->setLocalScaling(btVector3(m_Scale.X, m_Scale.Y, m_Scale.Z));

    m_Compound->addChildShape(local, m_CollisionShape);
    return true;
}

void Bullet3SphereCollider::SetOffset(Vector3 _offset, Quaternion _rotation)
{
    if (!m_CollisionShape) return;
    if (!m_Compound) m_Compound = new btCompoundShape();
    else m_Compound->removeChildShape(m_CollisionShape);

    m_Rot    = _rotation;
    m_Offset = _offset;

    btTransform local;
    local.setIdentity();
    local.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
    local.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));

    m_Compound->addChildShape(local, m_CollisionShape);
}
