#pragma once 

#include <Light/directional_light.h>
#include <Components/component.h>

namespace MirageEngine
{
	class DirectionalLight;
	namespace Core::Components
	{
		class DirectionalLightComponent : public Component
		{
			std::shared_ptr<MirageEngine::DirectionalLight> m_Light;

		public:
			DirectionalLightComponent();
			DirectionalLightComponent(GameObject* _owner);
			~DirectionalLightComponent() = default;

			static void AttachNew(std::shared_ptr<GameObject> _owner);

			Core::Math::Vector3 GetDirection() const { return m_Light->GetDirection(); }
			Core::Math::Vector4 GetColor() const { return m_Light->GetColor(); }
			Core::Math::Vector4 GetIntensity() const { return m_Light->GetIntensity(); }

			void SetDirection(const Core::Math::Vector3& _direction) { m_Light->SetDirection(_direction); }
			void SetColor(Core::Math::Vector4 _color) { m_Light->SetColor(_color); }
			void SetIntensity(float _intensity) { m_Light->SetIntensity(_intensity); }

			entt::id_type GetType() const override;
		};
	} // namespace MirageEngine::Core::Components
}