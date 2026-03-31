#include <Components/camera.h>
#include <Components/input_system.h>
#include <Events/event.h>
#include <iostream>
#include <time_manager.h>

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::Math;

Camera::Camera(unsigned int _width, unsigned int _height,
	float _fov, float _near, float _far, const Vector3 _position)
	:m_Width(_width), m_Height(_height), m_Fov(_fov), m_Near(_near), m_Far(_far)
{
	m_transform.SetPosition(_position);
}

void Camera::BindInput(InputMapping* map)
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
