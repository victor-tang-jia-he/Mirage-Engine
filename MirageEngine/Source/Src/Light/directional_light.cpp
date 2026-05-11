#include <Light/directional_light.h>

#include <Light/light_manager.h>
#include <Logs/log.h>
#include <entt/entt.hpp>
DEFINE_LOG_CATEGORY(LogDirectionalLight)

namespace MirageEngine
{
    DirectionalLight::DirectionalLight()
    {
        Type = LightType::Directional;
    }

    void DirectionalLight::OnStart()
    {
        LOG(logTemp, LogDirectionalLight, "Directional light created");
        LightManager::Instance().AddDirectionalLight(shared_from_this());
    }

    entt::id_type DirectionalLight::GetType() const { return entt::type_id<DirectionalLight>().hash(); }
} // namespace MirageEngine
