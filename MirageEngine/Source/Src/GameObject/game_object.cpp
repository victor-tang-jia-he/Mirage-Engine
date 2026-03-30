#include <GameObject/game_object.h>
#include <Components/component.h>
#include <Components/transform.h>

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace MirageEngine::Core::Math;
using namespace MirageEngine::Core::Components;

GameObject::GameObject(GameObject* _parent, std::string _name)
	: m_Parent(_parent),
	m_Name(_name)
{
	AddComponent<Transform>();
}

GameObject::GameObject(GameObject* _parent, std::string _name, bool _isStatic)
	: m_Parent(_parent),
	m_Name(_name),
	m_bIsStatic(_isStatic)
{
	AddComponent<Transform>();
}

GameObject::GameObject(GameObject* _parent, std::string _name, bool _isStatic, bool _isActive)
	: m_Parent(_parent),
	m_Name(_name),
	m_bIsStatic(_isStatic),
	m_bIsActive(_isActive)
{
	AddComponent<Transform>();
}

GameObject::~GameObject()
{
}

void GameObject::SetActive(bool _isActive)
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

void GameObject::RemoveComponent(Component* _component)
{
	m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), _component), m_Components.end());
}

Component* GameObject::GetComponent(std::string _name)
{
	for (Component* composent : m_Components)
	{
		// Transform Name by ID
		if (composent->GetName() == _name)
		{
			return composent;
		}
	}
	return nullptr;
}