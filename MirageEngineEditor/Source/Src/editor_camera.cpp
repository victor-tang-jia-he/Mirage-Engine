#include <editor_camera.h>

#include <Input/input_system.h>
#include <Input/input_mapping.h>
#include <Events/event.h>
#include <time_manager.h>
#include <GLFW/glfw3.h>

using namespace MirageEngine;
using namespace MirageEngine::Core;

void MirageEditor::EditorCamera::BindDefaultInput()
{
    std::shared_ptr<Input::InputMapping> map = MirageEngine::Core::Input::InputSystem::GetCurrentMapping();

    if (map == nullptr) return;

    map->RegisterAxis2D("Camera", GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_W, GLFW_KEY_S);
    map->RegisterAxis1D("Camera", GLFW_KEY_LEFT_CONTROL, GLFW_KEY_SPACE);
    map->RegisterInput("Run", GLFW_KEY_LEFT_SHIFT);
    map->RegisterInput("CameraView", GLFW_MOUSE_BUTTON_2);

    std::function<void(Input::CallbackInputInfo)> func = [this](Input::CallbackInputInfo info)
        {
            if (info.Held) 
            {
                m_CameraVelocity = m_RunVelocity;
            }
            else 
            {
                m_CameraVelocity = m_DefaultVelocity;
            }
        };

    map->RegisterAction("Run", func);
    map->RegisterAction("Camera", this, &EditorCamera::Move);
    map->RegisterAction("CameraView", this, &EditorCamera::View);
}

void MirageEditor::EditorCamera::Move(Input::CallbackInputInfo _info)
{
    if (_info.Held)
    {
        Math::Vector3 up = Math::Vector3(0, 1, 0).Rotate(m_Pitch, m_Yaw, 0);
        SetPosition(GetPosition() - up * _info.Axis1D * m_CameraVelocity * TimeManager::GetDeltaTime());

        Math::Vector3 forward = Math::Vector3(0, 0, -1).Rotate(m_Pitch, m_Yaw, 0);
        Math::Vector3 left = Math::Vector3(1, 0, 0).Rotate(m_Pitch, m_Yaw, 0);
        Math::Vector3 direction = forward * _info.Axis2D.Y + left * _info.Axis2D.X;
        SetPosition(GetPosition() + direction * m_CameraVelocity * TimeManager::GetDeltaTime());
    }
}

void MirageEditor::EditorCamera::View(Input::CallbackInputInfo _info)
{
    if (!_info.Held) return;

    Math::Vector2 mouseDelta = Input::InputSystem::GetMouseDelta();
    float sensitivity = -0.1f;

    m_Pitch = m_Pitch + ToRad(mouseDelta.Y * sensitivity);
    m_Yaw = m_Yaw + ToRad(mouseDelta.X * sensitivity);

    const float maxPitch = ToRad(89);
    m_Pitch = std::clamp(m_Pitch, -maxPitch, maxPitch);
}