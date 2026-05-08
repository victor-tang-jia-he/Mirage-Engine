#include <Light/spot_light.h>

#include <Light/light_manager.h>
#include <Logs/log.h>
#include <entt/entt.hpp>
DEFINE_LOG_CATEGORY(LogSpotLight)

namespace MirageEngine
{
    SpotLight::SpotLight()
    {
        Type = LightType::Spot;
    }

    void SpotLight::OnStart()
    {
        LOG(logTemp, LogSpotLight, "Spot light created");
        LightManager::Instance().AddSpotLight(shared_from_this());
    }

    entt::id_type SpotLight::GetType() const { return entt::type_id<SpotLight>().hash(); }
} // namespace MirageEngine
