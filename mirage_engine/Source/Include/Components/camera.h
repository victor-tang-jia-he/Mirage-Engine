#pragma once
#include <transform.h>

namespace MirageEngine
{
	namespace Core
	{
		namespace Input
		{
			struct InputMapping;
		}
	}
}

namespace MirageEngine
{
	namespace Core
	{
		namespace Components
		{
			class Camera
			{
				// Clipping Planes
				float m_near, m_far = 0.f;
				unsigned int m_width, m_height = 0;
				float m_fov = 90.f;

				MirageEngine::Core::Components::Transform m_transform;
			public:
				inline Camera(unsigned int _width, unsigned int _height, float _fov, float _near, float _far, const MirageEngine::Core::Math::Vector3 _position)
					:m_width(_width), m_height(_height), m_fov(_fov), m_near(_near), m_far(_far)
				{
					m_transform.SetPosition(_position);
				}
				~Camera() = default;

				void LookAt(const MirageEngine::Core::Math::Vector3& target)
				{
					m_transform.LookAt(target);
				}

				MirageEngine::Core::Math::Matrix4 GetPerspective() const { return MirageEngine::Core::Math::Matrix4::GetPerspectiveMatrix(m_width, m_height, m_near, m_far, m_fov); }
				MirageEngine::Core::Components::Transform& Transform() { return m_transform; }

				// SetParameter
				inline void SetNear(float _near) { m_near = _near; }
				inline void SetFar(float _far) { m_far = _far; }
				inline void SetWidth(unsigned int _width) { m_width = _width; }
				inline void SetHeight(unsigned int _height) { m_height = _height; }
				inline void SetFov(float _fov) { m_fov = _fov; }

				// debug
				void BindInput(MirageEngine::Core::Input::InputMapping* map);
			};
		}
	}
}