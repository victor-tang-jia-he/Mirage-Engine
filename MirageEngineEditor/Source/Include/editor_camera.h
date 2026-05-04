#pragma once

#include "Camera/i_camera.h"

namespace MirageEngine::Core::Input
{
    struct InputMapping;
    struct CallbackInputInfo;
} // namespace Input

namespace MirageEditor
{
	class EditorCamera : public MirageEngine::Core::ICamera
	{
    private:
        float m_CameraVelocity = 1.f;
        float m_DefaultVelocity = 1.f;
        float m_RunVelocity = 5.f;

        float m_CameraSensitivity = 1.0f;

    public:
        // Inputs
        void BindDefaultInput();

    private:

        void Move(MirageEngine::Core::Input::CallbackInputInfo info);
        void View(MirageEngine::Core::Input::CallbackInputInfo info);
	};
}