#include <Light/spot_light.h>

#include <Light/light_manager.h>
#include <GameObject/game_object.h>
#include <Logs/log.h>
#include <entt/entt.hpp>
DEFINE_LOG_CATEGORY(LogSpotLight)

namespace MirageEngine
{
    SpotLight::SpotLight()
    {
        p_Type = LightType::SPOT;
    }

    SpotLight::SpotLight(GameObject* _owner)
		: Component(_owner)
    {
        p_Type = LightType::SPOT;
    }

    void SpotLight::AttachNew(std::shared_ptr<GameObject> _owner)
    {
        std::shared_ptr<SpotLight> compPtr = std::make_shared<SpotLight>(_owner.get());
		LightManager::Instance().AddSpotLight(compPtr);
        _owner->AddComponent<SpotLight>(compPtr);
    }

    void SpotLight::OnStart()
    {
        LOG(logTemp, LogSpotLight, "Spot light created");
        LightManager::Instance().AddSpotLight(shared_from_this());
    }

    entt::id_type SpotLight::GetType() const { return entt::type_id<SpotLight>().hash(); }
} // namespace MirageEngine
