#pragma once
#include <Components/transform.h>
#include "component.h"
#include <Scripting/engine_script_api.h>

namespace MirageEngine
{
    namespace Core
    {
        namespace Input
        {
            struct InputMapping;
            struct CallbackInputInfo;
        } // namespace Input

        namespace Components
        {
            class ENGINE_API Camera : public Component
            {
                private:

                    // Clipping Planes
                    float m_Near = 0.f;
                    float m_Far = 0.f;
                    unsigned int m_Width = 0;
                    unsigned int m_Height = 0;
                    float m_Fov = 90.f;
                    float m_CameraVelocity = 1.f;
                    float m_CameraYaw;
                    float m_CameraPitch;

                    MirageEngine::Core::Components::Transform m_transform;

                public:

                    Camera() = default;
                    Camera(unsigned int _width, unsigned int _height, float _fov, float _near, float _far, const MirageEngine::Core::Math::Vector3 _position);
                    ~Camera() = default;

                    entt::id_type GetType() const override;

                    MirageEngine::Core::Components::Transform& Transform() { return m_transform; }

                    MirageEngine::Core::Math::Matrix4 GetPerspective() const { return MirageEngine::Core::Math::Matrix4::GetPerspectiveMatrix(m_Width, m_Height, m_Near, m_Far, m_Fov); }

                    MirageEngine::Core::Math::Matrix4 GetView() const
                    {
                        return MirageEngine::Core::Math::Matrix4::LookAt(m_transform.GetPosition(), m_transform.GetPosition() + m_transform.Forward(), m_transform.Up());
                    }

                    // SetParameter
                    void SetNear(float _near) { m_Near = _near; }

                    void SetFar(float _far) { m_Far = _far; }

                    void SetWidth(unsigned int _width) { m_Width = _width; }

                    void SetHeight(unsigned int _height) { m_Height = _height; }

                    void SetFov(float _fov) { m_Fov = _fov; }

                    // Getters
                    float GetNear() const { return m_Near; }

                    float GetFar() const { return m_Far; }

                    float GetFov() const { return m_Fov; }

                    unsigned int GetWidth() const { return m_Width; }

                    unsigned int GetHeight() const { return m_Height; }

                    // debug
                    void BindInput(std::shared_ptr<MirageEngine::Core::Input::InputMapping> map);

                private:

                    void Move(Input::CallbackInputInfo info);
                    void View(Input::CallbackInputInfo info);
            };
        } // namespace Components
    } // namespace Core
} // namespace MirageEngine
