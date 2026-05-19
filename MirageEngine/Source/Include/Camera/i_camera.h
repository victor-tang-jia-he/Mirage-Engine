#pragma once

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
            ICamera(unsigned int _width, unsigned int _height,
                float _fov, float _near, float _far, const Math::Vector3 _position);
            ~ICamera() = default;

            MirageEngine::Core::Math::Matrix4 GetPerspective() const;
            MirageEngine::Core::Math::Matrix4 GetView() const;

            // Setters
            void SetPosition(Math::Vector3 _position) { m_Position = _position; }
            void SetRotation(Math::Vector3 _rotation) { m_Rotation = _rotation; }
            void SetNear(float _near) { m_Near = _near; }
            void SetFar(float _far) { m_Far = _far; }
            void SetWidth(unsigned int _width) 
            { 
                m_Width = _width;
            }
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
