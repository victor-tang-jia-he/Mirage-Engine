#include <camera.h>
#include <input_system.h>
#include <event.h>
#include <iostream>

using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Input;

void MirageEngine::Core::Components::Camera::BindInput(MirageEngine::Core::Input::InputMapping* map)
{
	if (map == nullptr) return;
	map->Map[GLFW_KEY_W].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Pressed)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() + this->Transform().Forward());
				std::cout << this->Transform().GetPosition() << std::endl;
			}
		}));
	map->Map[GLFW_KEY_A].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Pressed)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() + this->Transform().Left());
			}
		}));
	map->Map[GLFW_KEY_S].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Pressed)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() - this->Transform().Forward());
			}
		}));
	map->Map[GLFW_KEY_D].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Pressed)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() - this->Transform().Left());
			}
		}));
}
