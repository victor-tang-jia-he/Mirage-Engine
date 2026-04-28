#include <Components/camera.h>
#include <Input/input_system.h>
#include <Input/input_mapping.h>
#include <Events/event.h>
#include <iostream>
#include <algorithm>
#include <time_manager.h>
#include <GLFW/glfw3.h>

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

    map->RegisterAxis2D("Camera", GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_W, GLFW_KEY_S);
    map->RegisterAxis1D("Camera", GLFW_KEY_LEFT_CONTROL, GLFW_KEY_SPACE);
    map->RegisterInput("Run", GLFW_KEY_LEFT_SHIFT);
    map->RegisterInput("CameraView", GLFW_MOUSE_BUTTON_2);
    std::function<void(CallbackInputInfo)> func = [this](CallbackInputInfo info)
    {
        if (info.Held) { m_CameraVelocity = 5.f; }
        else { m_CameraVelocity = 1.f; }
    };

    map->RegisterAction("Run", func);
    map->RegisterAction("Camera", this, &Camera::Move);
    map->RegisterAction("CameraView", this, &Camera::View);
}

void Camera::Move(Input::CallbackInputInfo info)
{
    if (info.Held)
    {
        Transform().SetPosition(Transform().GetPosition() - Transform().Up() * info.Axis1D * m_CameraVelocity * TimeManager::GetDeltaTime());

        Vector3 dir = Transform().Forward() * info.Axis2D.Y + Transform().Left() * info.Axis2D.X;
        Transform().SetPosition(Transform().GetPosition() + dir * m_CameraVelocity * TimeManager::GetDeltaTime());
    }
}

void Camera::View(Input::CallbackInputInfo info)
{
    if (!info.Held) return;
    Vector2 mouseDelta = InputSystem::GetMouseDelta();
    float sensitivity = -0.1f;

    m_CameraPitch = m_CameraPitch + ToRad(mouseDelta.Y * sensitivity);
    m_CameraYaw   = m_CameraYaw + ToRad(mouseDelta.X * sensitivity);

    const float maxPitch = ToRad(89);
    m_CameraPitch        = std::clamp(m_CameraPitch, -maxPitch, maxPitch);

    Quaternion rotationX = Quaternion({ 1.f, 0.f, 0 }, m_CameraPitch);
    Quaternion rotationY = Quaternion({ 0.f, 1.f, 0 }, m_CameraYaw);

    Transform().SetRotation(rotationY * rotationX);
}
