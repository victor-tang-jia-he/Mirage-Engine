#include <Components/camera.h>
#include <Components/input_system.h>
#include <Components/input_mapping.h>
#include <Events/event.h>
#include <iostream>
#include <time_manager.h>

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::Math;

Camera::Camera(unsigned int _width, unsigned int _height, float _fov, float _near, float _far, const Vector3 _position)
    : m_Width(_width)
    , m_Height(_height)
    , m_Fov(_fov)
    , m_Near(_near)
    , m_Far(_far)
{
    m_transform.SetPosition(_position);
}

void Camera::BindInput(std::shared_ptr<InputMapping> map)
{
    if (map == nullptr) return;
    map->Add(
        GLFW_KEY_W, std::make_shared<Event<CallbackInputInfo>>(
            "InputSystem",
            [this](CallbackInputInfo info)
            {
                if (info.Held) Transform().SetPosition(Transform().GetPosition() + Transform().Forward() * m_CameraVelocity * TimeManager::GetDeltaTime());
            }));

    map->Add(
        GLFW_KEY_A, std::make_shared<Event<CallbackInputInfo>>(
            "InputSystem",
            [this](CallbackInputInfo info)
            {
                if (info.Held) Transform().SetPosition(Transform().GetPosition() + Transform().Right() * m_CameraVelocity * TimeManager::GetDeltaTime());
            }));
    map->Add(
        GLFW_KEY_S, std::make_shared<Event<CallbackInputInfo>>(
            "InputSystem",
            [this](CallbackInputInfo info)
            {
                if (info.Held) Transform().SetPosition(Transform().GetPosition() - Transform().Forward() * m_CameraVelocity * TimeManager::GetDeltaTime());
            }));
    map->Add(
        GLFW_KEY_D, std::make_shared<Event<CallbackInputInfo>>(
            "InputSystem",
            [this](CallbackInputInfo info)
            {
                if (info.Held) Transform().SetPosition(Transform().GetPosition() - Transform().Right() * m_CameraVelocity * TimeManager::GetDeltaTime());
            }));
    map->Add(
        GLFW_KEY_Q, std::make_shared<Event<CallbackInputInfo>>(
            "InputSystem",
            [this](CallbackInputInfo info)
            {
                if (info.Held)
                {
                    Vector3 axis = Transform().Up();
                    Transform().SetRotation(Transform().GetRotation() * Quaternion(axis, m_CameraVelocity * TimeManager::GetDeltaTime()));
                }
            }));
    map->Add(
        GLFW_KEY_E, std::make_shared<Event<CallbackInputInfo>>(
            "InputSystem",
            [this](CallbackInputInfo info)
            {
                if (info.Held)
                {
                    Vector3 axis = Transform().Up();
                    Transform().SetRotation(Transform().GetRotation() * Quaternion(axis, -m_CameraVelocity * TimeManager::GetDeltaTime()));
                }
            }));
    map->Add(
        GLFW_KEY_SPACE, std::make_shared<Event<CallbackInputInfo>>(
            "InputSystem",
            [this](CallbackInputInfo info)
            {
                if (info.Held) Transform().SetPosition(Transform().GetPosition() + Transform().Up() * m_CameraVelocity * TimeManager::GetDeltaTime());
            }));
    map->Add(
        GLFW_KEY_LEFT_CONTROL, std::make_shared<Event<CallbackInputInfo>>(
            "InputSystem",
            [this](CallbackInputInfo info)
            {
                if (info.Held) Transform().SetPosition(Transform().GetPosition() - Transform().Up() * m_CameraVelocity * TimeManager::GetDeltaTime());
            }));
    map->Add(
        GLFW_KEY_LEFT_SHIFT, std::make_shared<Event<CallbackInputInfo>>(
            "InputSystem",
            [this](CallbackInputInfo info)
            {
                if (info.Held)
                {
                    m_CameraVelocity = 5.f;
                }
                else
                {
                    m_CameraVelocity = 1.f;
				}
            }));
}
