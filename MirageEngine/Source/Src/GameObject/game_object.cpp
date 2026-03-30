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
	auto mfzransform = AddComponent(new MirageEngine::Core::Components::Transform());

}

GameObject::GameObject(GameObject* _parent, std::string _name, bool _isStatic)
	: m_Parent(_parent),
	m_Name(_name),
	m_bIsStatic(_isStatic)
{
	AddComponent(new MirageEngine::Core::Components::Transform());
}

GameObject::GameObject(GameObject* _parent, std::string _name, bool _isStatic, bool _isActive)
	: m_Parent(_parent),
	m_Name(_name),
	m_bIsStatic(_isStatic),
	m_bIsActive(_isActive)
{
}

GameObject::~GameObject()
{
	AddComponent(new MirageEngine::Core::Components::Transform(Vector3{ 0,0,0 }, Quaternion{ 0,0,0 }, Vector3{ 1,1,1 }));
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

Component* GameObject::AddComponent(Component* _component)
{
	for (Component* composent : m_Components)
	{
		if (composent->GetName() == _component->GetName())
		{
			return;
			// throw(std::runtime_error("Component with the same name already exists."));
		}
	}

	m_Components.push_back(_component);
	return _component;
}

void GameObject::RemoveComponent(Component* _component)
{
	m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), _component), m_Components.end());
}