#include <Light/point_light.h>

#include <Light/light_manager.h>
#include <GameObject/game_object.h>
#include <Logs/log.h>
#include <entt/entt.hpp>
DEFINE_LOG_CATEGORY(LogPointLight)

namespace MirageEngine
{
    PointLight::PointLight()
    {
        p_Type = LightType::POINT;
        LOG(logTemp, LogPointLight, "Point light created");
    }
} // namespace MirageEngine
