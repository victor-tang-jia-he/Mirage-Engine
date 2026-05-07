#include <Light/point_light.h>

#include <Light/light_manager.h>
#include <Logs/log.h>
#include <entt/entt.hpp>
DEFINE_LOG_CATEGORY(LogPointLight)

namespace MirageEngine
{
    PointLight::PointLight()
    {
        Type = LightType::Point;
    }

    void PointLight::OnStart()
    {
        LOG(logTemp, LogPointLight, "Point light created");
        LightManager::Instance().AddPointLight(shared_from_this());
    }

    entt::id_type PointLight::GetType() const { return entt::type_id<PointLight>().hash(); }
} // namespace MirageEngine
