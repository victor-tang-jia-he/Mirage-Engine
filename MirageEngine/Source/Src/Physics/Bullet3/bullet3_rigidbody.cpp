#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_box_collider.h>
#include <Physics/Bullet3/bullet3_sphere_collider.h>
#include <Physics/Bullet3/bullet3_capsule_collider.h>
#include <Physics/physics_manager.h>

DEFINE_LOG_CATEGORY(LogBullet3Rigidbody)

using namespace MirageEngine::Core::Physics;

Bullet3RigidBody::~Bullet3RigidBody()
{
    PhysicsManager::Instance()->UnregisterRigidBody(this);
    if (m_rigidBody)
    {
        delete m_rigidBody->getMotionState();
        delete m_rigidBody;
        m_rigidBody = nullptr;
    }
    if (m_Compound) delete m_Compound;
}

bool Bullet3RigidBody::Create(ICollider* _shape, ERigidBodyType _type)
{
    m_Type     = _type;
    m_bIsDirty = false;
    if (!_shape)
    {
        LOG(logError, LogBullet3Rigidbody, "Cannot create rigid body : shape is null.");
        return false;
    }

    if (m_rigidBody)
    {
        LOG(logError, LogBullet3Rigidbody, "Cannot recreate rigid body : it is already created.");
        return false;
    }

    btCollisionShape* collisionShape = _shape->AsBullet3Collider()->GetShape();

    _shape->SetRigidbody(this);
    if (!collisionShape)
    {
        LOG(logError, LogBullet3Rigidbody, "Cannot create rigid body : collision shape is null.");
        return false;
    }
    m_Compound = new btCompoundShape;

    MirageEngine::Core::Math::Vector3 shapeScale = _shape->GetScale();
    collisionShape->setLocalScaling(btVector3(shapeScale.X, shapeScale.Y, shapeScale.Z));
    m_CurrentRigidBodyShapesBind.push_back(_shape);
    m_Compound->addChildShape(_shape->AsBullet3Collider()->GetTransform(), collisionShape);

    btScalar  mass = m_Mass;
    btVector3 inertia(0, 0, 0);

    switch (_type)
    {
        case ERigidBodyType::Static   : mass = 0.0f; break;
        case ERigidBodyType::Dynamic  : collisionShape->calculateLocalInertia(mass, inertia); break;
        case ERigidBodyType::Kinematic: mass = 0.0f; break;
        default:
            LOG(logWarning, LogBullet3Rigidbody, "Unknown rigid body type, defaulting to Dynamic.");
            collisionShape->calculateLocalInertia(mass, inertia);
            break;
    }

    btDefaultMotionState*                    motionState = new btDefaultMotionState(btTransform::getIdentity());
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, inertia);

    m_rigidBody = new btRigidBody(rigidBodyCI);

    if (_type == ERigidBodyType::Static) m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
    else if (_type == ERigidBodyType::Kinematic) m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

    PhysicsManager::Instance()->RegisterRigidBody(this, m_CollisionMask, m_CollisionGroup);
    return true;
}

void Bullet3RigidBody::Update()
{
    if (m_rigidBody == nullptr) return;
    for (ICollider* colliderShape : m_CurrentRigidBodyShapesBind)
        if (colliderShape->GetIsDirty()) { m_bIsDirty = true; }

    // Update all compount
    if (m_bIsDirty) UpdateCompound();

    // Update GameObject
    MirageEngine::GameObject* owner = GetOwner();
    if (owner == nullptr) return;
    btVector3    position = m_rigidBody->getWorldTransform().getOrigin();
    btQuaternion rotation = m_rigidBody->getWorldTransform().getRotation();

    owner->Transform.SetPosition(MirageEngine::Core::Math::Vector3(position.getX(), position.getY(), position.getZ()));
    owner->Transform.SetRotation(Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
    m_Velocity = MirageEngine::Core::Math::Vector3(m_rigidBody->getLinearVelocity().getX(), m_rigidBody->getLinearVelocity().getY(), m_rigidBody->getLinearVelocity().getZ());
}

void Bullet3RigidBody::OnTransformChanged(MirageEngine::Core::Components::Transform* _newTransform)
{
    if (!m_rigidBody) return;

    btTransform btTrans;
    btTrans.setOrigin(btVector3(_newTransform->GetPosition().X, _newTransform->GetPosition().Y, _newTransform->GetPosition().Z));
    btTrans.setRotation(btQuaternion(_newTransform->GetRotation().X, _newTransform->GetRotation().Y, _newTransform->GetRotation().Z, _newTransform->GetRotation().W));

    SetScale(_newTransform->GetScale());

    m_rigidBody->setWorldTransform(btTrans);
    m_rigidBody->getMotionState()->setWorldTransform(btTrans);

    int numManifolds = PhysicsManager::Instance()->m_worldPhysics->API_Bullet3().m_World->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold*    manifold = PhysicsManager::Instance()->m_worldPhysics->API_Bullet3().m_World->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* objA     = manifold->getBody0();
        const btCollisionObject* objB     = manifold->getBody1();

        if (objA == m_rigidBody || objB == m_rigidBody)
        {
            const btCollisionObject* other = (objA == m_rigidBody) ? objB : objA;
            const_cast<btCollisionObject*>(other)->activate(true);
        }
    }
}

void Bullet3RigidBody::SetScale(const Vector3& scale)
{
    if (m_CurrentScale == scale) return;
    m_CurrentScale = scale;
    m_bIsDirty     = true;
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
    btVector3 inertia(0, 0, 0);
    if (m_Mass > 0.0f)
    {
        m_Compound->calculateLocalInertia(m_Mass, inertia);
        m_rigidBody->setMassProps(m_Mass, inertia);
        m_rigidBody->updateInertiaTensor();
    }
    m_rigidBody->activate(true);
}

void Bullet3RigidBody::AddCollider(ICollider* _newShape)
{
    if (!_newShape) return;
    if (!_newShape->SetRigidbody(this)) return;
    m_CurrentRigidBodyShapesBind.push_back(_newShape);
    m_bIsDirty = true;
}

void Bullet3RigidBody::RemoveCollider(ICollider* _shape)
{
    auto it = std::find(m_CurrentRigidBodyShapesBind.begin(), m_CurrentRigidBodyShapesBind.end(), _shape);
    if (it == m_CurrentRigidBodyShapesBind.end()) return;
    _shape->SetRigidbody(nullptr);
    m_CurrentRigidBodyShapesBind.erase(it);
    m_bIsDirty = true;
}

void Bullet3RigidBody::UpdateCompound()
{
    using namespace MirageEngine::Core::Math;
    if (!m_rigidBody || !m_Compound) return;

    PhysicsManager::Instance()->UnregisterRigidBody(this); // Un register the current rigidbody from the world

    delete m_Compound;                                     // Destroy old one
    m_Compound = new btCompoundShape;                      // Create a new one

    for (ICollider* collider : m_CurrentRigidBodyShapesBind)
    {
        btCollisionShape*    collisionShape     = collider->AsBullet3Collider()->GetShape();
        IBulletColliderData* collisionShapeData = collider->AsBullet3Collider();
        if (!collisionShape) continue;
        if (collider->GetIsDirty())
        {
            delete collisionShape;
            collider->Create();                                     // Create a new shape
        }
        collisionShape = collider->AsBullet3Collider()->GetShape(); // Get new shape created
        Vector3 finalScale = collider->GetScale() * m_CurrentScale;
        collisionShape->setLocalScaling(btVector3(finalScale.X, finalScale.Y, finalScale.Z));
        // Add to compound
        m_Compound->addChildShape(collisionShapeData->GetTransform(), collisionShape); // Getting new transform
    }
    float mass = (m_rigidBody->getInvMass() == 0.0f) ? 0.0f : 1.0f / m_rigidBody->getInvMass();

    if (mass > 0.0f)
    {
        btVector3 inertia(0, 0, 0);
        m_Compound->calculateLocalInertia(mass, inertia);
        m_rigidBody->setMassProps(mass, inertia);
        m_rigidBody->updateInertiaTensor();
    }
    m_rigidBody->setCollisionShape(m_Compound);          // Set new compound to the rigidbody
    m_rigidBody->activate(true);
    PhysicsManager::Instance()->RegisterRigidBody(this,m_CollisionMask, m_CollisionGroup); // Register back to the world
    m_bIsDirty = false;
}
