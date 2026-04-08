#include <Physics/Bullet3/bullet3_box_collider.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>
using namespace MirageEngine::Core::Physics;

Bullet3BoxCollider::~Bullet3BoxCollider()
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

bool Bullet3BoxCollider::Create()
{
    m_CollisionShape = new btBoxShape(btVector3(m_HalfExtents.X, m_HalfExtents.Y, m_HalfExtents.Z));
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

void Bullet3BoxCollider::SetOffset(Vector3 _offset, Quaternion _rotation)
{
    m_Offset = _offset;
    m_Rot    = _rotation;

    if (m_RigidBody) { UpdateRigidBodyTransform(); }
}

void Bullet3BoxCollider::UpdateRigidBodyTransform()
{
    if (!m_RigidBody || !m_CollisionShape) return;

    btTransform localTransform;
    localTransform.setIdentity();
    localTransform.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
    localTransform.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));

    btDefaultMotionState* motionState = new btDefaultMotionState(localTransform);

    btRigidBody* rb = m_RigidBody->API_Bullet3().GetBullet3RigidBody();

    btVector3   linearVel      = rb->getLinearVelocity();
    btVector3   angularVel     = rb->getAngularVelocity();
    float       mass           = 1.0f / rb->getInvMass();
    btTransform worldTransform = rb->getWorldTransform();

    btMotionState* oldMotionState = rb->getMotionState();
    rb->setMotionState(motionState);
    delete oldMotionState;

    rb->setWorldTransform(worldTransform);
    rb->setLinearVelocity(linearVel);
    rb->setAngularVelocity(angularVel);

    rb->activate(true);
}

void Bullet3BoxCollider::SetHalfExtents(const Vector3& halfExtents)
{
    if (m_HalfExtents == halfExtents) return;

    m_HalfExtents = halfExtents;

    btCollisionShape* oldShape = m_CollisionShape;

    m_CollisionShape = new btBoxShape(btVector3(halfExtents.X, halfExtents.Y, halfExtents.Z));

    if (m_Compound)
    {
        m_Compound->removeChildShape(m_CollisionShape);

        btTransform local;
        local.setIdentity();
        local.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
        local.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));

        m_Compound->addChildShape(local, m_CollisionShape);
    }

    if (m_RigidBody) { m_RigidBody->API_Bullet3().SetNewCollisionShape(this); }

    delete oldShape;
}

void Bullet3BoxCollider::UpdateCompoundShape()
{
    if (!m_CollisionShape) return;

    const bool needsCompound = !(m_Offset.IsNull() && m_Rot == Quaternion(0.f, 0.f, 0.f, 1.f));

    if (needsCompound)
    {
        if (!m_Compound) m_Compound = new btCompoundShape();

        m_Compound->removeChildShape(m_CollisionShape);

        btTransform local;
        local.setIdentity();
        local.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
        local.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));

        m_Compound->addChildShape(local, m_CollisionShape);
    }
    else
    {
        if (m_Compound)
        {
            delete m_Compound;
            m_Compound = nullptr;
        }
    }

    if (m_RigidBody) m_RigidBody->API_Bullet3().SetNewCollisionShape(this);
}

Vector3 Bullet3BoxCollider::GetHalfExtents() const { return m_HalfExtents; }
