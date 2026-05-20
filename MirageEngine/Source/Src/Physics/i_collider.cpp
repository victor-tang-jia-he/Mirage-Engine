#include <Physics/i_collider.h>
#include <Physics/i_rigidbody.h>

#include <Logs/log.h>

using namespace MirageEngine::Core::Physics;

bool ICollider::SetRigidbody(std::shared_ptr<IRigidBody> _rigidbody)
{
    if (!_rigidbody)
        return false;

    if (p_RigidBody.lock() == _rigidbody)
    {
        LOG(logWarning, LogCat::System, "Trying to set the rigidbody with the same rigidbody");
        return false;
    }
    else if (!p_RigidBody.expired()) 
    { 
        p_RigidBody.lock()->RemoveCollider(shared_from_this());
    }

    p_RigidBody = _rigidbody;
    return true;
}
