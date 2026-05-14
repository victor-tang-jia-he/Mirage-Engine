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
        m_Type = LightType::Point;
    }

    PointLight::PointLight(GameObject* _owner)
		: Component(_owner)
    {
        m_Type = LightType::Point;
    }

    void PointLight::AttachNew(std::shared_ptr<GameObject> _owner)
    {
        std::shared_ptr<PointLight> compPtr = std::make_shared<PointLight>(_owner.get());
        _owner->AddComponent<PointLight>(compPtr);
    }

    void PointLight::OnStart()
    {
        LOG(logTemp, LogPointLight, "Point light created");
        LightManager::Instance().AddPointLight(shared_from_this());
    }

    entt::id_type PointLight::GetType() const { return entt::type_id<PointLight>().hash(); }
} // namespace MirageEngine
