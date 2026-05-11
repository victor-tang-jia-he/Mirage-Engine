#include <Physics/i_collider.h>
#include <Physics/i_rigidbody.h>
#include <Logs/log.h>

using namespace MirageEngine::Core::Physics;

bool ICollider::SetRigidbody(IRigidBody* _rigidbody)
{
    if (m_RigidBody == _rigidbody)
    {
        LOG(logWarning, LogCat::System, "Trying to set the rigidbody with the same rigidbody");
        return false;
    }
    else if (auto rigidbody = m_RigidBody)
    {
        rigidbody->RemoveCollider(this);
    }
    m_RigidBody = _rigidbody;
    return true;
}
