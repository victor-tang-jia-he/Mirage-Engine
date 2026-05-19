#include <Components/point_light_component.h>
#include <Light/light_manager.h>
#include <GameObject/game_object.h>

namespace MirageEngine::Core::Components
{
	PointLightComponent::PointLightComponent()
	{
		m_Light = LIGHT_MANAGER.CreatePointLight();		
		
		m_Light->GetParentWorldPosition = [this]() -> Core::Math::Vector3
			{
				if (m_Owner)
					return m_Owner->Transform.GetWorldMatrix().GetTranslation() + m_Light->GetPosition();
				return m_Light->GetPosition();
			};
	}

	PointLightComponent::PointLightComponent(GameObject* _owner) 
		: Component(_owner)
	{
		m_Light = LIGHT_MANAGER.CreatePointLight();
		m_Light->GetParentWorldPosition = [this]() -> Core::Math::Vector3
			{
				if (m_Owner)
					return m_Owner->Transform.GetWorldMatrix().GetTranslation() + m_Light->GetPosition();
				return m_Light->GetPosition();
			};
	}
	void PointLightComponent::AttachNew(std::shared_ptr<GameObject> _owner)
	{
		std::shared_ptr<PointLightComponent> compPtr = std::make_shared<PointLightComponent>(_owner.get());
		_owner->AddComponent<PointLightComponent>(compPtr);
	}
	entt::id_type PointLightComponent::GetType() const
	{
		return entt::type_id<PointLightComponent>().hash();
	}
}
