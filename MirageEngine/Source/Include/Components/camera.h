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
		}

		namespace Components
		{
			class Camera// : public Component
			{
			private:
				// Clipping Planes
				float m_Near, m_Far = 0.f;
				unsigned int m_Width, m_Height = 0;
				float m_Fov = 90.f;

				MirageEngine::Core::Components::Transform m_transform;
			public:
				inline Camera(unsigned int _width, unsigned int _height, float _fov, float _near, float _far, const MirageEngine::Core::Math::Vector3 _position)
					:m_Width(_width), m_Height(_height), m_Fov(_fov), m_Near(_near), m_Far(_far)
				{
					m_transform.SetPosition(_position);
				}
				~Camera() = default;

				void LookAt(const MirageEngine::Core::Math::Vector3& target)
				{
					m_transform.LookAt(target);
				}

				MirageEngine::Core::Components::Transform& Transform() { return m_transform; }
				MirageEngine::Core::Math::Matrix4 GetPerspective() const { return MirageEngine::Core::Math::Matrix4::GetPerspectiveMatrix(m_width, m_height, m_near, m_far, m_fov); }
				MirageEngine::Core::Math::Matrix4 GetView() const { return MirageEngine::Core::Math::Matrix4::LookAt(m_transform.GetPosition(), m_transform.GetPosition() + m_transform.Forward(), m_transform.Up()); }

				// SetParameter
				inline void SetNear(float _near) { m_Near = _near; }
				inline void SetFar(float _far) { m_Far = _far; }
				inline void SetWidth(unsigned int _width) { m_Width = _width; }
				inline void SetHeight(unsigned int _height) { m_Height = _height; }
				inline void SetFov(float _fov) { m_Fov = _fov; }
				// debug
				void BindInput(MirageEngine::Core::Input::InputMapping* map);
			};
		}
	}
}