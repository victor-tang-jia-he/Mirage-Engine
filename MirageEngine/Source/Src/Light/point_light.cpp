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

    PointLight::PointLight(GameObject* _owner)
		: Component(_owner)
    {
        p_Type = LightType::POINT;
        LOG(logTemp, LogPointLight, "Point light created");
    }

    void PointLight::AttachNew(std::shared_ptr<GameObject> _owner)
    {
        std::shared_ptr<PointLight> compPtr = std::make_shared<PointLight>(_owner.get());
		LightManager::Instance().AddPointLight(compPtr);
        _owner->AddComponent<PointLight>(compPtr);
    }

    void PointLight::OnStart()
    {
    }

    entt::id_type PointLight::GetType() const { return entt::type_id<PointLight>().hash(); }
} // namespace MirageEngine
