#include <GameObject/game_object.h>
#include <Components/component.h>
#include <Components/transform.h>
#include <Physics/i_rigidbody.h>

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Math;

MirageEngine::GameObject::GameObject(GameObject* _parent, std::string _name, bool _isStatic, bool _isActive)
	: m_Parent(_parent),
	m_Name(_name),
	m_bIsStatic(_isStatic),
	m_bIsActive(_isActive)
{
	IsRoot();
}

void MirageEngine::GameObject::AddChild(GameObject* _child, bool _isStatic, bool _isActive)
{
	if (_child->GetParent() != nullptr)
		_child->GetParent()->RemoveChild(_child);

	_child->SetParent(this);
	m_Childrens.push_back(_child);

	m_bIsStatic = _isStatic;
	m_bIsActive = _isActive;
}

MirageEngine::GameObject::~GameObject()
{
	for (auto& component : m_Components)
	{
		delete component;
	}
	m_Components.clear();
	for (auto& child : m_Childrens)
	{
		delete child;
	}
	m_Childrens.clear();
}

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

void MirageEngine::GameObject::RemoveComponent(MirageEngine::Core::Components::Component* _component)
{
	m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), _component), m_Components.end());
}

bool MirageEngine::GameObject::IsRoot()
{
	if (m_Parent == nullptr)
	{
		m_bIsRoot = true;
	}
	else
	{
		m_bIsRoot = false;
	}
	return m_bIsRoot;
}

void MirageEngine::GameObject::SetParent(GameObject* _parent)
{
	m_Parent = _parent;
}

void MirageEngine::GameObject::RemoveChild(GameObject* _child)
{
	m_Childrens.erase(std::remove(m_Childrens.begin(), m_Childrens.end(), _child), m_Childrens.end());
}

void MirageEngine::GameObject::SetPosition(MirageEngine::Core::Math::Vector3 _pos)
{
    Transform.SetPosition(_pos);

    if (auto rb = GetComponent<MirageEngine::Core::Physics::IRigidBody>()) rb->OnTransformChanged(&Transform);

}

void MirageEngine::GameObject::SetPosition(float _x, float _y, float _z) { SetPosition(MirageEngine::Core::Math::Vector3(_x, _y, _z)); }
