#include <GameObject/game_object.h>

using namespace MirageEngine;

GameObject::GameObject(std::string _name)
	: m_Name(_name)
{
	
}

GameObject::~GameObject()
{
}

void GameObject::SetActive(bool _isActive)
{
	if (_isActive)
	{
		std::cout << "GameObject is now active." << std::endl;
	}
	else
	{
		std::cout << "GameObject is now inactive." << std::endl;
	}
}