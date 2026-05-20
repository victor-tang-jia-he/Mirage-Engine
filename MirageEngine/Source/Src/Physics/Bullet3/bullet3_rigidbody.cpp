#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_box_collider.h>
#include <Physics/Bullet3/bullet3_sphere_collider.h>
#include <Physics/Bullet3/bullet3_capsule_collider.h>
#include <Physics/Bullet3/bullet3_world_physics.h>
#include <Physics/physics_manager.h>

#include <Components/transform.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogBullet3Rigidbody)

using namespace MirageEngine::Core::Physics;

Bullet3RigidBody::~Bullet3RigidBody()
{
	PhysicsManager::Instance().UnregisterRigidBody(this);
	if (m_RigidBody)
	{
		delete m_RigidBody->getMotionState();
	}
	if (m_Compound)
		m_Compound.reset();
}

bool Bullet3RigidBody::Create(std::shared_ptr<ICollider> _shape, ERigidBodyType _type, MirageEngine::Core::Components::Transform& _newTransform)
{
	m_Type = _type;
	m_bIsDirty = true;

	if (!_shape)
	{
		LOG(logError, LogBullet3Rigidbody, "Cannot create rigid body: shape is null.");
		return false;
	}

	if (m_RigidBody)
	{
		LOG(logError, LogBullet3Rigidbody, "Rigid body already created.");
		return false;
	}

	btCollisionShape* collisionShape = _shape->As<IBulletColliderData>()->GetShape();
	if (!collisionShape)
	{
		LOG(logError, LogBullet3Rigidbody, "Collision shape is null.");
		return false;
	}

	_shape->SetRigidbody(shared_from_this());

	// -------------------------
	// Compound setup 
	// -------------------------
	m_Compound = std::make_shared<btCompoundShape>();

	auto scale = _shape->GetScale();
	collisionShape->setLocalScaling(btVector3(scale.X, scale.Y, scale.Z));

	m_CurrentRigidBodyShapesBind.push_back(_shape);

	m_Compound->addChildShape(
		_shape->As<IBulletColliderData>()->GetTransform(),
		collisionShape
	);

	// -------------------------
	// Mass / inertia
	// -------------------------
	btScalar mass = m_Mass;
	btVector3 inertia(0, 0, 0);

	switch (_type)
	{
	case ERigidBodyType::STATIC:
		mass = 0.0f;
		break;

	case ERigidBodyType::DYNAMIC:
		collisionShape->calculateLocalInertia(mass, inertia);
		break;

	case ERigidBodyType::KINEMATIC:
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

	m_RigidBody = std::make_shared<btRigidBody>(rigidBodyCI);

	// -------------------------
	// Collision flags
	// -------------------------
	if (_type == ERigidBodyType::STATIC)
		m_RigidBody->setCollisionFlags(
			m_RigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

	else if (_type == ERigidBodyType::KINEMATIC)
		m_RigidBody->setCollisionFlags(
			m_RigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

	// -------------------------
	// Register in physics world
	// -------------------------
	PhysicsManager::Instance().RegisterRigidBody(
		shared_from_this(),
		m_CollisionMask,
		m_CollisionGroup
	);

	m_RigidBody->setActivationState(DISABLE_DEACTIVATION);
	return true;
}

void Bullet3RigidBody::UpdateTransform(MirageEngine::Core::Components::Transform& _transform)
{
	if (m_RigidBody == nullptr)
		return;
	for (std::weak_ptr<ICollider> colliderShapeWeak : m_CurrentRigidBodyShapesBind)
	{
		if (std::shared_ptr<ICollider> colliderShape = colliderShapeWeak.lock())
		{
			if (colliderShape->GetIsDirty())
			{
				m_bIsDirty = true;
			}
		}
	}

	// Update all compount
	if (m_bIsDirty)
		UpdateCompound();

	// Update GameObject
	btVector3 position = m_RigidBody->getWorldTransform().getOrigin();
	btQuaternion rotation = m_RigidBody->getWorldTransform().getRotation();

	_transform.SetPosition(MirageEngine::Core::Math::Vector3(position.getX(), position.getY(), position.getZ()));
	_transform.SetRotation(Quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
	m_Velocity = MirageEngine::Core::Math::Vector3(m_RigidBody->getLinearVelocity().getX(), m_RigidBody->getLinearVelocity().getY(), m_RigidBody->getLinearVelocity().getZ());
}

void Bullet3RigidBody::OnTransformChanged(MirageEngine::Core::Components::Transform& _newTransform)
{
	if (!m_RigidBody)
		return;


	Vector3 position = _newTransform.GetPosition();
	Quaternion rotation = _newTransform.GetRotation();

	btTransform btTrans;
	btTrans.setOrigin(btVector3(position.X, position.Y, position.Z));
	btTrans.setRotation(btQuaternion(rotation.X, rotation.Y, rotation.Z, rotation.W));
	SetScale(_newTransform.GetScale());

	m_RigidBody->setWorldTransform(btTrans);
	m_RigidBody->getMotionState()->setWorldTransform(btTrans);

	int numManifolds = PhysicsManager::Instance().GetWorldPhysics().lock()->API_Bullet3().GetWorld()->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* manifold = PhysicsManager::Instance().GetWorldPhysics().lock()->API_Bullet3().GetWorld()->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* objA = manifold->getBody0();
		const btCollisionObject* objB = manifold->getBody1();

		if (objA == m_RigidBody.get() || objB == m_RigidBody.get())
		{
			const btCollisionObject* other = (objA == m_RigidBody.get()) ? objB : objA;
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
	if (!m_RigidBody)
		return;
	m_Type = _type;

	int flags = m_RigidBody->getCollisionFlags() & ~(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT);

	switch (m_Type)
	{
	case ERigidBodyType::STATIC:
		flags |= btCollisionObject::CF_STATIC_OBJECT;
		m_RigidBody->setMassProps(0.0f, btVector3(0, 0, 0));
		break;
	case ERigidBodyType::KINEMATIC:
		flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
		m_RigidBody->setMassProps(0.0f, btVector3(0, 0, 0));
		break;
	case ERigidBodyType::DYNAMIC:
		m_RigidBody->setMassProps(m_Mass, btVector3(0, 0, 0));
		break;
	}

	m_RigidBody->setCollisionFlags(flags);
	m_RigidBody->updateInertiaTensor();

	m_RigidBody->setActivationState(m_Type == ERigidBodyType::DYNAMIC ? ACTIVE_TAG : DISABLE_DEACTIVATION);
}

void Bullet3RigidBody::SetMass(float _mass)
{
	if (m_RigidBody == nullptr)
		return;

	if (m_Type == ERigidBodyType::STATIC || m_Type == ERigidBodyType::KINEMATIC)
		return;

	m_Mass = _mass;
	btVector3 inertia(0, 0, 0);
	if (m_Mass > 0.0f)
	{
		m_Compound->calculateLocalInertia(m_Mass, inertia);
		m_RigidBody->setMassProps(m_Mass, inertia);
		m_RigidBody->updateInertiaTensor();
	}
	m_RigidBody->activate(true);
}

void Bullet3RigidBody::AddCollider(std::shared_ptr<ICollider> _newShape)
{
	if (!_newShape.get()) return;
	if (!_newShape->SetRigidbody(shared_from_this())) return;
	m_CurrentRigidBodyShapesBind.push_back(_newShape);
	m_bIsDirty = true;
}

void Bullet3RigidBody::RemoveCollider(std::shared_ptr<ICollider> _shape)
{
	auto it = std::find_if(m_CurrentRigidBodyShapesBind.begin(), m_CurrentRigidBodyShapesBind.end(),
		[&](const std::weak_ptr<ICollider>& weakCollider)
		{
			auto collider = weakCollider.lock();
			return collider && collider == _shape;
		});
	_shape->SetRigidbody(nullptr);
	m_CurrentRigidBodyShapesBind.erase(it);
	m_bIsDirty = true;
}

void Bullet3RigidBody::UpdateCompound()
{
	using namespace MirageEngine::Core::Math;
	if (!m_RigidBody || !m_Compound)
		return;

	PhysicsManager::Instance().UnregisterRigidBody(this); // Unregister the current rigidbody from the world

	m_Compound.reset();                                   // Destroy old one
	m_Compound = std::make_shared<btCompoundShape>();     // Create a new one

	for (std::weak_ptr<ICollider> colliderWeak : m_CurrentRigidBodyShapesBind)
	{
		if (std::shared_ptr<ICollider> collider = colliderWeak.lock())
		{
			btCollisionShape* collisionShape = collider->As<IBulletColliderData>()->GetShape();
			IBulletColliderData* collisionShapeData = collider->As<IBulletColliderData>();
			if (!collisionShape)
				continue;
			if (collider->GetIsDirty())
			{
				delete collisionShape;
				collider->Create();                                         // Create a new shape
				collider->SetIsDirty(false);
			}
			collisionShape = collider->As<IBulletColliderData>()->GetShape(); // Get new shape created
			Vector3 finalScale = collider->GetScale() * m_CurrentScale;
			collisionShape->setLocalScaling(btVector3(finalScale.X, finalScale.Y, finalScale.Z));
			// Add to compound
			m_Compound->addChildShape(collisionShapeData->GetTransform(), collisionShape); // Getting new transform
		}
	}
	float mass = (m_RigidBody->getInvMass() == 0.0f) ? 0.0f : 1.0f / m_RigidBody->getInvMass();

	// Reset collision flags
	int flags = m_RigidBody->getCollisionFlags();
	flags &= ~btCollisionObject::CF_NO_CONTACT_RESPONSE;
	m_RigidBody->setCollisionFlags(flags);

	switch (m_CollisionRespond)
	{
	case ECollisionResponse::OVERLAP:
		m_RigidBody->setCollisionFlags(m_RigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		break;
	case ECollisionResponse::BLOCK:
		m_RigidBody->setCollisionFlags(m_RigidBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		break;
	default:
		break;
	}
	if (mass > 0.0f)
	{
		btVector3 inertia(0, 0, 0);
		m_Compound->calculateLocalInertia(mass, inertia);
		m_RigidBody->setMassProps(mass, inertia);
		m_RigidBody->updateInertiaTensor();
	}
	m_RigidBody->setCollisionShape(m_Compound.get()); // Set new compound to the rigidbody
	m_RigidBody->activate(true);
	if (m_CollisionRespond == ECollisionResponse::IGNORE_COLLISION)
		PhysicsManager::Instance().RegisterRigidBody(shared_from_this(), m_CollisionMask, ECollisionGroup::NONE); // Register back to the world
	else
		PhysicsManager::Instance().RegisterRigidBody(shared_from_this(), m_CollisionMask, m_CollisionGroup); // Register back to the world
	m_bIsDirty = false;
}
