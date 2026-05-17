#include <Camera/i_camera.h>

using namespace MirageEngine::Core;

ICamera::ICamera(unsigned int _width, unsigned int _height,
    float _fov, float _near, float _far, const Math::Vector3 _position)
    : m_Width(_width)
    , m_Height(_height)
    , m_Fov(_fov)
    , m_Near(_near)
    , m_Far(_far)
    , m_Position(_position)
{
}

Math::Matrix4 ICamera::GetPerspective() const
{
    return MirageEngine::Core::Math::Matrix4::GetPerspectiveMatrix(m_Width, m_Height, m_Near, m_Far, m_Fov);
}

MirageEngine::Core::Math::Matrix4 MirageEngine::Core::ICamera::GetView() const
{
    Math::Vector3 forward = Math::Vector3(0, 0, -1).Rotate(m_Rotation.X, m_Rotation.Y, m_Rotation.Z);
    Math::Vector3 up = Math::Vector3(0, 1, 0).Rotate(m_Rotation.X, m_Rotation.Y, m_Rotation.Z);
    return MirageEngine::Core::Math::Matrix4::LookAt(m_Position, m_Position + forward, up);
}
