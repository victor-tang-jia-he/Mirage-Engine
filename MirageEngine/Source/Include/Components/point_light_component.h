#pragma once 

#include <Light/point_light.h>
#include <Components/component.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPointLightComponent)

namespace MirageEngine
{
	class PointLight;
	namespace Core::Components
	{
		class PointLightComponent : public Component
		{
			std::shared_ptr<MirageEngine::PointLight> m_Light;

		public:
			PointLightComponent();
			PointLightComponent(GameObject* _owner);
			~PointLightComponent();
			static void AttachNew(std::shared_ptr<GameObject> _owner);

			Core::Math::Vector3 GetPosition() const { return m_Light->GetPosition(); }
			Core::Math::Vector4 GetColor() const { return m_Light->GetColor(); }
			Core::Math::Vector4 GetIntensity() const { return m_Light->GetIntensity(); }

			void SetPosition(const Core::Math::Vector3& _position) { m_Light->SetPosition(_position); }
			void SetColor(Core::Math::Vector4 _color) { m_Light->SetColor(_color); }
			void SetIntensity(float _intensity) { m_Light->SetIntensity(_intensity); }

			entt::id_type GetType() const override;
		};
	} // namespace MirageEngine::Core::Components
}