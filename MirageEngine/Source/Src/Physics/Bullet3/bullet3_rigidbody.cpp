#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_box_collider.h>
#include <Physics/Bullet3/bullet3_sphere_collider.h>
#include <Physics/Bullet3/bullet3_capsule_collider.h>
#include <Physics/physics_manager.h>

using namespace MirageEngine::Core::Physics;

Bullet3RigidBody::~Bullet3RigidBody()
{
    if (m_rigidBody)
    {
        delete m_rigidBody->getMotionState();
        delete m_rigidBody;
        m_rigidBody = nullptr;
    }
}

bool Bullet3RigidBody::Create(ICollider* _shape, ERigidBodyType _type)
{
    m_Type = _type;
    if (!_shape)
    {
        LOG(logError, LogPhysics, "Cannot create rigid body : shape is null");
        return false;
    }

    btCollisionShape* collisionShape = _shape->AsBullet3Collider()->GetShape();
    _shape->SetRigidbody(this);
    if (!collisionShape)
    {
        LOG(logError, LogPhysics, "Cannot create rigid body : collision shape is null");
        return false;
    }

    btScalar  mass = m_Mass;
    btVector3 inertia(0, 0, 0);

    switch (_type)
    {
        case ERigidBodyType::Static   : mass = 0.0f; break;
        case ERigidBodyType::Dynamic  : collisionShape->calculateLocalInertia(mass, inertia); break;
        case ERigidBodyType::Kinematic: mass = 0.0f; break;
        default:
            LOG(logWarning, LogPhysics, "Unknown rigid body type, defaulting to Dynamic");
            collisionShape->calculateLocalInertia(mass, inertia);
            break;
    }

    btDefaultMotionState*                    motionState = new btDefaultMotionState(btTransform::getIdentity());
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, inertia);

    m_rigidBody = new btRigidBody(rigidBodyCI);

    if (_type == ERigidBodyType::Static) m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
    else if (_type == ERigidBodyType::Kinematic) m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

    PhysicsManager::Instance()->RegisterRigidBody(this);
    return true;
}

void Bullet3RigidBody::Update()
{
    if (m_rigidBody == nullptr) return;
    MirageEngine::GameObject* owner = GetOwner();
    if (owner == nullptr) return;
    btVector3 position = m_rigidBody->getWorldTransform().getOrigin();
    btQuaternion rotation = m_rigidBody->getWorldTransform().getRotation();

    owner->Transform.SetPosition(MirageEngine::Core::Math::Vector3(position.getX(), position.getY(), position.getZ()));
    owner->Transform.SetRotation(Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w()));
    m_Velocity = MirageEngine::Core::Math::Vector3(m_rigidBody->getLinearVelocity().getX(), m_rigidBody->getLinearVelocity().getY(), m_rigidBody->getLinearVelocity().getZ());
}

void Bullet3RigidBody::OnTransformChanged(MirageEngine::Core::Components::Transform* _newTransform)
{
    if (!m_rigidBody) return;

    btTransform btTrans;
    btTrans.setOrigin(btVector3(_newTransform->GetPosition().X, _newTransform->GetPosition().Y, _newTransform->GetPosition().Z));
    btTrans.setRotation(btQuaternion(_newTransform->GetRotation().X, _newTransform->GetRotation().Y, _newTransform->GetRotation().Z, _newTransform->GetRotation().W));

    m_rigidBody->setWorldTransform(btTrans);
    m_rigidBody->getMotionState()->setWorldTransform(btTrans);
}

void Bullet3RigidBody::SetType(ERigidBodyType _type)
{
    if (!m_rigidBody) return;
    m_Type = _type;

    m_rigidBody->setActivationState(DISABLE_SIMULATION);

    int flags = m_rigidBody->getCollisionFlags() & ~(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT);

    switch (m_Type)
    {
        case ERigidBodyType::Static:
            flags |= btCollisionObject::CF_STATIC_OBJECT;
            m_rigidBody->setMassProps(0.0f, btVector3(0, 0, 0));
            break;
        case ERigidBodyType::Kinematic:
            flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
            m_rigidBody->setMassProps(0.0f, btVector3(0, 0, 0));
            break;
        case ERigidBodyType::Dynamic: m_rigidBody->setMassProps(m_Mass, btVector3(0, 0, 0)); break;
    }

    m_rigidBody->setCollisionFlags(flags);
    m_rigidBody->updateInertiaTensor();

    m_rigidBody->setActivationState(m_Type == ERigidBodyType::Dynamic ? ACTIVE_TAG : DISABLE_DEACTIVATION);
}

void Bullet3RigidBody::SetMass(float _mass)
{
    if (m_rigidBody == nullptr) return;
    if (m_Type == ERigidBodyType::Static || m_Type == ERigidBodyType::Kinematic)
    {
        LOG(logWarning, LogPhysics, "Cannot set mass for static or kinematic rigid body");
        return;
    }
    m_Mass = _mass;
    PhysicsManager::Instance()->UnregisterRigidBody(this);
    btVector3 inertia(0, 0, 0);
    if (_mass > 0) m_rigidBody->getCollisionShape()->calculateLocalInertia(_mass, inertia);

    m_rigidBody->setMassProps(_mass, inertia);
    m_rigidBody->updateInertiaTensor();
    PhysicsManager::Instance()->RegisterRigidBody(this);
}

void Bullet3RigidBody::SetNewCollisionShape(ICollider* _newShape)
{
    if (!m_rigidBody || !_newShape) return;

    // Save current stats
    btVector3 linearVel  = m_rigidBody->getLinearVelocity();
    btVector3 angularVel = m_rigidBody->getAngularVelocity();
    float     mass       = 1.0f / m_rigidBody->getInvMass();

    btCollisionShape* newShape = _newShape->AsBullet3Collider()->GetShape();
    m_rigidBody->setCollisionShape(newShape); // Set new shape

    // Calculate again new velocity
    if (mass > 0)
    {
        btVector3 inertia(0, 0, 0);
        newShape->calculateLocalInertia(mass, inertia);
        m_rigidBody->setMassProps(mass, inertia);
    }

    // Restore velocity
    m_rigidBody->setLinearVelocity(linearVel);
    m_rigidBody->setAngularVelocity(angularVel);

    m_rigidBody->activate(true);
}
