#pragma once

#include <vector3.h>
#include <matrix4.h>

namespace MirageEngine
{
    namespace Core
    {
        class ICamera
        {
        protected:
            Math::Vector3 m_Position;
            Math::Vector3 m_Rotation;

            float m_Near = 0.1f;
            float m_Far = 100.0f;
            unsigned int m_Width = 1920;
            unsigned int m_Height = 1080;
            float m_Fov = 90.0f;

        public:

            ICamera() = default;
            ICamera(unsigned int _width, unsigned int _height, float _fov, float _near, float _far, const Math::Vector3 _position)
                : m_Width(_width)
                , m_Height(_height)
                , m_Fov(_fov)
                , m_Near(_near)
                , m_Far(_far)
                , m_Position(_position)
            {}
            ~ICamera() = default;

            MirageEngine::Core::Math::Matrix4 GetPerspective() const { return MirageEngine::Core::Math::Matrix4::GetPerspectiveMatrix(m_Width, m_Height, m_Near, m_Far, m_Fov); }

            MirageEngine::Core::Math::Matrix4 GetView() const
            {
                Math::Vector3 forward = Math::Vector3(0, 0, -1).Rotate(m_Rotation.X, m_Rotation.Y, m_Rotation.Z);
                Math::Vector3 up = Math::Vector3(0, 1, 0).Rotate(m_Rotation.X, m_Rotation.Y, m_Rotation.Z);
                return MirageEngine::Core::Math::Matrix4::LookAt(m_Position, m_Position + forward, up);
            }

            // SetParameter
            void SetPosition(Math::Vector3 _position) { m_Position = _position; }
            void SetRotation(Math::Vector3 _rotation) { m_Rotation = _rotation; }
            void SetNear(float _near) { m_Near = _near; }
            void SetFar(float _far) { m_Far = _far; }
            void SetWidth(unsigned int _width) { m_Width = _width; }
            void SetHeight(unsigned int _height) { m_Height = _height; }
            void SetFov(float _fov) { m_Fov = _fov; }

            // Getters
            Math::Vector3 GetPosition() const { return m_Position; }
            Math::Vector3 GetRotation() const { return m_Rotation; }
            float GetNear() const { return m_Near; }
            float GetFar() const { return m_Far; }
            float GetFov() const { return m_Fov; }
            unsigned int GetWidth() const { return m_Width; }
            unsigned int GetHeight() const { return m_Height; }
        };
    } // namespace Core
} // namespace MirageEngine
