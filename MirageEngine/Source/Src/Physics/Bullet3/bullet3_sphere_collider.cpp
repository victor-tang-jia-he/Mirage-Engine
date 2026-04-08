#include <Physics/Bullet3/bullet3_sphere_collider.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>
using namespace MirageEngine::Core::Physics;

Bullet3SphereCollider::~Bullet3SphereCollider()
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

void Bullet3SphereCollider::SetRadius(float _radius)
{
    if (m_Radius == _radius) return;
    m_Radius = _radius;
    UpdateCollisionShape();
}

void Bullet3SphereCollider::UpdateRigidBodyTransform()
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

void Bullet3SphereCollider::UpdateCompoundShape()
{
    const bool needsCompound = !(m_Offset.IsNull() && m_Rot == Quaternion(0.f, 0.f, 0.f, 1.f));

    btCollisionShape* newShape = new btSphereShape(m_Radius);
    btCollisionShape* oldShape = m_CollisionShape;
    m_CollisionShape           = newShape;

    if (needsCompound)
    {
        if (!m_Compound) { m_Compound = new btCompoundShape(); }
        else { m_Compound->removeChildShape(oldShape); }

        btTransform local;
        local.setIdentity();
        local.setOrigin(btVector3(m_Offset.X, m_Offset.Y, m_Offset.Z));
        local.setRotation(btQuaternion(m_Rot.X, m_Rot.Y, m_Rot.Z, m_Rot.W));
        m_Compound->addChildShape(local, newShape);
    }
    else
    {
        if (m_Compound)
        {
            m_Compound->removeChildShape(oldShape);
            delete m_Compound;
            m_Compound = nullptr;
        }
    }

    if (m_RigidBody) { m_RigidBody->API_Bullet3().SetNewCollisionShape(this); }

    delete oldShape;
}

void Bullet3SphereCollider::UpdateCollisionShape()
{
    btCollisionShape* oldShape = m_CollisionShape;

    m_CollisionShape = new btSphereShape(m_Radius);

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
