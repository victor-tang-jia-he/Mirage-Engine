#include <GameObject/game_object.h>
#include <Components/component.h>
#include <Components/transform.h>

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Math;

MirageEngine::GameObject::GameObject(GameObject* _parent, std::string _name)
	: m_Parent(_parent),
	m_Name(_name)
{
}

MirageEngine::GameObject::GameObject(GameObject* _parent, std::string _name, bool _isStatic)
	: m_Parent(_parent),
	m_Name(_name),
	m_bIsStatic(_isStatic)
{
}

MirageEngine::GameObject::GameObject(GameObject* _parent, std::string _name, bool _isStatic, bool _isActive)
	: m_Parent(_parent),
	m_Name(_name),
	m_bIsStatic(_isStatic),
	m_bIsActive(_isActive)
{
}

void MirageEngine::GameObject::AddChild(GameObject* _child)
{
	m_Childrens.push_back(_child);
}

void MirageEngine::GameObject::AddChild(GameObject* _child, bool _isStatic)
{
	m_Childrens.push_back(_child);
}

void MirageEngine::GameObject::AddChild(GameObject* _child, bool _isStatic, bool _isActive)
{
	m_Childrens.push_back(_child);
}

MirageEngine::GameObject::~GameObject()
{
}

void MirageEngine::GameObject::SetActive(bool _isActive)
{
	if (_isActive)
	{
		std::cout << "GameObject is now active." << std::endl;
		// TODO
	}
	else
	{
		std::cout << "GameObject is now inactive." << std::endl;
		// TODO
	}
}

void MirageEngine::GameObject::RemoveComponent(MirageEngine::Core::Components::Component* _component)
{
	m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), _component), m_Components.end());
}

MirageEngine::Core::Components::Component* MirageEngine::GameObject::GetComponent(std::string _name)
{
	for (MirageEngine::Core::Components::Component* composent : m_Components)
	{
		// Transform Name by ID
		if (composent->GetName() == _name)
		{
			return composent;
		}
	}
	return nullptr;
}