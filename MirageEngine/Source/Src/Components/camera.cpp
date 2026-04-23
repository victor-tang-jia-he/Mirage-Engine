#include <Components/camera.h>
#include <Input/input_system.h>
#include <Input/input_mapping.h>
#include <Events/event.h>
#include <iostream>
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

    map->RegisterAction("Camera132", this, &Camera::Move);

    // map->Add(GLFW_MOUSE_BUTTON_2, this, &Camera::View);
    // map->Add(
    //     GLFW_KEY_SPACE, std::make_shared<Event<CallbackInputInfo>>(
    //                         "InputSystem",
    //                         [this](CallbackInputInfo info)
    //                         {
    //                             if (info.Held) Transform().SetPosition(Transform().GetPosition() + Transform().Up() * m_CameraVelocity * TimeManager::GetDeltaTime());
    //                         }));
    //map->RegisterAction(
    //    "CameraUp", this,
    //    [this](CallbackInputInfo info)
    //    {
    //        if (info.Held) { m_CameraVelocity = 5.f; }
    //        else { m_CameraVelocity = 1.f; }
    //    });
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
    Vector2 mouseDelta = InputSystem::GetMouseDelta();

    // Sensibilit¨¦ r¨¦glable (par exemple 0.1f)
    float sensitivity = 0.1f;

    float yaw   = mouseDelta.X * sensitivity;
    float pitch = -mouseDelta.Y * sensitivity;

    Vector3 euler = Transform().GetRotation().ToEulerAngle();

    euler.Y += yaw;
    euler.X += pitch;

    // Clamp du pitch entre -89¡ã et +89¡ã (en radians si votre moteur utilise des radians)
    const float maxPitch = 89.0f * (3.14159f / 180.0f);
    euler.X              = std::clamp(euler.X, -maxPitch, maxPitch);

    Transform().SetRotation(Quaternion(euler));
}
