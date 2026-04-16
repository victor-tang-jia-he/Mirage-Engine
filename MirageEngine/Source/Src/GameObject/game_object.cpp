#include <GameObject/game_object.h>
#include <Components/component.h>
#include <Components/transform.h>
#include <atomic>
#include <Physics/i_rigidbody.h>

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Math;

static std::atomic<uint64_t> s_NextID = 1;

uint64_t GenerateUniqueID() {
	return s_NextID++;
}

MirageEngine::GameObject::GameObject(std::shared_ptr<GameObject> _parent, std::string _name, bool _isStatic, bool _isActive)
	: m_Parent(_parent),
	m_Name(_name),
	m_bIsStatic(_isStatic),
	m_bIsActive(_isActive)
{
	m_ID = GenerateUniqueID();
}

MirageEngine::GameObject::~GameObject() { m_Components.clear(); }

void MirageEngine::GameObject::AddChild(std::shared_ptr<GameObject> _child, bool _isStatic, bool _isActive)
{
	//if (_child->GetParent() != nullptr)
	//	_child->GetParent()->RemoveChild(_child);

	//_child->SetParent(this);
	m_Children.push_back(_child);

	_child->m_bIsStatic = _isStatic;
	_child->m_bIsActive = _isActive;
}

//MirageEngine::GameObject::~GameObject()
//{
//	for (auto& component : m_Components)
//	{
//		delete component;
//	}
//	m_Components.clear();
//	for (auto& child : m_Children)
//	{
//		delete child;
//	}
//	m_Children.clear();
//}

void MirageEngine::GameObject::SetActive(bool _isActive)
{
	if (_isActive)
	{
		m_bIsActive = true;
		std::cout << "GameObject is now active." << std::endl;
	}
	else
	{
		m_bIsActive = false;
		std::cout << "GameObject is now inactive." << std::endl;
	}
}

void MirageEngine::GameObject::RemoveComponent(std::shared_ptr<MirageEngine::Core::Components::Component> _component)
{
	m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), _component), m_Components.end());
}

void MirageEngine::GameObject::RemoveChild(std::shared_ptr<GameObject> _child)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), _child), m_Children.end());
}

void MirageEngine::GameObject::SetRotation(MirageEngine::Core::Math::Quaternion _rotation)
{
    Transform.SetRotation(_rotation);
    Transform.GetMatrix();
	if (auto rb = GetComponent<MirageEngine::Core::Physics::IRigidBody>()) rb->OnTransformChanged(&Transform);

}

void MirageEngine::GameObject::SetPosition(MirageEngine::Core::Math::Vector3 _pos)
{
	Transform.SetPosition(_pos);
    Transform.GetMatrix();
        if (auto rb = GetComponent<MirageEngine::Core::Physics::IRigidBody>()) rb->OnTransformChanged(&Transform);

}

void MirageEngine::GameObject::SetScale(MirageEngine::Core::Math::Vector3 _scale)
{
    Transform.SetScale(_scale);
    Transform.GetMatrix();
	if (auto rb = GetComponent<MirageEngine::Core::Physics::IRigidBody>()) rb->OnTransformChanged(&Transform);

}

void MirageEngine::GameObject::SetPosition(float _x, float _y, float _z) { SetPosition(MirageEngine::Core::Math::Vector3(_x, _y, _z)); }
void MirageEngine::GameObject::SetScale(float _x, float _y, float _z) { SetScale(MirageEngine::Core::Math::Vector3(_x, _y, _z)); }

void MirageEngine::GameObject::SetDirty(bool _isDirty)
{
	m_bIsDirty = _isDirty;
	if (_isDirty)
	{
		Transform.SetDirty(true);
	}
}
