#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_box_collider.h>
#include <Physics/Bullet3/bullet3_sphere_collider.h>
#include <Physics/Bullet3/bullet3_capsule_collider.h>
#include <Components/transform.h>
#include <Physics/physics_manager.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogBullet3Rigidbody)

using namespace MirageEngine::Core::Physics;

Bullet3RigidBody::~Bullet3RigidBody()
{
    PhysicsManager::Instance().UnregisterRigidBody(this);
    if (m_rigidBody)
    {
        delete m_rigidBody->getMotionState();
    }
    if (m_Compound)
        m_Compound.reset();
}

bool Bullet3RigidBody::Create(std::shared_ptr<ICollider> _shape, ERigidBodyType _type, MirageEngine::Core::Components::Transform& _newTransform)
{
    m_Type = _type;
    m_bIsDirty = false;

    if (!_shape)
    {
        LOG(logError, LogBullet3Rigidbody, "Cannot create rigid body: shape is null.");
        return false;
    }

    if (m_rigidBody)
    {
        LOG(logError, LogBullet3Rigidbody, "Rigid body already created.");
        return false;
    }

    btCollisionShape* collisionShape = _shape->AsBullet3Collider()->GetShape();
    if (!collisionShape)
    {
        LOG(logError, LogBullet3Rigidbody, "Collision shape is null.");
        return false;
    }

    _shape->SetRigidbody(shared_from_this());

    // -------------------------
    // Compound setup (correct usage)
    // -------------------------
    m_Compound = std::make_shared<btCompoundShape>();

    auto scale = _shape->GetScale();
    collisionShape->setLocalScaling(btVector3(scale.X, scale.Y, scale.Z));

    m_CurrentRigidBodyShapesBind.push_back(_shape.get());

    m_Compound->addChildShape(
        _shape->AsBullet3Collider()->GetTransform(),
        collisionShape
    );

    // -------------------------
    // Mass / inertia
    // -------------------------
    btScalar mass = m_Mass;
    btVector3 inertia(0, 0, 0);

    switch (_type)
    {
    case ERigidBodyType::Static:
        mass = 0.0f;
        break;

    case ERigidBodyType::Dynamic:
        collisionShape->calculateLocalInertia(mass, inertia);
        break;

    case ERigidBodyType::Kinematic:
        mass = 0.0f;
        inertia.setValue(0, 0, 0);
        break;

    default:
        LOG(logWarning, LogBullet3Rigidbody, "Unknown rigid body type, defaulting to Dynamic.");
        collisionShape->calculateLocalInertia(mass, inertia);
        break;
    }

    btTransform btTrans;
    btTrans.setOrigin(btVector3(_newTransform.GetPosition().X, _newTransform.GetPosition().Y, _newTransform.GetPosition().Z));
    btTrans.setRotation(btQuaternion(_newTransform.GetRotation().X, _newTransform.GetRotation().Y, _newTransform.GetRotation().Z, _newTransform.GetRotation().W));
    btDefaultMotionState* motionState = new btDefaultMotionState(btTrans);

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        mass,
        motionState,
        m_Compound.get(),
        inertia
    );

    m_rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);

    // -------------------------
    // Collision flags
    // -------------------------
    if (_type == ERigidBodyType::Static)
        m_rigidBody->setCollisionFlags(
            m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

    else if (_type == ERigidBodyType::Kinematic)
        m_rigidBody->setCollisionFlags(
            m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

    // -------------------------
    // Register in physics world
    // -------------------------
    PhysicsManager::Instance().RegisterRigidBody(
        shared_from_this(),
        m_CollisionMask,
        m_CollisionGroup
    );

    return true;
}

void Bullet3RigidBody::UpdateTransform(MirageEngine::Core::Components::Transform& _transform)
{
    if (m_rigidBody == nullptr)
        return;
    for (ICollider* colliderShape : m_CurrentRigidBodyShapesBind)
        if (colliderShape->GetIsDirty())
        {
            m_bIsDirty = true;
        }

    // Update all compount
    if (m_bIsDirty)
        UpdateCompound();

    btVector3 velocity(0, 0, 0);
    velocity.setY(-9.81f);
    //m_rigidBody->setLinearVelocity(velocity);

    // Update GameObject
    btVector3 position = m_rigidBody->getWorldTransform().getOrigin();
    btQuaternion rotation = m_rigidBody->getWorldTransform().getRotation();

    _transform.SetPosition(MirageEngine::Core::Math::Vector3(position.getX(), position.getY(), position.getZ()));
    _transform.SetRotation(Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
    m_Velocity = MirageEngine::Core::Math::Vector3(m_rigidBody->getLinearVelocity().getX(), m_rigidBody->getLinearVelocity().getY(), m_rigidBody->getLinearVelocity().getZ());

    m_rigidBody->activate(true);
}

void Bullet3RigidBody::OnTransformChanged(MirageEngine::Core::Components::Transform& _newTransform)
{
    if (!m_rigidBody)
        return;

    btTransform btTrans;
    btTrans.setOrigin(btVector3(_newTransform.GetPosition().X, _newTransform.GetPosition().Y, _newTransform.GetPosition().Z));
    btTrans.setRotation(btQuaternion(_newTransform.GetRotation().X, _newTransform.GetRotation().Y, _newTransform.GetRotation().Z, _newTransform.GetRotation().W));

    SetScale(_newTransform.GetScale());

    m_rigidBody->setWorldTransform(btTrans);
    m_rigidBody->getMotionState()->setWorldTransform(btTrans);

    int numManifolds = PhysicsManager::Instance().GetWorldPhysics().lock()->API_Bullet3().GetWorld()->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* manifold = PhysicsManager::Instance().GetWorldPhysics().lock()->API_Bullet3().GetWorld()->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* objA = manifold->getBody0();
        const btCollisionObject* objB = manifold->getBody1();

        if (objA == m_rigidBody.get() || objB == m_rigidBody.get())
        {
            const btCollisionObject* other = (objA == m_rigidBody.get()) ? objB : objA;
            const_cast<btCollisionObject*>(other)->activate(true);
        }
    }
}

void Bullet3RigidBody::SetScale(const Vector3& scale)
{
    if (m_CurrentScale == scale)
        return;
    m_CurrentScale = scale;
    m_bIsDirty = true;
}

void Bullet3RigidBody::SetType(ERigidBodyType _type)
{
    if (!m_rigidBody)
        return;
    m_Type = _type;

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
        case ERigidBodyType::Dynamic:
            m_rigidBody->setMassProps(m_Mass, btVector3(0, 0, 0));
            break;
    }

    m_rigidBody->setCollisionFlags(flags);
    m_rigidBody->updateInertiaTensor();

    m_rigidBody->setActivationState(m_Type == ERigidBodyType::Dynamic ? ACTIVE_TAG : ISLAND_SLEEPING);
}

void Bullet3RigidBody::SetMass(float _mass)
{
    if (m_rigidBody == nullptr)
        return;

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

void Bullet3RigidBody::AddCollider(std::shared_ptr<ICollider> _newShape)
{
    if (!_newShape.get()) return;
    if (!_newShape->SetRigidbody(shared_from_this())) return;
    m_CurrentRigidBodyShapesBind.push_back(_newShape.get());
    m_bIsDirty = true;
}

void Bullet3RigidBody::RemoveCollider(ICollider* _shape)
{
    auto it = std::find(m_CurrentRigidBodyShapesBind.begin(), m_CurrentRigidBodyShapesBind.end(), _shape);
    if (it == m_CurrentRigidBodyShapesBind.end())
        return;
    _shape->SetRigidbody(nullptr);
    m_CurrentRigidBodyShapesBind.erase(it);
    m_bIsDirty = true;
}

void Bullet3RigidBody::UpdateCompound()
{
    using namespace MirageEngine::Core::Math;
    if (!m_rigidBody || !m_Compound)
        return;

    PhysicsManager::Instance().UnregisterRigidBody(this); // Un register the current rigidbody from the world

    m_Compound.reset();                                   // Destroy old one
    m_Compound = std::make_shared<btCompoundShape>();     // Create a new one

    for (ICollider* collider : m_CurrentRigidBodyShapesBind)
    {
        btCollisionShape* collisionShape = collider->AsBullet3Collider()->GetShape();
        IBulletColliderData* collisionShapeData = collider->AsBullet3Collider();
        if (!collisionShape)
            continue;
        if (collider->GetIsDirty())
        {
            delete collisionShape;
            collider->Create();                                         // Create a new shape
            collider->SetIsDirty(false);
        }
        collisionShape = collider->AsBullet3Collider()->GetShape(); // Get new shape created
        Vector3 finalScale = collider->GetScale() * m_CurrentScale;
        collisionShape->setLocalScaling(btVector3(finalScale.X, finalScale.Y, finalScale.Z));
        // Add to compound
        m_Compound->addChildShape(collisionShapeData->GetTransform(), collisionShape); // Getting new transform
    }
    float mass = (m_rigidBody->getInvMass() == 0.0f) ? 0.0f : 1.0f / m_rigidBody->getInvMass();

    // Reset collision flags
    int flags = m_rigidBody->getCollisionFlags();
    flags &= ~btCollisionObject::CF_NO_CONTACT_RESPONSE;
    m_rigidBody->setCollisionFlags(flags);

    switch (m_CollisionRespond)
    {
    case ECollisionResponse::Overlap:
        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
        break;
    case ECollisionResponse::Block:
        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
        break;
    default:
        break;
    }
    if (mass > 0.0f)
    {
        btVector3 inertia(0, 0, 0);
        m_Compound->calculateLocalInertia(mass, inertia);
        m_rigidBody->setMassProps(mass, inertia);
        m_rigidBody->updateInertiaTensor();
    }
    m_rigidBody->setCollisionShape(m_Compound.get());                                               // Set new compound to the rigidbody
    m_rigidBody->activate(true);
    if (m_CollisionRespond == ECollisionResponse::Ignore) 
        PhysicsManager::Instance().RegisterRigidBody(shared_from_this(), m_CollisionMask, ECollisionGroup::None); // Register back to the world
    else 
        PhysicsManager::Instance().RegisterRigidBody(shared_from_this(), m_CollisionMask, m_CollisionGroup);                                                       // Register back to the world
    m_bIsDirty = false;
}
