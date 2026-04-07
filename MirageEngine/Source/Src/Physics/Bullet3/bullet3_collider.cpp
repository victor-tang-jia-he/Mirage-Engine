#include <Physics/Bullet3/bullet3_collider.h>
using namespace MirageEngine::Core::Physics;

bool Bullet3Collider::Create(ECollisionShapeType shapeType)
{
    switch (shapeType)
    {
        case ECollisionShapeType::Box    : m_collisionShape = new btBoxShape(btVector3(m_Transform.GetScale().X, m_Transform.GetScale().Y, m_Transform.GetScale().Z)); break;
        case ECollisionShapeType::Sphere : m_collisionShape = new btSphereShape(btScalar(m_Radius)); break;
        case ECollisionShapeType::Capsule: m_collisionShape = new btCapsuleShape(btScalar(m_Radius), btScalar(m_Height)); break;
        case ECollisionShapeType::Mesh   : LOG(logWarning, LogCat::System, "Mesh collision isn't implemented, instead use other already definited settings."); break;
        default                          : break;
    }
    if (m_collisionShape == nullptr) return false;
    return true;
}
