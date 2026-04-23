#pragma once
#include <Components/transform.h>
#include "component.h"

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
            class Camera : public Component
            {
                private:

                    // Clipping Planes
                    float        m_Near, m_Far     = 0.f;
                    unsigned int m_Width, m_Height = 0;
                    float        m_Fov            = 90.f;
                    float        m_CameraVelocity = 1.f;

                    MirageEngine::Core::Components::Transform m_transform;

                public:

                    Camera() = default;
                    Camera(unsigned int _width, unsigned int _height, float _fov, float _near, float _far, const MirageEngine::Core::Math::Vector3 _position);
                    ~Camera() = default;

                    void Init() override {};
                    void Update() override {};

                    entt::id_type GetType() const override { return entt::type_id<Camera>().hash(); }

                    MirageEngine::Core::Components::Transform& Transform() { return m_transform; }

                    MirageEngine::Core::Math::Matrix4 GetPerspective() const { return MirageEngine::Core::Math::Matrix4::GetPerspectiveMatrix(m_Width, m_Height, m_Near, m_Far, m_Fov); }

                    MirageEngine::Core::Math::Matrix4 GetView() const
                    {
                        return MirageEngine::Core::Math::Matrix4::LookAt(m_transform.GetPosition(), m_transform.GetPosition() + m_transform.Forward(), m_transform.Up());
                    }

                    // SetParameter
                    inline void SetNear(float _near) { m_Near = _near; }

                    inline void SetFar(float _far) { m_Far = _far; }

                    inline void SetWidth(unsigned int _width) { m_Width = _width; }

                    inline void SetHeight(unsigned int _height) { m_Height = _height; }

                    inline void SetFov(float _fov) { m_Fov = _fov; }

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
