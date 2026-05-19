#include <Light/directional_light.h>

#include <Light/light_manager.h>
#include <GameObject/game_object.h>
#include <entt/entt.hpp>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogDirectionalLight)

namespace MirageEngine
{
    DirectionalLight::DirectionalLight()
    {
        p_Type = LightType::DIRECTIONAL;
        LOG(logTemp, LogDirectionalLight, "Directional light created");
    }
} // namespace MirageEngine
