#include <Light/light_manager.h>

#include <Light/spot_light.h>
#include <Light/directional_light.h>
#include <Light/point_light.h>

namespace MirageEngine
{
    void LightManager::AddDirectionalLight(std::shared_ptr<DirectionalLight> light) { m_DirectionalLights.push_back(light); }

    void LightManager::AddPointLight(std::shared_ptr<PointLight> light) { m_PointLights.push_back(light); }

    void LightManager::AddSpotLight(std::shared_ptr<SpotLight> light) { m_SpotLights.push_back(light); }

    const std::vector<std::weak_ptr<DirectionalLight>>& LightManager::GetDirectionalLights() const { return m_DirectionalLights; }

    const std::vector<std::weak_ptr<PointLight>>& LightManager::GetPointLights() const { return m_PointLights; }

    const std::vector<std::weak_ptr<SpotLight>>& LightManager::GetSpotLights() const { return m_SpotLights; }
} // namespace MirageEngine
