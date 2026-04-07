#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_collider.h>
#include <Physics/physics_manager.h>

using namespace MirageEngine::Core::Physics;

MirageEngine::Core::Physics::Bullet3RigidBody::~Bullet3RigidBody()
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

    btCollisionShape* collisionShape = _shape->API_Bullet3().GetBullet3CollisionShape();
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

void MirageEngine::Core::Physics::Bullet3RigidBody::Update()
{
    if (m_rigidBody == nullptr) return;
    MirageEngine::GameObject* owner = GetOwner();
    if (owner == nullptr) return;
    btVector3 position = m_rigidBody->getWorldTransform().getOrigin();

    owner->Transform.SetPosition(MirageEngine::Core::Math::Vector3(position.getX(), position.getY(), position.getZ()));
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

void MirageEngine::Core::Physics::Bullet3RigidBody::SetType(ERigidBodyType _type)
{
    m_Type             = _type;
    PhysicsManager* pm = PhysicsManager::Instance();
    if (m_rigidBody == nullptr) return;
    if (_type == ERigidBodyType::Static)
    {
        pm->UnregisterRigidBody(this);

        btScalar  mass = 0.0f;
        btVector3 inertia(0, 0, 0);
        m_rigidBody->setMassProps(mass, inertia);
        m_rigidBody->updateInertiaTensor();

        m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));
        m_rigidBody->setAngularVelocity(btVector3(0, 0, 0));

        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

        pm->RegisterRigidBody(this);

        m_rigidBody->getMotionState()->setWorldTransform(m_rigidBody->getWorldTransform());
    }
    else if (_type == ERigidBodyType::Kinematic)
    {
        pm->UnregisterRigidBody(this);

        btScalar  mass = 0.0f;
        btVector3 inertia(0, 0, 0);
        m_rigidBody->setMassProps(mass, inertia);
        m_rigidBody->updateInertiaTensor();

        m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));
        m_rigidBody->setAngularVelocity(btVector3(0, 0, 0));

        m_rigidBody->setCollisionFlags((m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT) | btCollisionObject::CF_KINEMATIC_OBJECT);

        pm->RegisterRigidBody(this);

        m_rigidBody->getMotionState()->setWorldTransform(m_rigidBody->getWorldTransform());
    }
    else
    {
        pm->UnregisterRigidBody(this);
        btScalar  mass = m_Mass;
        btVector3 inertia(0, 0, 0);
        m_rigidBody->setMassProps(mass, inertia);
        m_rigidBody->updateInertiaTensor();
        m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));
        m_rigidBody->setAngularVelocity(btVector3(0, 0, 0));
        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT & ~btCollisionObject::CF_KINEMATIC_OBJECT);
        pm->RegisterRigidBody(this);
        m_rigidBody->getMotionState()->setWorldTransform(m_rigidBody->getWorldTransform());
    }
}

void MirageEngine::Core::Physics::Bullet3RigidBody::SetMass(float mass)
{
    PhysicsManager::Instance()->UnregisterRigidBody(this);
    m_Mass = mass;
    if (m_rigidBody == nullptr) return;
    if (m_Type == ERigidBodyType::Static || m_Type == ERigidBodyType::Kinematic)
    {
        LOG(logWarning, LogPhysics, "Cannot set mass for static or kinematic rigid body");
        return;
    }
    btVector3 inertia(0, 0, 0);
    if (mass > 0) m_rigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);

    m_rigidBody->setMassProps(mass, inertia);
    m_rigidBody->updateInertiaTensor();
    PhysicsManager::Instance()->RegisterRigidBody(this);
}
