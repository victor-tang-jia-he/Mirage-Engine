#include <Components/camera.h>
#include <Components/input_system.h>
#include <Events/event.h>
#include <iostream>
#include <time_manager.h>

using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::Math;
using namespace MirageEngine;

void MirageEngine::Core::Components::Camera::BindInput(MirageEngine::Core::Input::InputMapping* map)
{
	if (map == nullptr) return;
	map->Map[GLFW_KEY_W].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() + this->Transform().Forward() * 1.f * TimeManager::GetDeltaTime());
			}
		}));
	map->Map[GLFW_KEY_A].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() + this->Transform().Right() * 1.f * TimeManager::GetDeltaTime());
			}
		}));
	map->Map[GLFW_KEY_S].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() - this->Transform().Forward() * 1.f * TimeManager::GetDeltaTime());
			}
		}));
	map->Map[GLFW_KEY_D].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				this->Transform().SetPosition(this->Transform().GetPosition() - this->Transform().Right() * 1.f * TimeManager::GetDeltaTime());
			}
		}));
	map->Map[GLFW_KEY_Q].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				Vector3 vec = this->Transform().Up();
				this->Transform().SetRotation(this->Transform().GetRotation() * Quaternion(vec,1.f * TimeManager::GetDeltaTime()));
			}
		}));
	map->Map[GLFW_KEY_E].dispatcher.Add(new Event<CallbackInputInfo>("InputSystem", [this](CallbackInputInfo info) 
		{
			if (info.Held)
			{
				Vector3 vec = this->Transform().Up();
				this->Transform().SetRotation(this->Transform().GetRotation() * Quaternion(vec, -1.f * TimeManager::GetDeltaTime()));
			}
		}));
}
