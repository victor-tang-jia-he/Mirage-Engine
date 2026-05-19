#include <Components/directional_light_component.h>
#include <Light/light_manager.h>
#include <GameObject/game_object.h>

namespace MirageEngine::Core::Components
{
	DirectionalLightComponent::DirectionalLightComponent()
	{
		m_Light = LIGHT_MANAGER.CreateDirectionalLight();
	}

	DirectionalLightComponent::DirectionalLightComponent(GameObject* _owner)
		: Component(_owner)
	{
		m_Light = LIGHT_MANAGER.CreateDirectionalLight();
	}
	void DirectionalLightComponent::AttachNew(std::shared_ptr<GameObject> _owner)
	{
		std::shared_ptr<DirectionalLightComponent> compPtr = std::make_shared<DirectionalLightComponent>(_owner.get());
		_owner->AddComponent<DirectionalLightComponent>(compPtr);
	}
	entt::id_type DirectionalLightComponent::GetType() const
	{
		return entt::type_id<DirectionalLightComponent>().hash();
	}
}