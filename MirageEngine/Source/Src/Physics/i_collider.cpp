#include <Physics/i_collider.h>
#include <Physics/i_rigidbody.h>

using namespace MirageEngine::Core::Physics;

bool ICollider::SetRigidbody(std::shared_ptr<IRigidBody> _rigidbody)
{
    if (m_RigidBody == _rigidbody)
    {
        LOG(logWarning, LogCat::System, "Trying to set the rigidbody with the same rigidbody");
        return false;
    }
    else if (m_RigidBody != nullptr) { m_RigidBody->RemoveCollider(this); }
    m_RigidBody = _rigidbody;
    return true;
}
