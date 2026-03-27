#include <camera.h>
#include <input_system.h>
#include <event.h>
#include <iostream>

using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::Math;

void MirageEngine::Core::Components::Camera::BindInput(MirageEngine::Core::Input::InputMapping* map)
{
	if (map == nullptr) return;
	map->Map[GLFW_KEY_W].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() + this->Transform().Forward() * 0.1f);
			}
		}));
	map->Map[GLFW_KEY_A].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() + this->Transform().Right() * 0.1f);
			}
		}));
	map->Map[GLFW_KEY_S].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() - this->Transform().Forward() * 0.1f);
			}
		}));
	map->Map[GLFW_KEY_D].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() - this->Transform().Right() * 0.1f);
			}
		}));
	map->Map[GLFW_KEY_Q].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				Vector3 vec = this->Transform().Up();
				this->Transform().SetRotation(this->Transform().GetRotation() * Quaternion(vec,0.05f));
			}
		}));
	map->Map[GLFW_KEY_E].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				Vector3 vec = this->Transform().Up();
				this->Transform().SetRotation(this->Transform().GetRotation() * Quaternion(vec, -0.05f));
			}
		}));
}
